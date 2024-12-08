#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/util/queue.h"
#include <stdarg.h>
#include <stdio.h>

// Example
// [Start Byte] [Length] [Payload] [Checksum]
// 0x7E         0x03     0x01 0x02 0x03  0x06

#define START_BYTE 0x7E
#define MAX_PAYLOAD_SIZE 255

#define PACKET_ID_KEYBOARD 0x01
#define PACKET_ID_MOUSE    0x02
#define PACKET_ID_LOG      0x03

typedef struct {
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
} Packet;

queue_t send_queue;

// Function to calculate checksum (XOR of payload bytes)
uint8_t calculate_checksum(const uint8_t *payload, uint8_t length) {
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++) {
        checksum ^= payload[i];
    }
    return checksum;
}

bool queue_packet(const uint8_t *payload, uint8_t length) {
    Packet packet;
    packet.length = length;
    for (uint8_t i = 0; i < length; i++) {
        packet.payload[i] = payload[i];
    }

    // Enqueue the packet
    return queue_try_add(&send_queue, &packet);
}

void process_send_queue() {
    Packet packet;
    while (!queue_is_empty(&send_queue)) {
        if (queue_try_remove(&send_queue, &packet)) {
            // Construct the full packet with start byte, length, payload, and checksum
            uint8_t uart_packet[260]; // Start byte + length + payload + checksum
            uart_packet[0] = START_BYTE;
            uart_packet[1] = packet.length;

            for (uint8_t i = 0; i < packet.length; i++) {
                uart_packet[2 + i] = packet.payload[i];
            }
            uart_packet[2 + packet.length] = calculate_checksum(packet.payload, packet.length);

            // Send the full packet
            uart_write_blocking(UART_ID, uart_packet, 3 + packet.length);
        }
    }
}



// Function to send a packet
void send_packet(const uint8_t *payload, uint8_t length) {
    uint8_t packet[260]; // Start byte + length + payload + checksum (max 255 bytes payload)
    packet[0] = START_BYTE;
    packet[1] = length;
    
    for (uint8_t i = 0; i < length; i++) {
        packet[2 + i] = payload[i];
    }
    packet[2 + length] = calculate_checksum(payload, length);

    // Send packet over UART
    uart_write_blocking(UART_ID, packet, 3 + length);
}

bool receive_packet(uint8_t *payload, uint8_t *length) {
    static enum { WAIT_START, READ_LENGTH, READ_PAYLOAD, READ_CHECKSUM } state = WAIT_START;
    static uint8_t buffer[256];
    static uint8_t pos = 0;
    static uint8_t packet_length = 0;

    while (uart_is_readable(UART_ID)) {
        uint8_t byte = uart_getc(UART_ID);

        switch (state) {
            case WAIT_START:
                if (byte == START_BYTE) {
                    state = READ_LENGTH;
                }
                break;

            case READ_LENGTH:
                packet_length = byte;
                pos = 0;
                state = READ_PAYLOAD;                
                break;

            case READ_PAYLOAD:
                buffer[pos++] = byte;
                if (pos == packet_length) {
                    state = READ_CHECKSUM;
                }
                break;

            case READ_CHECKSUM:
                state = WAIT_START; // Reset state for the next packet
                if (calculate_checksum(buffer, packet_length) == byte) {
                    *length = packet_length;
                    for (uint8_t i = 0; i < packet_length; i++) {
                        payload[i] = buffer[i];
                    }
                    return true; // Packet successfully received
                }
                // If checksum fails, ignore the packet
                break;
        }
    }

    return false; // No complete packet received yet
}

void process_receive() {
    static uint8_t length;
    static uint8_t data[255];
    if (receive_packet(data, &length)) {
        
    }
}

void uart_queue_init() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Initialize the send queue with a capacity of 10 packets
    queue_init(&send_queue, sizeof(Packet), 10);
}

void kb_send_key(uint8_t key, bool is_key_pressed, uint8_t modifiers) {
    uint8_t payload[4];
    payload[0] = PACKET_ID_KEYBOARD; // ID for keyboard packet
    payload[1] = key;
    payload[2] = is_key_pressed ? 1 : 0;
    payload[3] = modifiers;

    queue_packet(payload, 4);
}

// Function to send a mouse movement packet
void ms_send_movement(uint8_t buttons, int8_t x, int8_t y, int8_t z) {
    uint8_t payload[5];
    payload[0] = PACKET_ID_MOUSE; // ID for mouse packet
    payload[1] = buttons;
    payload[2] = (uint8_t)x; // Cast to uint8_t to match packet format
    payload[3] = (uint8_t)y;
    payload[4] = (uint8_t)z;

    queue_packet(payload, 5);
}

void log_send(const char *format, ...) {
    char log_buffer[MAX_PAYLOAD_SIZE - 1]; // Reserve 1 byte for the ID
    va_list args;
    va_start(args, format);

    // Format the string
    int formatted_length = vsnprintf(log_buffer, sizeof(log_buffer), format, args);
    printf(format, args);
    va_end(args);

    if (formatted_length < 0 || (uint)formatted_length > sizeof(log_buffer)) {
        return; // Formatting error or too long
    }

    uint8_t payload[MAX_PAYLOAD_SIZE];
    payload[0] = PACKET_ID_LOG; // ID for logging packet

    // Copy the formatted string to the payload
    for (int i = 0; i < formatted_length; i++) {
        payload[i + 1] = log_buffer[i];
    }
    queue_packet(payload, formatted_length + 1);
}