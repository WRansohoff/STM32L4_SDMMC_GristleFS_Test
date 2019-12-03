# `Gristle` FAT Filesystem Test Firmware

This is a simple firmware image to test the `Gristle` FAT filesystem with an STM32L496RG chip using its SD/MMC peripheral to communicate with a microSD card.

It is not currently functional, and the following description is aspirational.

First, it turns on an on-board LED to indicate that the microSD card is being used and power should not be turned off. Then, it initializes the SD card and finds its storage capacity. Finally, creates a `test.txt` file with a simple 'Hello, world' message in the root directory, erasing it if it already exists. Oh, and then it turns off the LED to indicate that you can safely turn the power off.

Although, FAT filesystems sound fairly simple and this application does not intentionally cache writes, so I hope that the chances of SD card corruption are fairly low.

# Current Status

The SD/MMC peripheral initializes successfully, and it can send/receive command data to and from a connected SD Card. But I'm still working on the initialization procedure, which is currently stuck at the `CMD3` command, which is supposed to ask the card to publish a new 'address' to refer to it by. But that command times out; I think that `CMD2` might need to be called first.

Data transmission on the 4-bit SD card bus and the filesystem library are both still untested.

I'm also using a 'high-capacity' microSD card for testing, so 'standard-capacity' cards and/or cards which do not support V2.00 of the specification or higher are also untested.
