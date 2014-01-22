#ifndef VERTEX_H
#define VERTEX_H
#include "edge.h"
#include <vector>
#include <bitset>
#include <string>
#include <unordered_map>
#include <random>
#include <algorithm>
using namespace std;

class Vertex {
public:
    vector<Edge> edges;
    uint count;
    double cutoff;
    string text;
    char rounds;
    bitset<64> hashtable;
    void link_vertex(Vertex * endpoint, uint edge_count);
    void refresh_hashtable(Vertex *entry);
    void relation(Vertex *parent, unordered_map<Vertex*, double> &scores, double factor, int degree);
};


#endif // VERTEX_H
