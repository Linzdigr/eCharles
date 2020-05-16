# eCharles

Raspberry Pi C++ home project with autonomous domotics depending on sensors value.

## Prerequisites
This project requires wiringPi to be installed on your Pi (http://wiringpi.com/download-and-install/).

Before going further, copy the conf file into /etc:
```sh
cp -a ./otal.cnf /etc/otal.cnf
```

## Build & Installation

```sh
make
make install
```

In order to the Pi to start the Daemon at boot time, you might want to add the following to your /etc/rc.local:

```sh
eCharles &
```

-----

In case of FS failure on SD Card:

```sh
fdisk -l
fsck.fat -y /dev/mmcblk0p1 # (Check your correct number)
fsck.ext4 -y /dev/mmcblk0p2 # (Check your correct number)
```
