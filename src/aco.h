#ifndef DCC067_ACO_ACO_H
#define DCC067_ACO_ACO_H

#include "Graph.h"
#include <queue>
#include <limits>
#include <valarray>
#include <iostream>

typedef struct {
    vector<int> path;
    vector<bool> visited;
    double solution_value;
} Ant;

void initializeAnts(Graph &g, vector <Ant> &ants, int n);

void initializeParameters(vector <Ant> &ants, Graph &g, float pheromone);

Edge *selectNextCity(Ant &ant, Graph &g, float alpha, float beta);

void aco(Graph &g, int cycles, float evaporation, float alpha, float beta);

#endif //DCC067_ACO_ACO_H
