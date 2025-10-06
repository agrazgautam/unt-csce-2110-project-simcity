#include "simulation.h"
#include "commercial.h"
#include <vector>
#include <stdexcept>
#include <iostream>

/*
Commercial

    If a cell has a population of 0, is adjacent to a powerline in the current time step, there is at least 1 available worker, and there is at least one available good, that cell’s population will increase by 1 in the next time step and the available worker and available good are assigned to that job
    If a cell has a population of 0, is adjacent to at least one cell with a population of at least 1, there is at least 1 available worker, and there is at least one available good, that cell’s population will increase by 1 in the next time step and the available worker and available good are assigned to that job
    If a cell has a population of 1, is adjacent to at least two cells with a population of at least 1, there is at least 1 available worker, and there is at least one available good, that cell’s population will increase by 1 in the next time step and the available worker and available good are assigned to that job


    In the event of a decision needing to be made over two zoned cells that could grow and use available resources, the following rules must be used in order:

    Commercial zoned cells are prioritized over industrial zoned cells one
    The growth of larger population zoned cells is prioritized over smaller population zoned cells (i.e. a 1 population cell will always grow before a 0 population cell)
    The growth of zoned cells with greater total adjacent populations is prioritized over zoned cells with smaller total adjacent populations
    The growth of zoned cells with smaller Y coordinates is prioritized over zoned cells with greater Y coordinates, assuming the top left cell is 0,0
    The growth of zoned cells with smaller X coordinates is prioritized over zoned cells with greater X coordinates, assuming the top left cell is 0,0
*/

// It will loop through all the cells and changes the value if condition is met.
// If a cell is changed state will return true, otherwise false.


void updateCommercial(vector<vector<zone>>& grid, bool& state)
{   
    state = false;

    position largestCell = largest(grid, 'C');
    position largestAdjacentCell = largestAdjacent(grid, 'C');
    position cell;

    
    if (largestCell.row != -1)
    {
        updateCommercialCondition(grid, largestCell, state);

    }

    if (state == true){return;}


    if (largestAdjacentCell.row != -1)
    {
        updateCommercialCondition(grid, largestAdjacentCell, state);
    }

    if (state == true){return;}


    for (size_t row = 0; row < grid.size(); row++)
    {
        cell.row = row;

        for (size_t col = 0; col < grid.at(row).size(); col++)
        {   
            
            cell.col = col;

            if (grid.at(row).at(col).type == 'C')
            {
                updateCommercialCondition(grid, cell, state);

                if (state == true){return;}

            }

        }
    }
    
    return;


}

// Update the values according to given instruction
void updateCommercialCondition(vector<vector<zone>>& grid, const position& cell, bool& state)
{   
    zone worker = available(grid, 'R');
    zone goods = available(grid, 'I');
    zone update;

    int population = grid.at(cell.row).at(cell.col).population;

    switch (population)

    {
    case 0:
        if ((adjacent(grid, cell, 'T').count > 0 || adjacent(grid, cell, '#').count > 0) && worker.job >= 1 && goods.goods >= 1)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + goods.goods;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).count = 1;

            update.job = -worker.job;
            updateAvailableZone(grid, update, 'R');

            update.job = 0;
            update.goods = -goods.goods;
            updateAvailableZone(grid, update, 'I');

            return;

        }

        else if (adjacent(grid, cell, 'C').count >= 1 && adjacent(grid, cell, 'C').population >= 1 && worker.job >= 1 && goods.goods >= 1)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + goods.goods;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).count = 1;

            update.job = -worker.job;
            updateAvailableZone(grid, update, 'R');

            update.job = 0;
            update.goods = -goods.goods;
            updateAvailableZone(grid, update, 'I');

            return;

        }

    break;


    
    case 1:

        if (adjacent(grid, cell, 'C').count >= 2 && adjacent(grid, cell, 'C').population >= 1 && worker.job >= 1 && goods.goods >= 1)
        {
            state = true;
            grid.at(cell.row).at(cell.col).population = grid.at(cell.row).at(cell.col).population + 1;
            grid.at(cell.row).at(cell.col).goods = grid.at(cell.row).at(cell.col).goods + goods.goods;
            grid.at(cell.row).at(cell.col).job = grid.at(cell.row).at(cell.col).job + worker.job;
            grid.at(cell.row).at(cell.col).count = 1;

            update.job = -worker.job;
            updateAvailableZone(grid, update, 'R');

            update.job = 0;
            update.goods = -goods.goods;
            updateAvailableZone(grid, update, 'I');

            return;

        }

    break;
    

    default:
    break;

    }

    return;

}