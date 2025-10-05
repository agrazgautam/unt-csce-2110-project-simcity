#ifndef INDUSTRIAL
#define INDUSTRIAL

#include "simulation.h"
#include "commercial.h"
#include "residential.h"

void updateIndustrial(vector<vector<zone>>& grid, bool& state); // uses the original grid to make changes through pass by reference

void updateIndustrialCondition(vector<vector<zone>>& grid, const position& cell, bool& state);


#endif