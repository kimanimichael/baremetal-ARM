# BAREMETAL ARM
This is an exploration building of embedded systems on arm microcontrollers without using any framework

It covers:

<li>Blocking Design</li>
<li>State Machine Deign</li>
<li>RTOS design</li>
<li>Event Driven Systems Design </li>

## Hardware Requirements

1. [NUCLEO-F429ZI board](https://www.st.com/en/evaluation-tools/nucleo-f429zi.html#samplebuy-scroll)
2. USB-X to USB 2.0 Micro B cable


Although this project is built for the NUCLEO-F429ZI development board, users can modify it, if they can, to fit other development boards


## Installation
### Prerequisites
The following are required to build this program:
1. [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)
2. [Python](https://www.python.org/downloads/)

The following are required to flash this program onto a microcontroller
1. [stlink](https://github.com/stlink-org/stlink)

## Build
Ensure all prerequisites are met.

Run the following commands:
```console
pip install -r requirements.txt
```
```console
python3 build.py build-application --compile
```
## Flash

Connect the board to your PC and run this in the terminal
```console
python3 build.py build-application --flash
```
# Compile and Flash
```console
python3 build.py build-application --compile --flash
```
