#include "simulation.h"
#include "industrial.h"
#include "commercial.h"
#include "residential.h"

/*
Industrial

    If a cell has a population of 0, is adjacent to a powerline in the current time step, and there are at least 2 available workers, that cell’s population will increase by 1 in the next time step and the available workers are assigned to that job
    If a cell has a population of 0, is adjacent to at least one cell with a population of at least 1, and there are at least 2 available workers, that cell’s population will increase by 1 in the next time step and the available workers are assigned to that job
    If a cell has a population of 1, is adjacent to at least two cells with a population of at least 1, and there are at least 2 available workers, that cell’s population will increase by 1 in the next time step and the available workers are assigned to that job
    If a cell has a population of 2, is adjacent to at least four cells with a population of at least 2, and there are at least 2 available workers, that cell’s population will increase by 1 in the next time step and the available workers are assigned to that job
    A cell produces pollution equal to its population, and pollution spreads to all adjacent cells at a rate of one less unit of pollution per cell away from the source
    The industrial population provides goods to the commercial zones, at a rate of one good per population, but each good can only be sold at one commercial cell

*/

void updateIndustrial(vector<vector<zone>>& grid, bool& state)
{   
    state = false;

    position largestCell = largest(grid, 'I');
    position largestAdjacentCell = largestAdjacent(grid, 'I');
    position cell;

    
    if (largestCell.row != -1)
    {
        updateIndustrialCondition(grid, largestCell, state);

    }

    if (state == true){return;}

    if (largestAdjacentCell.row != -1)
    {
        updateIndustrialCondition(grid, largestAdjacentCell, state);
    }

    if (state == true){return;}


    for (size_t row = 0; row < grid.size(); row++)
    {
        cell.row = row;

        for (size_t col = 0; col < grid.at(row).size(); col++)
        {   
            
            cell.col = col;

            if (grid.at(row).at(col).type == 'R')
            {
                updateIndustrialCondition(grid, cell, state);

                if (state == true){return;}

            }

        }
    }
    
    return;


}


void updateIndustrialCondition(vector<vector<zone>>& grid, const position& cell, bool& state)
{   
    zone worker = available(grid, 'R');

    zone update;
    update.type = 'R';
    update.job = -worker.job;

    int population = grid.at(cell.row).at(cell.col).population;

    switch (population)

    {
    case 0:
        if (adjacent(grid, cell, 'T').count > 0 && worker.job >= 2)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).pollution = grid.at(cell.row).at(cell.col).pollution + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            updatePollution(grid, cell);

            updateAvailableZone(grid, update, 'R');

            return;

        }

        else if (adjacent(grid, cell, 'I').count >= 1 && adjacent(grid, cell, 'I').population >= 1 && worker.job >= 2)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).pollution = grid.at(cell.row).at(cell.col).pollution + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            updatePollution(grid, cell);
            
            updateAvailableZone(grid, update, 'R');

            return;

        }

    break;


    
    case 1:

        if (adjacent(grid, cell, 'I').count >= 2 && adjacent(grid, cell, 'I').population >= 1 && worker.job >= 2)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).pollution = grid.at(cell.row).at(cell.col).pollution + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            updatePollution(grid, cell);
            
            updateAvailableZone(grid, update, 'R');

            return;

        }

    break;


    case 2:

        if (adjacent(grid, cell, 'I').count >= 4 && adjacent(grid, cell, 'I').population >= 2 && worker.job >= 2)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).pollution = grid.at(cell.row).at(cell.col).pollution + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            updatePollution(grid, cell);
            
            updateAvailableZone(grid, update, 'R');

            return;

        }
    break;




    default:
    break;

    }

    return;

}