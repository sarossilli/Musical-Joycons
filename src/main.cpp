/* 
 * @file main.cc
 *
 * @brief Entry for musical-joycons program
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
//#include <unistd.h>
#include <song.hpp>
#include <thread>
#include <string>



#pragma warning(disable : 4996)

using namespace std;

#define JOYCON_VENDOR 0x057e
#define JOYCON_L_BT 0x2006
#define JOYCON_R_BT 0x2007
#define PRO_CONTROLLER 0x2009
#define JOYCON_CHARGING_GRIP 0x200e
#define SERIAL_LEN 18
#define L_OR_R(lr) (lr == 1 ? 'L' : (lr == 2 ? 'R' : '?'))

void playOnJC(Joycon jc, int track, string name);
inline bool exists_test(const std::string& name);

std::vector<Joycon> joycons;

void playOnJC(Joycon jc, int track, string name)
{
	MidiFile file;
	file.read(name);
	file.linkNotePairs();
	file.doTimeAnalysis();
	jc.play(file, track);
}

int main(int argc, char **argv)
{



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
			Joycon jc = Joycon(cur_dev);
			joycons.push_back(jc);
		
		}

		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	if(joycons.empty()){
		cout << "Warning: Joycons not found. Make sure they are paired" << endl;
		cin.get();
		exit(1);
	}
	// init joycons:
	for (int i = 0; i < joycons.size(); ++i)
	{
		joycons[i].init_bt();
	}


	string filename;
	int track;
	int track2;

	cout << "Enter Midi : " << endl;

	getline(cin, filename);
	while (!exists_test(filename)) {
		cout << "Midi Not Found" << endl;
		cout << "Enter Midi : " << endl;
		getline(cin, filename);
	}



	Song song(filename);
	int len = song.getTrackCount();
	cout << "This Midi File Has " << len << " Tracks" << endl;


	string input;
	cout << "Enter Track Number to play on Right Joycon" << endl;

	getline(cin, input);
	track = stoi(input);
	cout << track;
	while (track > len | track < 1) {
		cout << "Track Out of Range" << endl;
		cout << "Enter Track Number to play on Right Joycon" << endl;
		getline(cin, input);
		track = stoi(input);
	}
	cout << "Enter Track Number to play on Right Joycon" << endl;

	getline(cin, input);
	track2 = stoi(input);

	while (track2 > len | track2<1) {
		cout << "Track Out of Range" << endl;
		cout << "Enter Track Number to play on Right Joycon" << endl;
		getline(cin, input);
		track2 = stoi(input);
	}

	track--;
	track2--;


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
	else{
		playOnJC(joycons[0], track, filename);
	}
	return 0;
}


inline bool exists_test(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}
