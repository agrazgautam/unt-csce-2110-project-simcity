#include "simulation.h"
#include "commercial.h"
#include <vector>
#include <stdexcept>

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
    position cell = largest(grid, 'C');
    
    if (cell.row != -1)
    {
        zone resTotal = available(grid, 'R');
        zone indTotal = available(grid, 'I');
        if (resTotal.job >= 1 && indTotal.goods >= 1);
        {
            for (int row = cell.row - 1; row = cell.row + 1; row++)
            {
                if (row <0) {continue;}

                for (int col = cell.col - 1; col = cell.col + 1; col++)
                {
                    if (col < 0) {continue;}


                }

            }

        }
    }



}


zone adjCommercial(const vector<vector<zone>>& grid, const position& cell)
{
    zone commercial;
    commercial.type = 'C';

    if (grid.at(cell.row).at(cell.col).type != 'C'){throw invalid_argument("Only the cell type of commercial is allowed. Passed type: " + string(1,grid.at(cell.row).at(cell.col).type));}

    for (int row = cell.row - 1; row = cell.row + 1; row++)
    {
        if (row < 0) {continue;}
        
        for (int col = cell.col - 1; col = cell.col + 1; col++)
        {
            if (col < 0) {continue;}

            if (row == cell.row && col == cell.col){continue;}

            if (grid.at(row).at(col).type == 'C')
            {
                commercial = commercial + grid.at(row).at(col);
            }

        }
    }

    return commercial;

}
