#ifndef SIMULATION
#define SIMULATION

#include <string>

using namespace std;

// This is the config class which stores the basic simulation configuration data.

class config
{
    public:

        config();
        string layout;
        int limit;
        int rate;

};



/*
This is the zone class. It will store the properties of a zone cell.
R represents a residential zone
I represents an industrial zone
C represents a commercial zone
- represents a road
T represents a powerline
# represents a powerline over a road
P represents a power plant

*/

class zone
{
    public:
        zone();
        char type;
        int population, pollution;


};





#endif