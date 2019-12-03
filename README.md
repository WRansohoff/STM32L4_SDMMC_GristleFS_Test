# `Gristle` FAT Filesystem Test Firmware

This is a simple firmware image to test the `Gristle` FAT filesystem with an STM32L496RG chip using its SD/MMC peripheral to communicate with a microSD card.

It is not currently functional, and the following description is aspirational.

First, it turns on an on-board LED to indicate that the microSD card is being used and power should not be turned off. Then, it initializes the SD card and finds its storage capacity. Finally, it creates a `test.txt` file with a simple 'Hello, world' message in the root directory, erasing it if it already exists. Oh, and then it turns off the LED to indicate that you can safely turn the power off.

Although, FAT filesystems sound fairly simple and this application does not intentionally cache writes, so I hope that the chances of SD card corruption are fairly low.

# Current Status

The SD/MMC peripheral initializes successfully, and the card's capacity in 512-byte blocks seems to be received and calculated correctly. I think that the SD card initialization succeeds as well, but the methods to read and write blocks of data don't seem to work yet. Currently, the `sdmmc_read_block` method gets stuck waiting for the 'receive buffer not empty' flag to be set, so something is wrong there.

The filesystem library is still untested.

I'm also using a 16GB 'high-capacity' microSD card for testing, so 'standard-capacity' cards and/or cards which do not support V2.00 of the specification or higher are also untested.
