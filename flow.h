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
#include "vertex.h"
#include "edge.h"

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
