#include "flow.h"

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
    Dataset dataset("/tmp/sek.db", "train");
    vector<Row> rows = dataset.load_block(train_start_index);

    while(not rows.empty()) {
        train_block(rows);

        train_start_index = rows.back().id + 1;
        rows = dataset.load_block(train_start_index);
        cout << "train start: " << train_start_index << endl;

        if (train_start_index > row_limit) {
            // Short training
            break;
        }
    }
}

void Flow::train_block(vector<Row> &rows) {
    for (Row &row : rows) {
        unordered_map<string, uint> message_word_count;
        for (string &text : row.words) {
            Vertex *vert = vertices[text];
            if (not vert) vert = vertices[text] = new Vertex();
            vert->text = text;
            vert->count++;
            message_word_count[text]++;
        }
        for (pair<const string, uint> &start : message_word_count) {
            Vertex *start_vert = vertices[start.first];
            for (pair<const string, uint> &end : message_word_count) {
                if (start.first != end.first) {
                    Vertex *end_vert = vertices[end.first];
                    start_vert->link_vertex(end_vert, end.second);
                }
            }
        }
    }
}

void Flow::test(int row_limit) {
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
