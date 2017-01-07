## The multimode beacon is based on Arduino + si5351 module.

Supported modes:

* WSPR2
* WSPR15
* JT65
* JT9
* JT4
* ISCAT

Key features:

* Internal encoder for WSPR and ISCAT.
* Simple "in code configuration".
* Useful on-line mode configurator.
* TX start is controlled by extremely accurate I2C real-time clock Ds3231.
* Terminal-based command line interface.
* Support band-hopping with LPF auto selection.
* Pin-compliant with QRP-LABS Arduino shield and relay-switch board.

To configure base frequency, step of FSK manipulation, T/R interval, mode - please use javascript based configurator: http://ra9yer.blogspot.com/p/si5351-configurator.html or open local configurator : ./doc/band_configurator.html 

The WSPR beacon message in defined directly  in the code in text form. Please use command-line utilities _jt65code_, _jt4code_,  _jt9code_ to generate symbol stream for these modes.

Updated at Jenuary 2017. Experimental feature: GPS module instead of DS3231. Please see macros TIME_SLICE_GPS/TIME_SLICE_DS3231 in si5351-beacon.ino file.

Terminal commands:

Command | Description | Example
------------ | -------------- | --------------
ts | Time Show (show current time) | ts
ts N | Time Show N times | ts 5
start | Start transmission | start
stop | Terminate transmission | stop
activate | Activate autostart TX mode | activate
deactivate | Deactivate autostart TX mode | deactivate
si | Show system info (time+launch time) | si
bhe | Band hopping enable | bhe
bhd | Band hopping disable | bhd
nb | Next band (useful when band hopping is disabled) | nb
temperature | show current temperature measured by Ds3231 | temperature 
setdate | update time in Ds3231 chip | setdate 2016-05-01 12:35:30


Please configure terminal with **newline**  termination.
Command `setdate` - in-chip datetime will be updated as soon as you hit enter.

![Block diagram](https://2.bp.blogspot.com/-hdYOkN2AvIw/VybUynSdfFI/AAAAAAAAAJ8/lczwSbtwa54h3cWx3Q5UmBE4t_etAARygCLcB/s320/arduino-ds3231-si5351.png)

![qrp-labs arduio shield](https://3.bp.blogspot.com/-Rw9oxR0kns4/VybUZYk75xI/AAAAAAAAAJ4/JWqDjWBB75sM4XN4ooix6PZDgHmby1rtwCLcB/s320/arduin-si5351-qrplabs.jpg)

After downloading, rename folder to 'si5351-beacon'. It is arduino obligatory, the folder name should have the same name as *.ino file has.
The project was tested in Arduino 1.6.7 development environment. If you have any problems in sketch compiling please use exactly this version.


Si5351 module, arduio-shield, LPF's, relay-switched board was purchased from http://www.qrp-labs.com  
Ds3231 module - from aliexpress.  
Arduino Leonardo - from local shop.  

Link to blog post: http://ra9yer.blogspot.com/2016/05/arduino-si5351-beacon.html

