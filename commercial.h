#ifndef COMMERCIAL
#define COMMERCIAL

#include "simulation.h"
#include <vector>


zone adjCommercial(const vector<vector<zone>>& grid, const position& cell);

void updateCommercial(vector<vector<zone>>& grid, bool& state); // uses the original grid to make changes through pass by reference


#endif
