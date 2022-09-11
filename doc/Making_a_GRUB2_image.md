I follow this [guide](https://wiki.osdev.org/User:Graham/Making_a_GRUB2_image), it works on `Ubuntu 20.04.3 LTS`.

## Making the image
Create an empty image file and set up a loop device for it, just pick a valiable loop will be fine, you can check it by `sudo losetup`, here we used loop0:
```
dd if=/dev/zero of=disk.img bs=512 count=32130
sudo losetup /dev/loop0 disk.img
```

Create an MS-DOS partition table with fdisk:
```
sudo fdisk /dev/loop0
```
Within the fdisk prompt type "o" to create a partition table, then "n" to create a new partition. Type "p" to create a primary partition and make it the first partition with "1". I left all the other settings as their defaults by hitting enter. Finally, use "w" to save the changes.
```
Welcome to fdisk (util-linux 2.34).

Changes will remain in memory only, until you decide to write them.

Be careful before using the write command.


Command (m for help): o
Created a new DOS disklabel with disk identifier 0xf125d318.

Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (1-4, default 1): 1
First sector (2048-32129, default 2048):
Last sector, +/-sectors or +/-size{K,M,G,T,P} (2048-32129, default 32129):

Created a new partition 1 of type 'Linux' and of size 14.7 MiB.
Command (m for help): w

The partition table has been altered.
Calling ioctl() to re-read partition table.
Re-reading the partition table failed.: Invalid argument

The kernel still uses the old table. The new table will be used at the next reboot or after you run partprobe(8) or kpartx(8).
```

Find the first sector of the partition with fdisk:
```
sudo fdisk -lu /dev/loop0
```

```
Disk /dev/loop0: 15.71 MiB, 16450560 bytes, 32130 sectors

Units: sectors of 1 * 512 = 512 bytes

Sector size (logical/physical): 512 bytes / 512 bytes

I/O size (minimum/optimal): 512 bytes / 512 bytes

Disklabel type: dos

Disk identifier: 0xc52c1f4c


Device        Boot Start   End Sectors  Size Id Type

/dev/loop0p1       2048 32129   30082 14.7M 83 Linux
```
You need to multiply the number under "Start" by 512. For me, this was `2048` * 512 = 1048576. Now create a loop device for this partition (like how /dev/sda and /dev/sda0 would work on your normal system):
```
sudo losetup -o 1048576 /dev/loop1 /dev/loop0
```

Format this as ext2 and mount it to a temporary location:

```
sudo mke2fs /dev/loop1
sudo mkdir -p /mnt/d
sudo mount /dev/loop1 /mnt/d
```
Install GRUB:
```
 sudo grub-install --root-directory=/mnt/d --disk-module=biosdisk --modules="ext2 part_msdos" /dev/loop0
```
Now unmount the partition and remove the loop devices:
```
sudo umount /mnt/d
sudo losetup -d /dev/loop1
sudo losetup -d /dev/loop0
rmdir /mnt/d
```
And test it:
```
qemu-system-x86_64 -hda ./disk.img
```
If you get a GRUB prompt then it worked!
