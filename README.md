# SumoBot--STEMmo-Wrestlers
This is a repository of the C++ programs written for my 2025 Sumobot project. It also includes some 3D Models of the final design, as well as some prototypes. 

My bot used an Arduino Nano ATmega, so some model specific things in the code may cause you issues- Most notably, which pins are PWM and hardware interrupt capable. The other components I used were two 12 Volt DC motors, a TB6612FNG motor driver, a rechargeable 9V battery, a TCRT5000 IR sensor, and an HCSR04 Ultrasonic sensor. Note that with only one 9V battery, if you run the motors too high it will drain the logic voltage (from the motor driver) and mess up sensor inputs. To deal with that, I ended up adding another 9V battery. 

The Sumo. files includes a class I wrote to make programming bot behaviour simple- most of it is self explanatory. At the start of main, simply create a Sumo object, and through it you do all the action calls. 

In Main, I use a big switch statement to handle bot behaviour, and hardware interrupts to quickly change actions if the infrared sensor is tripped, or if the rear impact button is triggered. 

I included some CAD files in the form of a .ZIP (Autodesk Inventor formats) of my final version. You can 3D print every component except the electronics. 
