# RobotArm
Project of Robotic arm with three rotating axes.
This arm uses Rpi pico as controller, that controlls motors via PWM signal sent do motor drivers.

![Screenshot_1](https://github.com/MarcinD12/RobotArm/assets/111440372/e1f49609-db71-4c86-9758-cf678b7bb0d0)

## Hardware:
- Raspberry Pi Pico
- 2x Cytron Maker Drive MX1508
- 3x Old car seat motor
- Step-Down Converter XL4015
- 3x AS5600 to be mounted

## How it works:
Very simple steering by reading which buttons are pressed and basing on that PWM signal is sent to motor driver. The longer button is pressed then bigger is that channel duty. There is very simple algorithm that prevents from very hard breaking rotation by decrementing signal to 0.

## To do:
- Redesign main axis mount (now is just put on bearing so it is very unstable)
- Add encoders
- Add attachment-probably gripper


