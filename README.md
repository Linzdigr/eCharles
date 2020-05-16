#eCharles

Raspberry Pi C++ home project with autonomous domotics depending on sensors value.

##Installation

```sh
make
```

In order to the Pi to start the Daemon at boot time, you might want to add the following to your /etc/rc.local:

```
/etc/eCharles &
```

-----

In case of FS failure:

fdisk -l
fsck.fat -y /dev/mmcblk0p1 (Check your correct number)
fsck.ext4 -y /dev/mmcblk0p2 (Check your correct number)
