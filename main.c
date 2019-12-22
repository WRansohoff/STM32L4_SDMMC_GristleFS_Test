// Vendor-provided device header file.
#include "stm32l4xx.h"

// BSP include.
#include "port/board_stm32l496rg.h"

// Global application values.
#include "global.h"

// Gristle FAT filesystem includes.
#include <fcntl.h>
#include "gristle.h"
#include "partition.h"

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

  // Delay briefly to allow for SD peripheral power-on.
  // TODO: Why is such a long delay required here?
  timer_delay( 1000 );

  // Initialize the connected SD card.
  if ( block_init() == -1 ) {
    gpio_lo( GPIOA, 15 );
    while( 1 ) {};
  }
  // Mount the filesystem. Assume that the entire card is formatted,
  // and find the partition location from the MBR.
  // TODO: This assumes that there is only one partition, and could
  // cause buffer overflows if there are multiple partitions (!).
  struct partition* part_list;
  // TODO: This buffer should be declared elsewhere
  // or dynamically allocated.
  uint8_t block_buf[ 512 ];
  block_read( 0, block_buf );
  int num_parts = read_partition_table( block_buf, block_get_volume_size(), &part_list );
  if ( num_parts <= 0 ) {
    gpio_lo( GPIOA, 15 );
    while( 1 ) {};
  }
  if ( fat_mount( part_list[ 0 ].start, part_list[ 0 ].length, part_list[ 0 ].type ) == -1 ) {
    gpio_lo( GPIOA, 15 );
    while( 1 ) {};
  }

  // If a file called 'test.txt' exists, erase it.
  // TODO: Is it okay to just blindly call `fat_unlink`?
  int sderr;
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
  fat_write( test_fd, "Hello, world!\0\0\0\0", 16, &sderr );
  // Close the file.
  while( fat_close( test_fd, &sderr ) == -1 ) {};

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
