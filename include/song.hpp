/* 
 * @file song.hpp
 *
 * @brief Song header
 * Represents a playable midi file 
 * construct using Song(filename)
 * use play(joycon, track) to play song on a joycon 
 *	
 * 
 * @author Sam Rossilli
 * Contact: sarossili@gmail.com
 *
 */

#include "MidiFile.h"
#include "Binasc.h"
#include <fstream>
#include <string>
//#include <unistd.h>
#include <math.h> /* pow */

#include <thread>
#include <chrono>

using namespace std;
using namespace smf;

class Song
{
private:
    MidiFile file;
    int parts;
    MidiEventList t1;
    MidiEventList t2;
    double diff;
    double shift;
    int minRumble = 120;
    int maxRumble = 250;

public:
    Song(string);
    int print(int);
    void play(Joycon, int);
    int mk_odd(int);
    int *getRange(int);
};

int Song::print(int t)
{
    int track = t;
    double curTime = 0;
    for (int event = 0; event < file[track].size(); event++)
    {
        if (file[track][event].isNoteOn())
        {
            int key = file[track][event].getKeyNumber();
            double time = file[track][event].seconds;
            double duration = time - curTime;
            curTime = time;
            cout << event << "\t" << key << "\t" << time << "\t" << duration << endl;
        }
    }
    return 0;
}

// Gets max/min notes for a track
// Used to fit track within bounds of joycon rumble
int *Song::getRange(int track)
{

    static int minmax[2];
    MidiEventList tr = file[track];

    minmax[0] = 500; //MIN
    minmax[1] = 0;
    for (int i = 0; i < tr.size() - 1; i++)
    {
        if (tr[i].isNoteOn())
        {
            int midiNote = tr[i].getKeyNumber();
            if (midiNote < minmax[0])
            {
                minmax[0] = midiNote;
            }
            if (midiNote > minmax[1])
            {
                minmax[1] = midiNote;
            }
        }
    }
    return minmax;
}

void Song::play(Joycon jc, int track)
{

    if (track > file.getTrackCount())
    {
        cerr << "Requested track does not exist. Check that you put the correct midi track number" << endl;
        return;
    }

    cout << "Playing tack " << track << endl;

    double note;
    double curTime = 0;

    // move through each event
    for (int i = 0; i < file[track].size(); i++)
    {
        //Calculate note/sleeptime
        if (file[track][i].isNote())
        {
            double time = file[track][i].seconds;
            int diff = ((time - curTime) * 1000);
            this_thread::sleep_for(chrono::milliseconds(diff));
            curTime = time;
        }

        //play noteOn
        if (file[track][i].isNoteOn())
        {
            int note = (file[track][i].getKeyNumber()) + shift; // calculate midi note to joycon rumble value
            note = diff * (note - 60) + minRumble;
            if (note > maxRumble | note < minRumble) //Handle overflow of rumble value
            {
                note = (note % maxRumble) + minRumble;
            }
            jc.rumble(mk_odd(note), 1);
        }
        //Turn note off
        if (file[track][i].isNoteOff())
        {
            jc.rumble(100, 3);
        }
    }
    return;
}

Song::Song(string fileName)
{



    file.read(fileName);

    if (!file.status())
    {
        cerr << "Error reading MIDI file " << endl;
        exit(1);
    }
    if (file.getTrackCount() < 2)
    {
        cerr << "Not enough Tracks for two joycons" << endl;
    }

    double min;
    double max;
    int *ptr;
    for (int i = 0; i < file.getTrackCount(); i++)
    {
        ptr = getRange(i);
        if (ptr[0] < min)
        {
            min = ptr[0];
        }
        if (ptr[1] > max)
        {
            max = ptr[1];
        }
    }
    diff = 5;

    //NEED TO IMPLEMENT BETTER NOTE TO RUMBLE
    //diff = (maxRumble - minRumble) / (84 - 60);
    //shift = (ptr[0] + ((ptr[1] - ptr[0]) / 2)) - ((84 - 60) / 2);

    file.linkNotePairs();
    file.doTimeAnalysis();
}

// Notes have to be odd for some reason
inline int Song::mk_odd(int n)
{
    return n - (n % 2 ? 0 : 1);
}