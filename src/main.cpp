/* 
 * @file main.cc
 *
 * @brief Entry for musical-joycons program
 * Program Syntax: 
 *     ./build (filename) (leftJoycon Track) (rightJoycon Track)
 *	
 * Main enumerates HID device and finds the joycon devices on bluetooth. 
 * 
 * @author Sam Rossilli
 * Contact: sarossili@gmail.com
 *
 */

#include <iostream>
#include <vector>
#include <hidapi.h>
#include <joycon.hpp>
#include <unistd.h>
#include <song.hpp>
#include <thread>

#pragma warning(disable : 4996)

using namespace std;

#define JOYCON_VENDOR 0x057e
#define JOYCON_L_BT 0x2006
#define JOYCON_R_BT 0x2007
#define PRO_CONTROLLER 0x2009
#define JOYCON_CHARGING_GRIP 0x200e
#define SERIAL_LEN 18
#define PI 3.14159265359
#define L_OR_R(lr) (lr == 1 ? 'L' : (lr == 2 ? 'R' : '?'))

void playOnJC(Joycon jc, int track, string name);

std::vector<Joycon> joycons;

void playOnJC(Joycon jc, int track, string name)
{
	Song song(name);
	song.play(jc, track);
}

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		cerr << "Incorrect syntax" << endl
			 << "musicalJC (midiFile.mid) (trackNumber1) (trackNumber2)" << endl;
		exit(1);
	}

	string filename = argv[1];
	int track = atoi(argv[2]);
	int track2 = atoi(argv[3]);

	// Enumerate HID devices on the system
	struct hid_device_info *devs, *cur_dev;
	int res = hid_init();

	devs = hid_enumerate(JOYCON_VENDOR, 0x0);
	cur_dev = devs;

	// Linear search for joycons
	while (cur_dev)
	{
		// identify by vendor:
		if (cur_dev->vendor_id == JOYCON_VENDOR)
		{

			// bluetooth, left / right joycon:
			if (cur_dev->product_id == JOYCON_L_BT || cur_dev->product_id == JOYCON_R_BT)
			{
				Joycon jc = Joycon(cur_dev);
				joycons.push_back(jc);
			}

			// pro controller:
			if (cur_dev->product_id == PRO_CONTROLLER)
			{
				Joycon jc = Joycon(cur_dev);
				joycons.push_back(jc);
			}
		}

		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	// init joycons:
	for (int i = 0; i < joycons.size(); ++i)
	{
		joycons[i].init_bt();
	}

	if (joycons.size() == 2)
	{
		// Create Threads
		// Badly done concurency but it works well enough
		vector<std::thread> threads;
		threads.push_back(thread(playOnJC, ref(joycons[0]), track, ref(filename)));

		threads.push_back(thread(playOnJC, ref(joycons[1]), track2, ref(filename)));

		for (auto &thread : threads)
		{
			thread.join();
		}
	}
	else if (joycons.size() == 1)
	{
		playOnJC(joycons[0], track, filename);
	}
	else
	{
		cerr << "Joycons not found. Try Re-pairing the joycons" << endl;
	}
	return 0;
}
