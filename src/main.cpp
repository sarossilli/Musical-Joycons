#include <iostream>
#include <vector>
#include <hidapi.h>
#include <joycon.hpp>
#include <unistd.h>
#include <song.hpp>


#pragma warning(disable:4996)

using namespace std;


#define JOYCON_VENDOR 0x057e
#define JOYCON_L_BT 0x2006
#define JOYCON_R_BT 0x2007
#define PRO_CONTROLLER 0x2009
#define JOYCON_CHARGING_GRIP 0x200e
#define SERIAL_LEN 18
#define PI 3.14159265359
#define L_OR_R(lr) (lr == 1 ? 'L' : (lr == 2 ? 'R' : '?'))


//Helper Functions
inline int mk_odd(int);
void nsleep(useconds_t);
void play(Joycon,string,int);


std::vector<Joycon> joycons;

int res = 0;

int main(int argc, char** argv){

	string filename = argv[1];
	int track = atoi(argv[2]);

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
	
	Song midiTest(filename);
	midiTest.play(joycons[0],track);
	printf("Done.\n");

}

void nsleep(useconds_t n){
	usleep(n*1000);
}



   
