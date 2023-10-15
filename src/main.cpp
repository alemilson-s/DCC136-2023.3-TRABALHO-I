#include <iostream>
#include "Graph.h"
#include "Node.h"
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "aco.h"
#include <chrono>

using namespace std;


double calculaDistancia(Node &n, Node &m) {
    return sqrt((m.getX() - n.getX()) * (m.getX() - n.getX()) + (m.getY() - n.getY()) * (m.getY() - n.getY()));
}

Graph *leitura(ifstream &input_file) {
    Graph *g = new Graph(0, false, true, true);
    if (!input_file) {
        cout << "Arquivo não está aberto!" << endl;
        return nullptr;
    }
    int id = 0, n, h, d;
    input_file >> n >> h >> d;
    g->setD(d);
    g->setN(n);
    g->setH(h);
    double x, y, weight_node, t_max;
    input_file >> t_max;
    g->setTMax(t_max);
    vector<double> t_d;
    for (int i = 0; i < d; i++) {
        double value;
        input_file >> value;
        t_d.push_back(value);
    }
    g->setTD(t_d);
    input_file >> x >> y >> weight_node;
    Node *node = g->allocateNode(id++);
    node->setX(x);
    node->setY(y);
    node->setWeight(weight_node);
    node->setType('S');
    input_file >> x >> y >> weight_node;
    node = g->allocateNode(id++);
    node->setX(x);
    node->setY(y);
    node->setWeight(weight_node);
    node->setType('E');
    for (int i = 0; i < h; i++) {
        input_file >> x >> y >> weight_node;
        node = g->allocateNode(id++);
        node->setX(x);
        node->setY(y);
        node->setWeight(weight_node);
        node->setType('H');
    }
    for (int i = 0; i < n - 2; i++) {
        input_file >> x >> y >> weight_node;
        node = g->allocateNode(id++);
        node->setX(x);
        node->setY(y);
        node->setWeight(weight_node);
        node->setType('V');
    }
    node = g->getFirstNode();
    while (node != nullptr) {
        Node *next = node->getNextNode();
        while (next != nullptr) {
            double weight_edge = calculaDistancia(*node, *next);
            node->insertEdge(next->getId(), weight_edge);
            next->insertEdge(node->getId(), weight_edge);
            next = next->getNextNode();
        }
        node = node->getNextNode();
    }
    return g;
}

int main(int argc, char **argv) {
    if (argc != 2 && argc != 1) {
        cout << "ERROR: Expecting: ./<program_name> <input_file>" << endl;
        cout << argv[1] << endl;
        cout << "Passar arquivo de entrada como parâmertro!" << endl;
        return -1;
    }
    string file_name = argv[1];

    ifstream input_file;
    input_file.open(file_name, ios::in);

    Graph *g = leitura(input_file);

    if (input_file.is_open())
        input_file.close();
    int cycles = 100;

//    g->print();
    auto start_time = std::chrono::high_resolution_clock::now();
    aco(*g, cycles, 0.7, 1, 9);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    delete g;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;
    return 0;

}