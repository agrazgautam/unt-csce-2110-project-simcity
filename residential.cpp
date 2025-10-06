#include "residential.h"
#include "simulation.h"
#include "commercial.h"

#include <vector>
#include <stdexcept>


/*
Residential

    If a cell has a population of 0 and is adjacent to a powerline in the current time step, that cell’s population will increase by 1 in the next time step
    If a cell has a population of 0 and is adjacent to at least one cell with a population of at least 1, that cell’s population will increase by 1 in the next time step
    If a cell has a population of 1 and is adjacent to at least two cells with a population of at least 1, that cell’s population will increase by 1 in the next time step
    If a cell has a population of 2 and is adjacent to at least four cells with a population of at least 2, that cell’s population will increase by 1 in the next time step
    If a cell has a population of 3 and is adjacent to at least six cells with a population of at least 3, that cell’s population will increase by 1 in the next time step
    If a cell has a population of 4 and is adjacent to at least eight cells with a population of at least 4, that cell’s population will increase by 1 in the next time step
    The residential population provides workers to the industrial and commercial zones, but each worker can only have one job


    In the event of a decision needing to be made over two zoned cells that could grow and use available resources, the following rules must be used in order:

    Commercial zoned cells are prioritized over industrial zoned cells one
    The growth of larger population zoned cells is prioritized over smaller population zoned cells (i.e. a 1 population cell will always grow before a 0 population cell)
    The growth of zoned cells with greater total adjacent populations is prioritized over zoned cells with smaller total adjacent populations
    The growth of zoned cells with smaller Y coordinates is prioritized over zoned cells with greater Y coordinates, assuming the top left cell is 0,0
    The growth of zoned cells with smaller X coordinates is prioritized over zoned cells with greater X coordinates, assuming the top left cell is 0,0

*/

void updateResidential(vector<vector<zone>>& grid, bool& state)
{   
    state = false;

    position largestCell = largest(grid, 'R');
    position largestAdjacentCell = largestAdjacent(grid, 'R');
    position cell;

    
    if (largestCell.row != -1)
    {
        updateResidentialCondition(grid, largestCell, state);

    }

    if (state == true){return;}

    if (largestAdjacentCell.row != -1)
    {
        updateResidentialCondition(grid, largestAdjacentCell, state);
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
                updateResidentialCondition(grid, cell, state);

                if (state == true){return;}

            }

        }
    }
    
    return;


}


void updateResidentialCondition(vector<vector<zone>>& grid, const position& cell, bool& state)
{   

    int population = grid.at(cell.row).at(cell.col).population;

    switch (population)

    {
    case 0:
        if (adjacent(grid, cell, 'T').count > 0)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            return;

        }

        else if (adjacent(grid, cell, 'R').count >= 1 && adjacent(grid, cell, 'R').population >= 1)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            return;

        }

    break;


    
    case 1:

        if (adjacent(grid, cell, 'R').count >= 2 && adjacent(grid, cell, 'R').population >= 1)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
            grid.at(cell.row).at(cell.col).count = 1;

            return;

        }

    break;


    case 2:

        if (adjacent(grid, cell, 'R').count >= 4 && adjacent(grid, cell, 'R').population >= 2)
            {
                state = true;
                grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
                grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
                grid.at(cell.row).at(cell.col).count = 1;

                return;

            }
    break;


    
    case 3:
        if (adjacent(grid, cell, 'R').count >= 6 && adjacent(grid, cell, 'R').population >= 3)
            {
                state = true;
                grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
                grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
                grid.at(cell.row).at(cell.col).count = 1;

                return;

            }
    break;

    
    case 4:
        if (adjacent(grid, cell, 'R').count >= 8 && adjacent(grid, cell, 'R').population >= 4)
            {
                state = true;
                grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
                grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + 1;
                grid.at(cell.row).at(cell.col).count = 1;

                return;

            }
    break;



    default:
    break;

    }

    return;

}