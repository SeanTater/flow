#include "edge.h"

bool Edge::operator <(const Edge &other) {
    return score < other.score;
}

bool Edge::operator <(const Vertex *other) {
    return (unsigned long)endpoint < (unsigned long)other;
}
