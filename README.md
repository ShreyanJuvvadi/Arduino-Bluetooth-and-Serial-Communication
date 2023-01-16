# Arduino-Bluetooth-and-Serial-Communication
Allows an arduino to use bluetooth and serial ports to act as an intermediate in communication

Associated with https://github.com/ShreyanJuvvadi/Windows-COM-Port-Communication

***

Function

1. Boots up immediately with power
2. Checks for data recieved
    1. If data is recieved then check verification
        1. If verification exisits (should not be possible)
            1. relay the information to hardware serial
        2. If verification does not exist, authenticate to see if the last 10 inputs are the same as the randomized values shown on the LEDs
            1. If verification passes, relay all future information to hardware serial
                1. If disconnects, remove authentication state
    2. If no data is recieved, continue on with the while loop

***

Materials needed <br />

* HM-19 bluetooth moduele (1) (Others should theroetically work with slight modificatiosn to code) <br />
* Arduino UNO (1) (Any ATmega328P 16MHz processors should work) <br />
* LEDs (10) (Can change code to use less at the expense of security) <br />
* 10k Resistor (1) <br />
* PCB or breadboarding material <br />
* Bluetoth terminal capable device (App used https://play.google.com/store/apps/details?id=com.reb.dsd_ble&hl=en_US&gl=US) <br />

***

Features include 

* Randomized password and LEDs to show password
* Verification states
* Deauthenticating

***

Files

* Main file (.ino) is for the arduino compiler
* 2 Png files to show pinouts

***

Bugs and Issues

* Long input strings, usually above 15 characters, can crash the bluetooth module
