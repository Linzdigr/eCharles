# eCharles

Raspberry Pi C++ home project with autonomous domotics depending on sensors value.

## Prerequisites
We recommend Raspberry Pi Model 3 to run this project.

WiringPi have to be installed: (http://wiringpi.com/download-and-install/).

Before going further, copy the conf file into /etc:
```sh
cp -a ./otal.cnf /etc/otal.cnf
```

Now, we install Docker and docker-compose:

```sh
sudo apt update && \
sudo apt upgrade && \
sudo apt install raspberrypi-kernel raspberrypi-kernel-headers && \
(sudo curl -sSL https://get.docker.com | sh) && \
sudo apt install docker-compose git libi2c-dev && \
sudo usermod -aG docker ${USER}
```

## Build & Installation

```sh
git clone https://github.com/Linzdigr/eCharles
cd eCharles
make
make install
```

In order to the Pi to start the Daemon at boot time, you might want to add the following to your /etc/rc.local:

```sh
echarles &
```

-----

In case of FS failure on SD Card:

```sh
fdisk -l
fsck.fat -y /dev/mmcblk0p1 # (Check your correct number)
fsck.ext4 -y /dev/mmcblk0p2 # (Check your correct number)
```
