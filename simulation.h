#ifndef SIMULATION
#define SIMULATION

#include <string>
#include <vector>

using namespace std;

// This is the config class which stores the basic simulation configuration data.

class config
{
    public:

        config();
        string layout;
        int limit;
        int rate;
        int timestep;

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

The zone class has its type, population, pollution, and goods.

*/

class zone
{
    public:
        zone();
        char type;
        int population, pollution;
        int goods, job;
        int count;
        
        /*
        The industrial population provides goods to the commercial zones, at a rate of one good per population
        Only industrial type produces pollution. But it spreades across.
        */

    zone operator+ (const zone& other);
};



struct position
{   
    position();
    int row, col;
};


position largest(const vector<vector<zone>>& grid, const char& type); // It will return the position of the first largest value.


zone available(const vector<vector<zone>>& grid, const char& type);


zone adjacent(const vector<vector<zone>>& grid, const position& cell, const char& type);


position largestAdjacent(const vector<vector<zone>>& grid, const char& type); // Returns the position of the largest adjacent total population

#endif