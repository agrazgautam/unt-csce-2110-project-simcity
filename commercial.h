#ifndef COMMERCIAL
#define COMMERCIAL

#include "simulation.h"
#include <vector>


void updateCommercial(vector<vector<zone>>& grid, bool& state); // uses the original grid to make changes through pass by reference

void updateCommercialCondition(vector<vector<zone>>& grid, const position& cell, bool& state);

#endif
