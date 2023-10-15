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
    double tripTime;
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

Edge *selectNextNode(Ant &ant, Graph &g, float alpha, float beta, int k, double maxTripTime, double tripTime, bool h1, bool penultimate);

void aco(Graph &g, int cycles, float evaporation, float alpha, float beta);

Edge *closestHotel(Graph &g, Node *current_node, bool penultimate);

Ant firstLocalSearch(Graph &g, Ant &ant);

Ant secondLocalSearch(Graph &g, Ant &ant);

bool tryInsertNodeIntoTrip(int id_node, int index_node, int index_trip, Trip &trip, Graph &g);

Ant vnd(Graph &g, Ant &ant);

bool isValid(Graph &g, Ant ant);

#endif //DCC067_ACO_ACO_H
