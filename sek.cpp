#include "sek.h"
void Sek::train(int row_limit) {
    int train_start_index = 0;
    Dataset dataset("/tmp/sek.db", "train");
    vector<Row> rows = dataset.load_block(train_start_index);

    while(not rows.empty() and train_start_index < row_limit) {
        train_block(rows);

        train_start_index = rows.back().id + 1;
        rows = dataset.load_block(train_start_index);
        cout << "train start: " << train_start_index << endl;
    }
}

void Sek::train_block(vector<Row> &rows) {
    for (Row &row : rows) {
        flowgraph.train(row.words);
    }
}


void Sek::test(int row_limit) {
    const int start_index = 100000;
    int index = start_index;
    Dataset dataset("/tmp/sek.db", "train");
    vector<Row> rows = dataset.load_block(start_index);
    while(not rows.empty() and (index - start_index) < row_limit) {
        for (Row &row : rows) {
            auto results = flowgraph.test(row.words);
            results.erase(remove_if(results.begin(), results.end(),
                                    [](const ScoredResult<string> r){ return r.result.substr(0, 4) != "tag:"; }), results.end());
            //TODO: Find a text-agnostic way of determining this
            //if (link.first->text.substr(0, 4) == "tag:")

            // Display log printout (capture for submission)
            cout << row.id << ",\"";
            for (uint i=0; i<3 && i < results.size(); i++) {
                cout << results[i].result.substr(4) << " ";
            }
            cout << "\" \"" << row.tag_string << "\"" <<     endl;
            cout << "\"" << endl;
        }

        index = rows.back().id + 1;
        rows = dataset.load_block(index);
    }
}
