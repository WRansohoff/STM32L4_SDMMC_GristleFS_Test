// Vendor-provided device header file.
#include "stm32l4xx.h"

// BSP include.
#include "port/board_stm32l496rg.h"

// Global application values.
#include "global.h"

/**
 * Main program.
 */
int main(void) {
  // Call the high-level 'board_init' method to set up pins and
  // peripherals. The method can be found in the BSP header file.
  board_init();

  // TODO: Initialize and test the connected SD card.

  // Main application loop.
  while (1) {
    // Test application complete; wait for power-off.
  }
}
