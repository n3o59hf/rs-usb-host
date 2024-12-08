/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2024 No0ne (https://github.com/No0ne)
 *           (c) 2023 Dustin Hoffman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "ps2x2pico.h"
#include "bsp/board_api.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

int main() {
  board_init();
  sleep_ms(3000);
  printf("\n%s-%s\n", PICO_PROGRAM_NAME, PICO_PROGRAM_VERSION_STRING);
  
  tuh_hid_set_default_protocol(HID_PROTOCOL_REPORT);
  tusb_init();

  stdio_init_all();

  uart_queue_init();
  
  log_send("\n%s-%s\n", PICO_PROGRAM_NAME, PICO_PROGRAM_VERSION_STRING);
  while(1) {
    tuh_task();
    process_send_queue();
    process_receive();
  }
}

void reset() {
  printf("\n\n *** PANIC via tinyusb: watchdog reset!\n\n");
  log_send("PANIC via tinyusb: watchdog reset!");
  watchdog_enable(1000, false);
}
