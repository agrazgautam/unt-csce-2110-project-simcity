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



#endif