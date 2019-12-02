// Vendor-provided device header file.
#include "stm32l4xx.h"

// BSP include.
#include "port/board_stm32l496rg.h"

// Global application values.
#include "global.h"

// Gristle FAT filesystem includes.
#include <fcntl.h>
#include "gristle.h"

/**
 * Main program.
 */
int main(void) {
  // Call the high-level 'board_init' method to set up pins and
  // peripherals. The method can be found in the BSP header file.
  board_init();

  // Turn on the on-board LED to indicate that the SD card may have
  // ongoing writes, so the board shouldn't be shut down.
  gpio_hi( GPIOA, 15 );

  // Initialize the connected SD card.
  int sderr;
  block_init();
  // Mount the filesystem. Assume that the entire card is formatted,
  // and there is no partition table.
  fat_mount( 0, block_get_volume_size(), 0 );

  // If a file called 'test.txt' exists, erase it.
  // TODO: Is it okay to just blindly call `fat_unlink`?
  int test_fd = fat_open( "test.txt", O_RDWR, S_IWUSR, &sderr );
  if ( test_fd != -1 ) {
    // Close the file, then unlink it.
    fat_close( test_fd, &sderr );
    fat_unlink( "test.txt", &sderr );
  }

  // Create the `test.txt` file.
  test_fd = fat_open( "test.txt",
                      ( O_CREAT | O_RDWR ),
                      S_IWUSR,
                      &sderr );
  // Enter an infinite loop if the file couldn't be created.
  // If the previous steps succeeded, this shouldn't happen.
  if ( test_fd == -1 ) { while( 1 ) {}; }
  // Write "Hello, world!" to the test file.
  fat_write( test_fd, "Hello, world!\0", 13, &sderr );
  // Close the file.
  fat_close( test_fd, &sderr );

  // Done; un-mount the filesystem to prevent SD card corruption.
  block_halt();

  // Turn on-board LED off to indicate that it's safe to turn off
  // the device.
  gpio_lo( GPIOA, 15 );

  // Main application loop.
  while (1) {
    // Test application complete; wait for power-off.
  }
}
