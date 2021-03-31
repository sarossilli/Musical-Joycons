[<img src="https://raw.githubusercontent.com/Sarossilli/Musical-Joycons/master/readme/header.gif" alt="HEADER">]() 

### Play Music with your Joycon Controller

## Project Overview:
- Created a program that allows nintendo switch joycons to play a midifile through vibrations
- Utilized HID API hidraw library to interface with joycons
- Built a Song class to interface between music files and joycon vibration output

## How to Use:
1. Download the release
2. Extract exe and dll into the same folder
3. Pair Joycons
4. run musical-joycons.exe
5. Put Midi File into the same directory as the exe
6. Input midiFile name, track1 and track2 when prompted

## How to Develop:
This is specifically made for linux. I will add a windows version later (hopefully)
1. Clone the repository 
`git clone https://github.com/sarossilli/Musical-Joycons.git`

2. Compile the HIDAPI for your system and add lib files to libs folder

Instructions here: `https://github.com/libusb/hidapi`

3. use the makefile to Compile:
use command `make` in terminal

4. Connect your Joycons through bluetooth

5. Run the program:
use command `./bin/build (midifile).mid track1 track2`

## Code and Resources Used 
**Language Used:** C++

**Libraries:** 

HIDAPI, using the Linux/hidraw backend. Github Repo Avaliable [Here](https://github.com/signal11/hidapi)
MIDI file parsing library Github Repo Avaliable [Here] https://github.com/craigsapp/midifile
**Resources Used:**

Nintendo Switch Reverse Engineering. Github Repo Avaliable [Here](https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/)

JoyCon-Driver. Github Repo Avaliable [Here](https://github.com/fossephate/JoyCon-Driver)

