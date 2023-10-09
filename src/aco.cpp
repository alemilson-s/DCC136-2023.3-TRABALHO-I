#include "Graph.h"
#include <queue>
#include <limits>
#include <valarray>
#include <iostream>
#include "aco.h"
#include <random>
#include <cmath>

using namespace std;

void aco(Graph &g, int cycles, float evaporation, float alpha, float beta) {
    cout << "Construindo solução ACO..." << endl;
    Ant best;
    int n_ants = g.getOrder() * 1 / 6;
    best.solution_value = 0;
    vector<Ant> ants(n_ants, Ant());
    initializeParameters(ants, g, 10);
    int t = 0;
    bool h1 = false;

    double maxTripTime;
    double tripTime;
    while (t < cycles) {
        initializeAnts(g, ants, g.getOrder()); 
        int j = 0;
        while (j < n_ants) {
            int k = 0;
            tripTime = ants[j].inicialTime;
            while (k < g.getD()) {
                maxTripTime = g.getTD()[k];
                if (k == g.getD() - 1)
                    h1 = true;
                while (tripTime <= maxTripTime) {
                    Edge *next_node = selectNextNode(ants[j], g, alpha, beta, k, maxTripTime, tripTime, h1);
                    Node *node = g.getNode(next_node->getTargetId());
                    ants[j].tour.trips[k].path.push_back(node->getObjectId());
                    ants[j].tour.trips[k].visited[node->getObjectId()] = true;
                    ants[j].solution_value += node->getWeight();
                    tripTime += next_node->getWeight();
//                    tratar para tripTime não ultrapassar maxTripTime
                    if (node->getType() == 'E')
                        break;
                }
                tripTime = 0;
                k++;
            }
            if (ants[j].solution_value > best.solution_value)
                best = ants[j];
            cout << "Solução: " << best.solution_value << endl;
            cout << "Solução: " << ants[j].solution_value << endl;
            j++;
        }

        // nesse ponto todas as formigas já calcularam sua própria solução
        // agora começa a parte de alteração nos níveis de feromônios das soluções
        // os feromônios são atualizados de acordo com a qualidade da solução
        // para cada formiga, irá percorrer o tour e cada uma das trips, 
        // passa pelo path da trip e atualiza a taxa de feromônio de cada aresta, segundo a fórmula
        for (j = 0; j < n_ants; j++) {
            for (int i = 0; i < ants[j].tour.trips.size(); i++) {
                for (int k = 0; k < ants[j].tour.trips[i].path.size() - 1; k++) {
                    Node *node = g.getNode(ants[j].tour.trips[i].path[k]);
                    Edge *edge = node->getEdge(ants[j].tour.trips[i].path[k + 1]);
                    double pheromone = (1 - evaporation) * edge->getPheromone() +
                                    evaporation * ((g.getOrder() * ants[j].solution_value));
                    edge->setPheromone(pheromone);
                    if (!g.getDirected()) {
                        node = g.getNode(ants[j].tour.trips[i].path[k + 1]);
                        edge = node->getEdge(ants[j].tour.trips[i].path[k]);
                        edge->setPheromone(pheromone);
                    }
                }
            }
        }
        // percorre o path da melhor solução e atualiza o feromônio dela com um valor maior do que das outras rotas
        for(int k = 0; k < best.tour.trips.size(); k++){
            for (int i = 0; i < best.tour.trips[k].path.size() - 1; i++) {
                Node *node = g.getNode(best.tour.trips[k].path[i]);
                Edge *edge = node->getEdge(best.tour.trips[k].path[i + 1]);
                double pheromone = (1 - evaporation) * edge->getPheromone() + evaporation * (best.solution_value);
                edge->setPheromone(pheromone);
            }
        }
//        cout << "próxima geração" << endl;
        t++;
    }
    // imprime a solução
    cout << "Solução:" << endl;
    for (int k = 0; k < best.tour.trips.size(); k++) {
        for (int i = 0; i < best.tour.trips[k].path.size() - 1; i++) {
            cout << '\t' << '(' << best.tour.trips[k].path[i] << ", " << best.tour.trips[k].path[i + 1] << ')' << endl;
        }
    }
    cout << "Valor da solução: " << best.solution_value << endl;
}

void initializeAnts(Graph &g, vector<Ant> &ants, int n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(g.getH() + 2, n - 1);
    for (int i = 0; i < ants.size(); i++) {
        ants[i].tour.trips.clear();
        Trip d;
        ants[i].tour.trips.resize(g.getD(), d);
        for (int j = 0; j < g.getD(); j++) {
            ants[i].tour.trips[j].visited.clear();
            ants[i].tour.trips[j].path.clear();
        }
        ants[i].tour.trips[0].path.push_back(0);
        for (int j = 0; j < g.getD(); j++) {
            ants[i].tour.trips[j].visited.resize(g.getOrder(), false);
            ants[i].tour.trips[j].visited[1] = true;
        }
        ants[i].tour.trips[0].visited[0] = true;
        int aleatory_number = dis(rd);
        Node *node = g.getNodeObjectId(aleatory_number);
        ants[i].tour.trips[0].path.push_back(node->getId());
        ants[i].tour.trips[0].visited[node->getObjectId()] = true;
        ants[i].solution_value = node->getWeight();
        ants[i].inicialTime = sqrt((g.getNode(0)->getX() - g.getNode(aleatory_number)->getX()) *
                                   (g.getNode(0)->getX() - g.getNode(aleatory_number)->getX()) +
                                   (g.getNode(0)->getY() - g.getNode(aleatory_number)->getY()) *
                                   (g.getNode(0)->getY() - g.getNode(aleatory_number)->getY()));
    }
}

