[<img src="https://raw.githubusercontent.com/Sarossilli/Musical-Joycons/master/readme/header.gif" alt="HEADER">]() 

### Play Music with your Joycon Controller

## Project Overview:
- Created a program that allows nintendo switch joycons to play music through vibrations
- Utilized HID API hidraw library to interface with joycons
- Built a MIDI interpreter to interface between music files and joycon vibration output

## How to Use:
This is specifically made for linux. I will add a windows version later
1. Clone the repository 
In a terminal window: 
`git clone https://github.com/sarossilli/Musical-Joycons.git`

2. use the makefile to complile:
use command `make` in terminal

3. Connect your joycons through bluetooth

4. Run the program:
use command `sudo ./a.out`

If there is a segmentation error, make sure that your joycons are connected to your computer through bluetooth
## Code and Resources Used 
**Language Used:** C++

**Libraries:** 

HIDAPI, using the Linux/hidraw backend. Github Repo Avaliable [Here](https://github.com/signal11/hidapi)

**Resources:**

Nintendo Switch Reverse Engineering. Github Repo Avaliable [Here](https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/)

JoyCon-Driver. Github Repo Avaliable [Here](https://github.com/fossephate/JoyCon-Driver)

