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
    sum.count = this->count + other.count;

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


zone adjacent(const vector<vector<zone>>& grid, const position& cell, const char& type)
{
    zone adjacentCell;
    adjacentCell.type = type;

    for (int row = cell.row - 1; row = cell.row + 1; row++)
    {
        if (row < 0 || row > grid.size()-1) {continue;}
        
        for (int col = cell.col - 1; col = cell.col + 1; col++)
        {
            if (col < 0 || col > grid.at(row).size()-1) {continue;}

            if (row == cell.row && col == cell.col){continue;}

            if (grid.at(row).at(col).type == type)
            {
                adjacentCell = adjacentCell + grid.at(row).at(col);
            }

        }
    }

    return adjacentCell;
}


// This function will return the x,y coordinate of the value with largest adjacent population. If the value is 0 or the type does not exist, it will return negative.
position largestAdjacent(const vector<vector<zone>>& grid, const char& type)
{
    position cell, largestCell;
    int population = 0;
    zone largestAdjacent;

    for (int row = 0; row < grid.size(); row++)
    {
        cell.row = row;

        for (int col = 0; col < grid.at(row).size(); col++)
        {   
            cell.col = col;

            if (grid.at(row).at(col).type == type)
            {
                largestAdjacent = adjacent(grid, cell, type);

                if (largestAdjacent.population > population)
                {
                    largestCell = cell;
                }

            }

        }
    }

    return largestCell;


}


void updatePollution(vector<vector<zone>>& grid, const position& cell)
{   
    int pollution = grid.at(cell.row).at(cell.col).pollution;

    for (int row = cell.row - 1; row = cell.row + 1; row++)
    {
        if (row < 0 || row > grid.size()-1) {continue;}
        
        for (int col = cell.col - 1; col = cell.col + 1; col++)
        {
            if (col < 0 || col > grid.at(row).size()-1) {continue;}

            if (row == cell.row && col == cell.col){continue;}

            grid.at(row).at(col).pollution = grid.at(row).at(col).pollution + pollution -1;

            if (grid.at(row).at(col).pollution < 0) {grid.at(row).at(col).pollution = 0;}

        }
    }

    return;

}

void updateAvailableZone(vector<vector<zone>>& grid, zone cell, const char& type)
{

    for (int row = 0; row < grid.size(); row++)
    {

        for (int col = 0; col < grid.at(row).size(); col++)
        {   
            if (cell.population == 0 && cell.pollution == 0 && cell.goods == 0 && cell.job == 0) {return;}
            

            if (grid.at(row).at(col).type == type)
            {

                if (grid.at(row).at(col).population + cell.population < 0)
                {
    
                    cell.population = cell.population + grid.at(row).at(col).population;
                    grid.at(row).at(col).population = 0;
                }

                else
                {
                    grid.at(row).at(col).population = grid.at(row).at(col).population + cell.population;
                    cell.population = 0;

                }
                ///////////////////////////////////////////////////////////////


                if (grid.at(row).at(col).pollution + cell.pollution < 0)
                {
    
                    cell.pollution = cell.pollution + grid.at(row).at(col).pollution;
                    grid.at(row).at(col).pollution = 0;
                }
                
                else
                {
                    grid.at(row).at(col).pollution = grid.at(row).at(col).pollution + cell.pollution;
                    cell.pollution = 0;

                }
                ///////////////////////////////////////////////////////////////////


                if (grid.at(row).at(col).goods + cell.goods < 0)
                {
    
                    cell.goods = cell.goods + grid.at(row).at(col).goods;
                    grid.at(row).at(col).goods = 0;
                }
                
                else
                {
                    grid.at(row).at(col).goods = grid.at(row).at(col).goods + cell.goods;
                    cell.goods = 0;

                }
                ///////////////////////////////////////////////////////////////////



                if (grid.at(row).at(col).job + cell.job < 0)
                {
    
                    cell.job = cell.job + grid.at(row).at(col).job;
                    grid.at(row).at(col).job = 0;
                }
                
                else
                {
                    grid.at(row).at(col).job = grid.at(row).at(col).job + cell.job;
                    cell.job = 0;

                }
                ///////////////////////////////////////////////



            }

        }
    }

    return;

}