#ifndef FLOW_H
#define FLOW_H
#include <vector>
#include <list>
#include <bitset>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cassert>
#include <random>
#include "dataset.h"
#include "row.h"

class Vertex;
class VEdge {
public:
    Vertex *endpoint;
    uint count;
    double score;
    //VEdge(Vertex *e, uint c) {
    //    endpoint = e;
    //    count = c;
    //}
    bool operator <(const VEdge &other) {
        return score < other.score;
    }
    bool operator <(const Vertex *other) {
        return (ulong)endpoint < (ulong)other;
    }
};

class Vertex {
public:
    vector<VEdge> edges;
    uint count;
    double cutoff = 0.0;
    string text;
    char rounds;
    bitset<64> hashtable;

    void link_vertex(Vertex * endpoint, uint edge_count) {
        static random_device rdev;
        static mt19937 generator(rdev());
        double r = log(generate_canonical<double, 64>(generator));
        if (hashtable[(ulong)endpoint % 64]) {
            auto pos = lower_bound(edges.begin(), edges.end(), endpoint);
            if (pos != edges.end() && (*pos).endpoint == endpoint) {
                (*pos).score += r;
                (*pos).count++;
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
            (*pos).endpoint = endpoint;
            (*pos).score = r;
            (*pos).count = 1; //edge_count;

            if (edges.size() > 15) {
                // Erase the worst of our entries
                auto m = max_element(edges.begin(), edges.end());
                cutoff = (*m).score;
                edges.erase(m);
                refresh_hashtable(endpoint);
                // Erase the same one from theirs too
                m = lower_bound(endpoint->edges.begin(), endpoint->edges.end(), this);
                if (m != endpoint->edges.end() && (*m).endpoint == this)
                    endpoint->edges.erase(m);
                endpoint->refresh_hashtable(this);
            }


        }
    }

    void refresh_hashtable(Vertex *entry) {
        if (not (rounds % 16)) {
            hashtable.reset();
            for (VEdge &edge : edges) {
                hashtable[(ulong)edge.endpoint % 64] = true;
            }
        } else {
            hashtable[(ulong)entry % 64] = true;
        }
        rounds++;
    }


    void relation(Vertex *parent, unordered_map<Vertex*, double> &scores, double factor, int degree=2) {
        scores[this] += factor;
        if (degree == 0) return;
        for (VEdge &edge : edges) {
            if (edge.endpoint != parent) {
                // (double)edge.count / (double)count
                edge.endpoint->relation(this, scores, factor * exp(edge.score), degree-1);
            }
        }
    }
};

typedef unordered_map<string, Vertex*> VertexMap;

class Flow
{
    VertexMap vertices;
public:
    void train(int row_limit=INT_MAX);
    void train_block(vector<Row> &rows);
    void test(int row_limit=INT_MAX);
};

#endif // FLOW_H
