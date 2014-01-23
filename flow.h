#ifndef FLOW_H
#define FLOW_H
#include <vector>
#include <unordered_map>
#include "dataset.h"
#include "row.h"
#include "vertex.h"
#include "edge.h"


/// Theory
// Count words
// Count the edges between words
// Keep only the million best edges
//   Best means highest count

/// Implementation
// Read record in blocks to keep from leaking memory
// Read edges in blocks
//      since individually would not allow good comparisons
//      and since all at once leaks memory

template<typename Item>
class ScoredResult {
public:
    Item result;
    double score;

    ScoredResult(Item i, double s) {
        result = i;
        score = s;
    }

    bool operator<(const ScoredResult result) const {
        return score < result.score;
    }

    bool operator>(const ScoredResult result) const {
        return score > result.score;
    }
};

template<typename Item>
class Flow
{
    typedef Vertex<Item> IVertex;
    typedef unordered_map<Item, IVertex*> VertexMap;
    VertexMap vertices;
public:
    void train(vector<Item> message);
    void test(int row_limit=INT_MAX);
    vector<ScoredResult<Item>> test(vector<Item> message);
};

template<typename Item>
void Flow<Item>::train(vector<Item> message) {
    unordered_map<Item, uint> message_word_count;
    for (Item &phrase : message) {
        IVertex *vert = vertices[phrase];
        if (not vert) vert = vertices[phrase] = new IVertex();
        vert->text = phrase;
        vert->count++;
        message_word_count[phrase]++;
    }
    for (pair<const Item, uint> &start : message_word_count) {
        IVertex *start_vert = vertices[start.first];
        for (pair<const Item, uint> &end : message_word_count) {
            if (start.first != end.first) {
                IVertex *end_vert = vertices[end.first];
                start_vert->link_vertex(end_vert, end.second);
            }
        }
    }
}

template<typename Item>
vector<ScoredResult<Item> > Flow<Item>::test(vector<Item> message) {
    unordered_map<string, uint> message_word_count;
    for (Item &text : message) {
        message_word_count[text]++;
    }

    unordered_map<IVertex*, double> links;
    for (pair<const Item, uint> &start : message_word_count) {
        IVertex *vert = vertices[start.first];
        if (not vert) continue; // Can't find it
        vert->relation(vert, links, start.second, 2);
    }

    vector<ScoredResult<Item>> ordered_links;
    for (auto &link : links) {
        //TODO: Find a text-agnostic way of determining this
        if (link.first->text.substr(0, 4) == "tag:")
            ordered_links.push_back(ScoredResult<Item>(link.first->text, link.second));
    }

    sort(ordered_links.begin(), ordered_links.end(), std::greater<ScoredResult<Item> >());

    return ordered_links;
}



#endif // FLOW_H
