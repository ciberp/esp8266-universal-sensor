# esp8266 universal sensor firmware

An open source arduino based firmware for esp8266. To which we connect the desired sensors
and configure them in the settings. Sensors data is displayed on webpage and can be pushed
or pulled from it. Data can be periodically pushed:
 - to <a href="https://domoticz.com/">Domoticz</a> (home automation system) via domoticz http api
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
 
## Flashing the firmware using windows tool <a href="https://github.com/nodemcu/nodemcu-flasher">NodeMCU Flasher</a>

Download <a href="https://github.com/nodemcu/nodemcu-flasher/raw/master/Win32/Release/ESP8266Flasher.exe">Win32 Windows Flasher</a><br>
Download <a href="https://github.com/nodemcu/nodemcu-flasher/raw/master/Win64/Release/ESP8266Flasher.exe">Win64 Windows Flasher</a><br>
Download <a href="https://github.com/ciberp/esp8266-universal-sensor/raw/master/universal-sensor.bin">universal-sensor.bin</a>

Set universal-sensor.bin and set offset to 0x00000<br>
![flasher](https://github.com/nodemcu/nodemcu-flasher/blob/master/Resources/Images/NodeMCU-Flasher-Setting.png)<br>
Choose proper COM port and click Flash<br>
![flasher](https://github.com/nodemcu/nodemcu-flasher/blob/master/Resources/Images/NodeMCU-Flasher-Success.png)

## Flashing the firmware using <a href="https://github.com/espressif/esptool">esptool.py</a>
 - install esptool (debian): 
        
        user@hp:~ > sudo pip install esptool
 - connect esp8266 board
 - find to which port is connected: 
        
        user@hp:~ > dmesg | grep USB
   and search for something like: 
   
        usb 6-1: ch341-uart converter now attached to ttyUSB0
 - download <a href="https://github.com/ciberp/esp8266-universal-sensor/raw/master/universal-sensor.bin">universal-sensor.bin</a>
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
and configure system settings to your needs. Next time esp8266 will reboot it will automatically connect to your AP set in settings. If it can't connect it will again automatically turn on own AP with ap ssid and ap psk set in settings. To see esp8266 IP address when connected to your wifi check serial terminal:

    user@hp:~ > miniterm.py /dev/ttyUSB0 115200
    --- Miniterm on /dev/ttyUSB0  115200,8,N,1 ---
    --- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
    ;l␀d��|␀�$�|␃␄␌␄�␌d�␄c|ǃ␂�␛�{�c�␌b��og�l'o���␌#␜x�l{l{$p�'�␘␃␄
    AP IP address: 192.168.4.1


    Connecting to WiFi..
    Connected to SSID: W32.Blaster.Worm
    IP address: 172.22.0.18
    Looking for 1-Wire devices...

## Screenshots
root webpage
![universal-sensor-root](https://user-images.githubusercontent.com/23559198/33344753-53df22e4-d48a-11e7-9f35-2d9ede40e03d.png)
settings (http://IP_address/settings)
![universal-sensor-settings](https://user-images.githubusercontent.com/23559198/33344860-a36f7c32-d48a-11e7-819f-ddf45ef0c93c.png)
json (http://IP_address/json)
![universal-sensor-json](https://user-images.githubusercontent.com/23559198/33344947-e770307a-d48a-11e7-8612-707541fe1836.png)
