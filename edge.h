#ifndef EDGE_H
#define EDGE_H

class Vertex;
class Edge {
public:
    Vertex *endpoint;
    unsigned int count;
    double score;
    bool operator <(const Edge &other);
    bool operator <(const Vertex *other);
};

#endif // EDGE_H
