# `Gristle` FAT Filesystem Test Firmware

This is a simple firmware image to test the `Gristle` FAT filesystem with an STM32L496RG chip using its SD/MMC peripheral to communicate with a microSD card.

It is currently untested, and the following description is aspirational.

First, it turns on an on-board LED to indicate that the microSD card is being used and power should not be turned off. Then, it initializes the SD card and finds its storage capacity. Finally, creates a `test.txt` file with a simple 'Hello, world' message in the root directory, erasing it if it already exists. Oh, and then it turns off the LED to indicate that you can safely turn the power off.

Although, FAT filesystems sound fairly simple and this application does not intentionally cache writes, so I hope that the chances of SD card corruption are fairly low.
