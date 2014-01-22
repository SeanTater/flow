#ifndef FLOW_H
#define FLOW_H
#include <vector>
#include <unordered_map>
#include "dataset.h"
#include "row.h"
#include "vertex.h"
#include "edge.h"

template<typename Item>
class Flow
{
    typedef unordered_map<Item, Vertex*> VertexMap;
    VertexMap vertices;
public:
    void train(vector<Item> message);
    void test(int row_limit=INT_MAX);
};

template<typename Item>
void Flow<Item>::train(vector<Item> message) {
    unordered_map<Item, uint> message_word_count;
    for (Item &phrase : message) {
        Vertex *vert = vertices[phrase];
        if (not vert) vert = vertices[phrase] = new Vertex();
        vert->text = phrase;
        vert->count++;
        message_word_count[phrase]++;
    }
    for (pair<const Item, uint> &start : message_word_count) {
        Vertex *start_vert = vertices[start.first];
        for (pair<const Item, uint> &end : message_word_count) {
            if (start.first != end.first) {
                Vertex *end_vert = vertices[end.first];
                start_vert->link_vertex(end_vert, end.second);
            }
        }
    }
}

template<typename Item>
void Flow<Item>::test(int row_limit) {
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


    int start_index = 100000;
    Dataset dataset("/tmp/sek.db", "train");
    vector<Row> rows = dataset.load_block(start_index);
    while(not rows.empty()) {
        for (Row &row : rows) {
            unordered_map<string, uint> message_word_count;
            for (string &text : row.words) {
                message_word_count[text]++;
            }

            unordered_map<Vertex*, double> links;
            for (pair<const string, uint> &start : message_word_count) {
                Vertex *vert = vertices[start.first];
                if (not vert) continue; // Can't find it
                vert->relation(vert, links, start.second, 2);
            }

            vector<pair<Vertex*, double> > ordered_links;
            for (auto &link : links) {
                if (link.first->text.substr(0, 4) == "tag:")
                    ordered_links.push_back(link);
            }

            sort(ordered_links.begin(), ordered_links.end(),
                 [](const pair<Vertex*, double> &v1, const pair<Vertex*, double> &v2){return v1.second < v2.second;}
            );
            cout << links.size() << endl;
            cout << row.id << ",\"";
            for (uint i=0; i<3 && i < ordered_links.size(); i++) {
                cout << ordered_links[i].first->text.substr(5) << " ";
            }
            cout << "\" \"" << row.tag_string << "\"" <<     endl;
            cout << "\"" << endl;
        }

        start_index = rows.back().id + 1;
        rows = dataset.load_block(start_index);

        if (start_index > row_limit) {
            // Short training
            break;
        }
    }
}

#endif // FLOW_H
