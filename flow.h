#ifndef FLOW_H
#define FLOW_H
#include <vector>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <boost/pool/poolfwd.hpp>
#include <boost/pool/pool_alloc.hpp>
#include "dataset.h"
#include "row.h"

class Edge {
public:
    union {
        ulong id;
        struct {
            uint to;
            uint from;
        };
    };
    mutable uint count;

    Edge() {
        id = 0;
    }
    Edge(uint f, uint t) {
        from = f;
        to = t;
    }
    bool operator <(const Edge &other) const {
        return id < other.id;
    }
};

class Flow
{
    Dataset dataset;
    int connection_limit;
    unordered_map<string, uint> word_to_id;
    vector<string> id_to_word;
    vector<uint> word_count;
    // Map, not unordered map.
    // It's slower (by about 2x actually) for training
    // but! Ordering implies that you can search for a start word and iterate all its results

    map<Edge,
        uint,
        less<Edge>,
        boost::fast_pool_allocator<pair<const Edge, uint> >
    > edges;
public:
    Flow();
    void train(int row_limit=INT_MAX);
    void relation(vector<string> words);
    void relation(uint word, uint parent, unordered_map<uint, double> &scores, int degree=3, double factor=1);
    double score(Edge edge, uint count);
};

#endif // FLOW_H
