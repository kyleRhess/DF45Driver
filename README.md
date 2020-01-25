# DF45 Motor Driver

### A low-level Arduino sketch for controlling the [Nanotec DF45](https://us.nanotec.com/products/1544-df45-brushless-dc-motor/) BLDC (Brushless DC) motor.
\
\
This code was written for the Arduino Uno or any ATMega328.
\
The [motor driver module itself](https://kylerhess.github.io/driver.html) uses an embedded `ATMEGA328-AUR` and is proprietary to the controller PCB design.
\
The DF45 Driver relies on brushless motors with Hall-Effect sensor feedback in order to function properly. These motors operate under `Four Quadrant Motor Control`.

![](https://kylerhess.github.io/images/driver/4.JPG)
