# attiny13_servo_demo
### Description
Small demo code to control a servo motor using an <b>attiny13</b> microcontroller. <br>
With <b>Fast PWM Mode</b>, the PWM frequency is set by the prescaler value (on an <b>attiny13</b>), so I could only get close to the "standard" 50Hz. <br>
The program sets it to 73Hz which is totally fine (servo motors can be controlled by a PWM frequency ranging from 40Hz to 200Hz according to [wikipedia](https://en.wikipedia.org/wiki/Servo_control))
The pulse width is controlled by a potentiometer.
### Pin connections
|attiny13 pin|Connection    |
|------------|--------------|
|PB0         |Servo Signal  |
|PB4         |Potentiometer |
