#include <iostream>
#include <vector>
#include <hidapi.h>
#include <joycon.hpp>
#include <unistd.h>

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

inline int mk_odd(int);
void nsleep(useconds_t);

std::vector<Joycon> joycons;

unsigned char buf[65];
int res = 0;

int main(){
    int read;	// number of bytes read
	int written;// number of bytes written
	const char *device_name;

	// Enumerate and print the HID devices on the system
	struct hid_device_info *devs, *cur_dev;

	res = hid_init();

init_start:

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
			//buf[0] = 0x80 | 0x40 | 0x2 | 0x1; // Flash top two, solid bottom two
			//buf[0] = 0x8 | 0x4 | 0x2 | 0x1; // All solid
			//buf[0] = 0x80 | 0x40 | 0x20 | 0x10; // All flashing
			//buf[0] = 0x80 | 0x00 | 0x20 | 0x10; // All flashing except 3rd light (off)
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


	// notes arbitrarily defined:
	#define C3 110
	#define D3 120
	#define E3 130
	#define F3 140
	#define G3 150
	#define G3A4 155
	#define A4 160
	#define A4B4 165
	#define B4 170
	#define C4 180
	#define D4 190
	#define D4E4 195
	#define E4 200
	#define F4 210
	#define F4G4 215
	#define G4 220
	#define A5 230
	#define B5 240
	#define C5 250



	if (true) {
		for (int i = 0; i < 1; ++i) {

			printf("Playing mario theme...\n");

			float spd = 1;
			float spd2 = 1;

			//goto N1;

			Joycon joycon = joycons[0];

			nsleep(1000);

			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(400 / spd2);

			joycon.rumble(mk_odd(A4), 1); nsleep(400 / spd); joycon.rumble(1, 3);	// too low for joycon
			nsleep(50 / spd2);

			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(G3), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G1
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E3), 2); nsleep(200 / spd); joycon.rumble(1, 3);	// E1
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2

			nsleep(100 / spd2);
			joycon.rumble(mk_odd(B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// B2

			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// A2-B2?
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(G3), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G1


			nsleep(100 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(A5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A3



			nsleep(200 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2

			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2

			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// B2


			nsleep(200 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(G3), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G1
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E3), 2); nsleep(200 / spd); joycon.rumble(1, 3);	// E1

			nsleep(200 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// B2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(A4B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// A2-B2?
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2


			nsleep(100 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(A5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A3
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2


			nsleep(200 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(B4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// B2

																					// new:

			nsleep(500 / spd2);

			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// F2-G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2-E2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2

			nsleep(200 / spd2);

			joycon.rumble(mk_odd(G3A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// G1-A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2


			nsleep(200 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2


			nsleep(300 / spd2);

			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// F2-G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2-E2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2


																					// three notes:
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(C5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(C3), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C3), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3


		N1:


			nsleep(500 / spd2);
			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// F2G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2

			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2E2

			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2



			nsleep(200 / spd2);
			joycon.rumble(mk_odd(G3A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// G1A2

			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2


			nsleep(300 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2E2
			nsleep(300 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2
			nsleep(300 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2


			nsleep(800 / spd2);


			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// F2G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2E2
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2

			nsleep(200 / spd2);


			joycon.rumble(mk_odd(G3A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// G1A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2

			nsleep(200 / spd2);

			joycon.rumble(mk_odd(A4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// A2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// D2

			nsleep(300 / spd2);


			joycon.rumble(mk_odd(G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4G4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// F2G2
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(F4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// F2


			nsleep(50 / spd2);
			joycon.rumble(mk_odd(D4E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);// D2E2
			nsleep(100 / spd2);
			joycon.rumble(mk_odd(E4), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// E2

																					// 30 second mark

																					// three notes:

			nsleep(300 / spd2);
			joycon.rumble(mk_odd(C5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3
			nsleep(200 / spd2);
			joycon.rumble(mk_odd(C5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3
			nsleep(50 / spd2);
			joycon.rumble(mk_odd(C5), 1); nsleep(200 / spd); joycon.rumble(1, 3);	// C3


			nsleep(1000);
		}
	}

	printf("Done.\n");
}

void nsleep(useconds_t n){
	usleep(n*1000);
}
inline int mk_odd(int n) {
	return n - (n % 2 ? 0 : 1);
}