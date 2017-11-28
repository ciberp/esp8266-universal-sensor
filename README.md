# esp8266 universal sensor firmware

An open source arduino based firmware for esp8266. To which we connect the desired sensors
and configure them in the settings. Sensors data is displayed on webpage and can be pushed
or pulled from it. Data can be periodically pushed:
 - to Domoticz (home automation system) via domoticz http api
 - or via Simple http post   
Or pulled via http as json data.

Logging is also supported as syslog using TCP.

Currently supported sensors: DS18B20, DHT11, DHT22, DHT21, AM2302, BME280, K-type with MAX6675, ultrasonic HC-SR04. I/O pins as
output can be button or url controlled.


## Flashing/upgrading via OTA
 - open url:
        
        http://sensors_ip_address/espupdate
 - browse and upload binary file
 - if everything goes well esp8266 will reboot and boot up with new firmware

## Flashing the firmware using esptool.py
 - install esptool (debian): 
        
        user@hp:~ > sudo pip install esptool
 - connect esp8266 board
 - find to which port is connected: 
        
        user@hp:~ > dmesg | grep USB
   and search for something like: 
   
        usb 6-1: ch341-uart converter now attached to ttyUSB0
 - run esptool: 
 
        user@hp:~ > esptool.py --port /dev/ttyUSB0 write_flash 0x0000  universal-sensor.bin
 - esptool output:
 
        user@hp:~ > esptool.py --port /dev/ttyUSB0 write_flash 0x0000 universal-sensor.bin 
        esptool.py v2.2
        Connecting....
        Detecting chip type... ESP8266
        Chip is ESP8266EX
        Uploading stub...
        Running stub...
        Stub running...
        Configuring flash size...
        Auto-detected Flash size: 4MB
        Compressed 345008 bytes to 238239...
        Wrote 345008 bytes (238239 compressed) at 0x00000000 in 21.1 seconds (effective 131.0 kbit/s)...
        Hash of data verified.

        Leaving...
        Hard resetting...

After flashing esp8266 will boot up and turn on software access point with ssid **universal**. Connect to esp8266
ssid using psk **password**. When connected open url: 
    
    http://192.168.4.1/settings
and configure system settings to your needs. Next time esp8266 will reboot it will automatically connect to your AP set in settings. If it can't connect it will again automatically turn on own AP with ap ssid and ap psk set in settings.

