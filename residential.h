#ifndef RESIDENTIAL
#define RESIDENTIAL

#include "simulation.h"
#include "commercial.h"

void updateResidential(vector<vector<zone>>& grid, bool& state); // uses the original grid to make changes through pass by reference

void updateResidentialCondition(vector<vector<zone>>& grid, const position& cell, bool& state);

#endif