void initializeParameters(vector<Ant> &ants, Graph &g, float pheromone) {
    Node *node = g.getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            edge->setPheromone(pheromone);
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    initializeAnts(g, ants, g.getOrder());
}

Edge *
selectNextNode(Ant &ant, Graph &g, float alpha, float beta, int trip, double maxTripTime, double tripTime, bool h1) {
    Edge *edges[g.getOrder()];
    int n_edges = 0, current_node;
    if (trip == 0)
        current_node = ant.tour.trips[trip].path.back();
    else
        current_node = ant.tour.trips[trip - 1].path.back();

    Edge *edge = g.getNode(current_node)->getFirstEdge();
    double q = 0;
    vector<float> qualities;
    float quality;
    Edge *hotelEdge;
    // analisa todas das arestas do no atual
    while (edge != nullptr) {
        Node *node = g.getNode(edge->getTargetId()); 
        // se o nó destino da aresta não foi visitado, calculo a "qualidade" do mesmo e coloca no vector de qualidades
        if (!ant.tour.trips[trip].visited[node->getObjectId()]) {
            quality = node->getWeight() / (edge->getWeight()+1);
            qualities.push_back(quality); 
            q += pow(edge->getPheromone(), alpha) * pow(quality, beta); // o q será usado para o calculo de prbabilidades de cada nó q possa ser escolhido
            edges[n_edges] = edge;
            n_edges++;
        }
        edge = edge->getNextEdge();
    }

    vector<double> probabilities(n_edges, 0.0); // vector para armazenar as probabilidades

    for (int k = 0; k < n_edges; k++) {
        probabilities[k] = (pow(edges[k]->getPheromone(), alpha) * pow(qualities[k], beta)) / q; // caclula as probabilidades baseado na qualidade
    }
    // p será utilizado como margem para selecionar o nó conforme a probabilidade
    double p[probabilities.size()];
    for (int i = 0; i < probabilities.size(); i++)
        p[i] = probabilities[i] * 10000;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0.0, 10000.0);
    double r = distribution(gen);
    double t = 0;
    float distance;
    for (int i = 0; i < probabilities.size(); i++) {
        t += p[i];
        // soma t até chegar no valor aleatório sorteado
        if (t >= r) {
            // se for a última trip, precisamos forçar que encerre  em h1
            if(h1){
                Node *n1 = g.getNode(edges[i]->getTargetId());
                Node *n2 = g.getNode(1);
                hotelEdge = n1->getEdge(n2->getObjectId());
            }// senão, vamos encerrar no hotel mais próximo 
            else{
                hotelEdge = closestHotel(g, g.getNode(edges[i]->getTargetId()));
            }
            distance = sqrt((g.getNode(hotelEdge->getTargetId())->getX() - g.getNode(edges[i]->getTargetId())->getX()) * (g.getNode(hotelEdge->getTargetId())->getX() - g.getNode(edges[i]->getTargetId())->getX()) + (g.getNode(hotelEdge->getTargetId())->getY() - g.getNode(edges[i]->getTargetId())->getY()) * (g.getNode(hotelEdge->getTargetId())->getY() - g.getNode(edges[i]->getTargetId())->getY()));
            // caso haja disponibilidade de tempo para ir ao próximo nó, e, dele, ir ao hotel mais próximo, então podemos ir ao nó
            if(tripTime + edges[i]->getWeight() + distance < maxTripTime && g.getNode(edges[i]->getTargetId())->getType() == 'V'){
                return edges[i];
            }
        }
    }
    return hotelEdge;
}

// retorna o hotel mais próximo ao nó
Edge *closestHotel(Graph &g, Node *current_node){ 
    Edge *edge = current_node->getFirstEdge();
    float closest = numeric_limits<float>::max();
    Edge *closestHotel;
    float distance;
    // analisa todas as arestas do no que possivelmente será escolhido
    // retorna o hotel mais próximo a esse nó
    while(edge != nullptr){
        Node *node = g.getNode(edge->getTargetId());
        // cálculo da distância 
        distance = sqrt((current_node->getX() - node->getX()) * (current_node->getX() - node->getX()) + (current_node->getY() - node->getY()) * (current_node->getY() - node->getY()));
        if(node->getType() == 'H' && distance < closest){
            closest = distance;
            closestHotel = edge;
        }
        edge = edge->getNextEdge();
    }
    return closestHotel;
}