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
