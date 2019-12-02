// Vendor-provided device header file.
#include "stm32l4xx.h"

// BSP include.
#include "port/board_stm32l496rg.h"

// Global application values.
#include "global.h"

// Gristle FAT filesystem includes.
#include "gristle.h"

/**
 * Main program.
 */
int main(void) {
  // Call the high-level 'board_init' method to set up pins and
  // peripherals. The method can be found in the BSP header file.
  board_init();

  // Initialize the connected SD card.
  block_init();
  // Mount the filesystem. Assume that the entire card is formatted,
  // and there is no partition table.
  fat_mount( 0, block_get_volume_size(), 0 );

  // Main application loop.
  while (1) {
    // Test application complete; wait for power-off.
  }
}
