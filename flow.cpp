#include "flow.h"

Flow::Flow() : dataset("/tmp/sek.db", "train")
{
    connection_limit = 100;
}


void Flow::train(int row_limit) {
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


    int train_start_index = 0;
    vector<Row> rows = dataset.load_block(train_start_index);
    while(not rows.empty()) {

        for (Row &row : rows) {
            unordered_map<uint, uint> message_word_count;
            for (string &text : row.words) {
                uint id;
                try {
                    id = word_to_id.at(text);
                } catch (out_of_range) {
                    word_to_id[text] = id = id_to_word.size();
                    id_to_word.push_back(text);
                    word_count.push_back(0);
                }
                word_count[id]++;
                message_word_count[id]++;
            }
            for (pair<const uint, uint> &start : message_word_count) {
                Edge ends = Edge(start.first, 0);
                for (pair<const uint, uint> &end : message_word_count) {
                    if (start.first != end.first) {
                        ends.to = end.first;
                        edges[ends] += end.second;
                    }
                }
            }

        }

        train_start_index = rows.back().id + 1;
        rows = dataset.load_block(train_start_index);
        cout << "train start: " << train_start_index << endl;
        cout << "edge count: " << edges.size() <<endl;

        // Every 100 million unique edges, cut out the worst 25 million
        if (edges.size() > 10000000) {
            vector<pair<float, Edge> > worst;
            worst.reserve(12000000);
            for (pair<Edge, uint> edge : edges) {
                worst.emplace_back(score(edge.first, edge.second), edge.first);
            }
            nth_element(worst.begin(), worst.begin() + 5000000, worst.end());
            for (int i=0; i<25000000; i++) {
                edges.erase(worst[i].second);
            }
        }

        if (train_start_index > row_limit) {
            // Short training
            break;
        }
    }
}

void Flow::relation(vector<string> words) {
    unordered_map<uint, double> scores;
    vector<pair<double, string> > sorted;
    for (string word : words) {
        relation(word_to_id[word], ~0, scores, 1);
    }
    for (pair<const uint, double> entry : scores) {
        sorted.emplace_back(entry.second, id_to_word[entry.first]);
    }
    sort(sorted.begin(), sorted.end());
    reverse(sorted.begin(), sorted.end());
    sorted.resize(100);
    for (pair<double, string> entry : sorted) {
        cout << entry.first << " -> " << entry.second << endl;
    }
}

double Flow::score(Edge edge, uint count) {
    if (count < 10) return 0;
    return count / (double)(word_count[edge.from] * word_count[edge.to]);
}

void Flow::relation(uint word, uint parent, unordered_map<uint, double> &scores, int degree, double factor) {
    scores[word] += factor;
    cout << id_to_word[word] << " " << factor << endl;
    assert(factor <= 1);
    if (not degree) return;
    double total=0;

    // Where should you stop? degrees-of-kevin-bacon
    Edge ends = Edge(word, 0);
    auto start = edges.lower_bound(ends);
    ends.from++;
    auto end = edges.lower_bound(ends);
    /*for(auto result=start; result != end; result++) {
        //cout << "> " << id_to_word[ends.to] << endl;
        auto edge = *result;
        if (edge.first.to != parent) {
            double v = score(edge.first, edge.second);
            total += v;
        }
    }*/

    for(auto result=start; result != end; result++) {
        auto edge = *result;
        if (edge.first.to != parent) {
            double v = score(edge.first, edge.second);
            relation(edge.first.to, word, scores, degree-1, factor * v);
        }
    }
}
