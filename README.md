# `Gristle` FAT Filesystem Test Firmware

This is a simple firmware image to test the `Gristle` FAT filesystem with an STM32L496RG chip using its SD/MMC peripheral to communicate with a microSD card.

First, it turns on an on-board LED to indicate that the microSD card is being used. Then, it initializes the SD card and finds its storage capacity. Finally, it creates a `test.txt` file with a simple 'Hello, world' message in the root directory. If `test.txt` already exists, it deletes the old file before creating a new one. Once everything is done, it turns off the LED to indicate that you can safely turn the power off.

FAT filesystems sound fairly simple and this application does not intentionally cache writes, so I hope that the chances of SD card corruption are fairly low. But I did brick one card while learning about the peripheral and specification, so ymmv.

# Current Status

The application and filesystem library both seem to work, but this simple firmware does not perform exhaustive tests:

* The `Gristle` library can successfully read the master boot record and mount a FAT32 partition, iff there is only a single partition on the card.

* I believe that the library can find and delete an existing `/test.txt` file, but I still need to verify that by disabling the subsequent step which creates a new file.

* The library can create a new file, buffer writes to it, and flush data to the SD card when the file is closed.

* The application can un-mount the SD card after communication is finished.

* The `/test.txt` file created by this application can be read when the SD card is plugged into a laptop, and it contains the expected message.

Please don't use an expensive SD card or one with any important data, if you do decide to try this. I'm not very confident in this code yet, and messing up SD card communication can cause permanent data loss and/or permanently damage the card. I haven't bricked any cards recently, but I broke one back in November and I would be surprised if I don't need to buy a few more as I continue to learn about SD card communication.

I'm also using a 16GB 'high-capacity' microSD card for testing, so 'standard-capacity' cards and/or cards which do not support V2.00 of the specification or higher are also untested. I tried to include logic which should support them, but I haven't tested that.
