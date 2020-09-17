#include <iostream>
#include <vector>
#include <hidapi.h>
#include <joycon.hpp>
#include <unistd.h>

#include "MidiFile.h"
#include "Binasc.h"
#include <fstream>
#include <string>


#pragma warning(disable:4996)

using namespace std;
using namespace smf;

#define JOYCON_VENDOR 0x057e
#define JOYCON_L_BT 0x2006
#define JOYCON_R_BT 0x2007
#define PRO_CONTROLLER 0x2009
#define JOYCON_CHARGING_GRIP 0x200e
#define SERIAL_LEN 18
#define PI 3.14159265359
#define L_OR_R(lr) (lr == 1 ? 'L' : (lr == 2 ? 'R' : '?'))

inline int mk_odd(int);
void nsleep(useconds_t);
void play(Joycon,string,int);

std::vector<Joycon> joycons;

unsigned char buf[65];
int res = 0;

int main(int argc, char** argv){

	// Enumerate and print the HID devices on the system
	struct hid_device_info *devs, *cur_dev;

	res = hid_init();

	devs = hid_enumerate(JOYCON_VENDOR, 0x0);
	cur_dev = devs;
	while (cur_dev) {

		// identify by vendor:
		if (cur_dev->vendor_id == JOYCON_VENDOR) {

			// bluetooth, left / right joycon:
			if (cur_dev->product_id == JOYCON_L_BT || cur_dev->product_id == JOYCON_R_BT) {
				Joycon jc = Joycon(cur_dev);
				joycons.push_back(jc);
			}

			// pro controller:
			if (cur_dev->product_id == PRO_CONTROLLER) {
				Joycon jc = Joycon(cur_dev);
				joycons.push_back(jc);
			}

		}


		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);



	// init joycons:
    for (int i = 0; i < joycons.size(); ++i) {
		joycons[i].init_bt();
    }

	// set lights:
	printf("setting LEDs...\n");
	for (int r = 0; r < 5; ++r) {
		for (int i = 0; i < joycons.size(); ++i) {
			Joycon *jc = &joycons[i];
			// Player LED Enable
			memset(buf, 0x00, 0x40);
			if (i == 0) {
				buf[0] = 0x0 | 0x0 | 0x0 | 0x1;		// solid 1
			}
			if (i == 1) {
				if (/*settings.combineJoyCons*/true) {
					buf[0] = 0x0 | 0x0 | 0x0 | 0x1; // solid 1
				} else if (/*!settings.combineJoyCons*/false) {
					buf[0] = 0x0 | 0x0 | 0x2 | 0x0; // solid 2
				}
			}
			jc->send_subcommand(0x01, 0x30, buf, 1);
		}
	}


	// give a small rumble to all joycons:
	printf("vibrating JoyCon(s).\n");
	for (int k = 0; k < 1; ++k) {
		for (int i = 0; i < joycons.size(); ++i) {
			joycons[i].rumble(100, 1);
			nsleep(300);
			joycons[i].rumble(100, 3);
		}
	}
	nsleep(1000);
	play(joycons[0],argv[1],atoi(argv[2]));
	printf("Done.\n");

}

void nsleep(useconds_t n){
	usleep(n*1000);
}
inline int mk_odd(int n) {
	return n - (n % 2 ? 0 : 1);
}

void play(Joycon joycon,string fileName, int track){
	MidiFile midifile;
	int note;

	midifile.read(fileName);
	if (!midifile.status()) {
		cerr << "Error reading MIDI file " << endl;
		exit(1);

	}
	midifile.doTimeAnalysis();
	midifile.linkNotePairs();

	cout << "PYLAYING TRACK: " << track << endl;
	for (int i=0; i<midifile[track].size(); i++) {
		if (!midifile[track][i].isNoteOn()) {
			continue;
		}
		note = (5.7* midifile[track][i].getKeyNumber()) - 164.40; //Temporary frequency estimation
		if(note >230){ note -=80;}
		joycon.rumble(mk_odd(note), 1);

		nsleep(midifile[track][i].getDurationInSeconds()*1000);

		joycon.rumble(1, 3);

   }
}

   
