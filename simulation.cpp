#include "simulation.h"
#include <vector>
#include <stdexcept>
#include <string>

using namespace std;


config::config()
{
    layout = "";
    limit = -1;
    rate = -1;
    timestep = -1;

};


zone::zone()
{
    type = 'n';
    population = 0;
    pollution = 0;
    goods = 0;
    job = 0;
    count = 0;

};

zone zone::operator+ (const zone& other)
{
    zone sum;
    if (this->type != other.type)
    {
        throw invalid_argument("The type must match for adding.");
    }

    sum.population = this->population + other.population;
    sum.pollution = this->pollution + other.pollution;
    sum.goods = this->goods + other.goods;
    sum.job = this->job + other.job;

    return sum;
}


position::position()
{
    row = -1;
    col = -1;    
};



// This function will return the x,y coordinate of the largest value. If the value is 0 or the type does not exist, it will return negative.
position largest(const vector<vector<zone>>& grid, const char& type)
{
    position cell;
    int largest = 0;

    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid.at(row).size(); col++)
        {
            if ( grid.at(row).at(col).type == type )
            {
                if (grid.at(row).at(col).population > 0 && grid.at(row).at(col).population > largest)
                {
                    cell.row = row;
                    cell.col = col;
                    largest = grid.at(row).at(col).population;
                }
            }
        }
    }

    return cell;

}

zone available(const vector<vector<zone>>& grid, const char& type)
{
    zone total;
    total.type = type;
    for (vector<vector<zone>>::const_iterator row = grid.begin(); row != grid.end(); row++)
    {
        for (vector<zone>::const_iterator cell = row->begin(); cell != row->end(); cell++)
        { 
            if (cell->type == type)
            {
                total = total + *cell;

            }

        }
    }

    return total;
}


