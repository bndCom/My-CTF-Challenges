# Certified FS Investigation Expert

## Overview
We are provided with a disk image containing two partitions. The second partition can be mounted, but the first one appears to be corrupted and cannot be mounted.

## Solution
- The disk uses MBR partitioning. The first partition is FAT32 (as identified using `fdisk`), and it is corrupted.
1. To retrieve the SHA-1 hash of the MBR boot code, extract the first 446 bytes.

- Using `fdisk`, we observe that the first partition starts at sector 1024. Typically, partitions start at sector 2048 with 1MB alignment. However, the starting sector can be calculated using the formula: `start of second partition - size of the first partition`.

2. To solve the rest of the challenge, we can proceed with one of the following approaches:
    1. Parse the entire disk manually, including the FAT boot sector and directory entries in the root cluster, as demonstrated in [solve.c](solve.c).
    2. Parse only the MBR, fix the entry for the first partition in the partition table, and proceed with the next steps.
    3. Use `testdisk` to automatically repair the partition table, then:
        1. Use `fsstat` to retrieve the **volume ID**.
        2. Use `sudo istat -o 0 /dev/loop0p1 18`, then add 2048 (the offset of the first partition) to the file's address to locate the file.