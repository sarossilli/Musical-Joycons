#include "MidiFile.h"
#include "Binasc.h"
#include <fstream>
#include <string>
#include <unistd.h>
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
    void nsleep(double);
    int tpq;
    int qps = 1;

public:
    Song(string);
    int print(int);
    int play(Joycon, int);
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

int Song::play(Joycon jc, int track)
{

    double note;
    double curTime = 0;

    //Fit MIDI NOTES Between 130-255
    int *ptr;
    int minRumble = 125;
    int maxRumble = 255;

    ptr = getRange(track);

    double diff = (maxRumble - minRumble) / (72 - 48);
    int shift =  (ptr[0]+ ((ptr[1] - ptr[0]) / 2)) - ((72 - 48)/2); 
    cout << "MID" << shift << endl;
    double a = 440.0; // a is 440 hz...

    for (int i = 0; i < file[track].size(); i++)
    {
        if (file[track][i].isNote())
        {
            double time = file[track][i].seconds;
            int diff = ((time - curTime) * 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(diff));
            curTime = time;
        }

        if (file[track][i].isNoteOn())
        {
            int note = (file[track][i].getKeyNumber()) + shift; //Temporary frequency estimation
            note = diff * (note - 48) + minRumble;
            if (note > maxRumble | note < minRumble)
            {
                note = (note % maxRumble) + minRumble;
            }

            cout << note << endl;
            jc.rumble(mk_odd(note), 1);
        }
        if (file[track][i].isNoteOff())
        {
            jc.rumble(100, 3);
        }
    }
    return 0;
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
        cerr << "Not enough Tracks" << endl;
        exit(1);
    }

    file.linkNotePairs();
    file.doTimeAnalysis();
}

void Song::nsleep(double n)
{

    usleep(n * 1000);
}

inline int Song::mk_odd(int n)
{
    return n - (n % 2 ? 0 : 1);
}