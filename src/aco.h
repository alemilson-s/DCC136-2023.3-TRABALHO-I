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
} Trip;

typedef struct {
    vector <Trip> trips;
} Tour;

typedef struct {
    Tour tour;
    double solution_value;
    double inicialTime;
} Ant;

void initializeAnts(Graph &g, vector <Ant> &ants, int n);

void initializeParameters(vector <Ant> &ants, Graph &g, float pheromone);

Edge *selectNextNode(Ant &ant, Graph &g, float alpha, float beta, int k, double maxTripTime, double tripTime, bool h1);

void aco(Graph &g, int cycles, float evaporation, float alpha, float beta);

Edge *closestHotel(Graph &g, Node *current_node);

#endif //DCC067_ACO_ACO_H
