# Setting Volumio on RPi with Audio Injector Support

## Volumio image


Install system prereqs (on your Linux machine):

```
apt install git squashfs-tools kpartx multistrap qemu-user-static samba debootstrap parted dosfstools qemu binfmt-support qemu-utils
```
Clone the build repo on your local folder

```
cd
mkdir volumio
cd volumio
git clone https://github.com/volumio/Build Build
cd Build
```

Building the image as su

```
sudo su
```

if on Ubuntu, you may need to remove `$forceyes` from line 989 of `/usr/sbin/multistrap`

Building:

```
./build.sh -b arm -d pi -v 2.0 -l reponame
```

This will take ~20 minutes to finish

When it finishes, you'll end up with the following file in the `Build` folder:

```
$ ls -l Volumio2.0-2017-07-23-pi.img 
-rw-r--r-- 1 root root 2936012800 Jul 23 14:16 Volumio2.0-2017-07-23-pi.img
```

## Enabling Audio Injector 

Use Etcher (with GUI) or dd (no GUI) to write the image to an SD card

Insert SD card to your Pi and after few minutes (first time only) you'll get login prompt.
Use `volumio` as username and password

Update repos:

```
sudo apt-get update
```

Enable SSH

```
sudo cat '' > /boot/ssh
```

Enable Audio Injector driver

```
sudo echo 'dtoverlay=audioinjector-addons' >> /boot/config.txt
```

Reboot
```
reboot
```

## Testing Audio Injector

Check that Audio Injector is recognized:

```
dmesg ~ grep | octo
```
```
[   11.914759] audioinjector-octo soc:sound: ASoC: CPU DAI (null) not registered - will retry
[   11.914776] audioinjector-octo soc:sound: snd_soc_register_card failed (-517)
[   13.477351] audioinjector-octo soc:sound: cs42448 <-> 3f203000.i2s mapping ok
```

Setup asoundrc by creating `/etc/asound.conf` and add the following:
```
pcm.!default {
#       type hw
#       card 0
        type plug
        slave.pcm "anyChannelCount"
}

ctl.!default {
        type hw
        card 0
}

pcm.anyChannelCount {
    type route
    slave.pcm "hw:0"
    slave.channels 8;
    ttable {
           0.0 1
           1.1 1
           2.2 1
           3.3 1
           4.4 1
           5.5 1
           6.6 1
           7.7 1
    }
}

ctl.anyChannelCount {
    type hw;
    card 0;
}
```
check that we can see the anychannelcount plugin like so :
```
$ aplay -L | grep -C 1 anyCh
default
anyChannelCount
sysdefault:CARD=ALSA
```
## Default usage of USB WiFi Dongle

In order to use a USN WiFi dongle as `wlan0`
add new line with `dtoverlay=pi3-disable-wifi` to `/boot/config.txt`






