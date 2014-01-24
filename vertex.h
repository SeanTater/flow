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

template<typename Item>
class Vertex {
public:
    vector<Edge<Item>> edges;
    uint count;
    double cutoff;
    Item text;
    char rounds;
    bitset<64> hashtable;
    void link_vertex(Vertex<Item> *endpoint, uint edge_count);
    void refresh_hashtable(Vertex<Item> *entry);
    void relation(Vertex *parent, unordered_map<Vertex<Item> *, double> &scores, double factor, int degree=2);
};


template<typename Item>
void Vertex<Item>::link_vertex(Vertex<Item> * endpoint, uint edge_count) {
    static random_device rdev;
    static mt19937 generator(rdev());
    double r = generate_canonical<double, 64>(generator);
    if (hashtable[(ulong)endpoint % 64]) {
        auto pos = lower_bound(edges.begin(), edges.end(), endpoint);
        if (pos != edges.end() && pos->endpoint == endpoint) {
            pos->score = min(pos->score, r);
            pos->count++;
            return;
        }/*
        for (VEdge &edge : edges) {
            if (edge.endpoint == endpoint) {
                edge.count += 1;//edge_count;
                return;
            }
        }*/
    }

    if (r < cutoff) {
        // Create a new edge
        auto pos = lower_bound(edges.begin(), edges.end(), endpoint);
        pos = edges.emplace(pos);
        pos->endpoint = endpoint;
        pos->score = r;
        pos->count = 1; //edge_count;

        if (edges.size() > 15) {
            // Erase the worst of our entries
            auto m = max_element(edges.begin(), edges.end());
            cutoff = m->score;
            edges.erase(m);
            refresh_hashtable(endpoint);
            // Erase the same one from theirs too
            m = lower_bound(endpoint->edges.begin(), endpoint->edges.end(), this);
            if (m != endpoint->edges.end() && m->endpoint == this)
                endpoint->edges.erase(m);
            endpoint->refresh_hashtable(this);
        }


    }
}

template<typename Item>
void Vertex<Item>::refresh_hashtable(Vertex<Item> *entry) {
    if (not (rounds % 16)) {
        hashtable.reset();
        for (Edge<Item> &edge : edges) {
            hashtable[(ulong)edge.endpoint % 64] = true;
        }
    } else {
        hashtable[(ulong)entry % 64] = true;
    }
    rounds++;
}

template<typename Item>
void Vertex<Item>::relation(Vertex *parent, unordered_map<Vertex<Item>*, double> &scores, double factor, int degree) {
    scores[this] += factor;
    if (degree == 0) return;
    for (Edge<Item> &edge : edges) {
        if (edge.endpoint != parent) {
            // (double)edge.count / (double)count
            edge.endpoint->relation(this, scores, factor * edge.score, degree-1);
        }
    }
}



#endif // VERTEX_H
