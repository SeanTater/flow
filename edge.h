#ifndef EDGE_H
#define EDGE_H

template<typename Item>
class Vertex;

template<typename Item>
class Edge {
public:
    Vertex<Item> *endpoint;
    unsigned int count;
    double score;
    bool operator <(const Edge<Item> &other);
    bool operator <(const Vertex<Item> *other);
};

template<typename Item>
bool Edge<Item>::operator <(const Edge<Item> &other) {
    return score < other.score;
}

template<typename Item>
bool Edge<Item>::operator <(const Vertex<Item> *other) {
    return (unsigned long)endpoint < (unsigned long)other;
}

#endif // EDGE_H
