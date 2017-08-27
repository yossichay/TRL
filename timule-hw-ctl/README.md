# timule-hw-ctl
Timule HW Controller

## OLED

[DIYmall 0.96" 128x64 White I2C][1]

### Connecting to RPi

| Signal  | RPi Pin |
|---------|:-------:|
| VCC     | 1       |  
| GND     | 6       |
| SDA     | 3       |
| SCL     | 5       |


### Install OLED library

```sh
cd src/ArduiPi-SSD1306
sudo make
```

### Creating Bitmaps code

[Online tool][2]

### libmpdclient

[Documentation (through example)][3]

[1]: https://drive.google.com/open?id=0B8DSGdAr8_31UEItMmx6ZDJIOWs
[2]: http://javl.github.io/image2cpp/
[3]: http://libmpdclient.sourcearchive.com/documentation/2.2/example_8c-source.html
