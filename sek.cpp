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
        flowgraph.train(row.words_with_tags);
    }
}

void Sek::test(int row_limit, bool submit) {
    int start_index = 0;
    ofstream submissions;
    if (submit) {
        // If submitting, use test data as input
        // and save results to a file
        submissions.open("/tmp/submit.csv");
        submissions << "Id,Tags" << endl;

    } else {
        // When doing diagnostics, use training data
        // and start long past where the training ended
        start_index = 100000;
    }


    Dataset dataset("/tmp/sek.db", submit ? "test" : "train");

    int index = start_index;
    vector<Row> rows = dataset.load_block(start_index);

    struct PR {
        double tp;
        double fp;
        double fn;
    };
    unordered_map<string, PR> scores;

    while(not rows.empty() and (index - start_index) < row_limit) {
        for (Row &row : rows) {
            // Gives the list of related words
            auto scored_results = flowgraph.test(row.words);
            cout << scored_results.size() << endl;
            vector<string> tags;
            // Forget the scores, keep only the tags and rank
            for (auto result : scored_results) {
                cout << result.result << endl;
                if (result.result.substr(0, 4) == "tag:")
                    tags.push_back(result.result.substr(5));
            }

            // Remove everything after the null tag
            tags.erase(find(tags.begin(), tags.end(), "0"), tags.end());

            if (submit) {
                // Push the results to a file
                submissions << row.id << ",\"";
                for (string tag : tags) {
                    submissions << tag << " ";
                }
                submissions << "\"" << endl;
            } else {
                // Calculate statistics and diagnostics
                cout << row.id << ", have: ";
                // See whether the positives were actually right
                for (string tag : tags) {
                    cout << tag << " ";
                    auto result = find(row.tags.begin(), row.tags.end(), tag);
                    if (result != row.tags.end()) {
                        // True positive
                        scores[tag].tp += 1;
                    } else
                        // False positive
                        scores[tag].fp += 1;
                }

                cout << endl << "want:";

                // See how many we didn't guess that we should have
                for (string row_tag : row.tags) {
                    cout << row_tag << " ";
                    auto result = find(tags.begin(), tags.end(), row_tag);
                    if (result == tags.end())
                        // False negative
                        scores[row_tag].fn += 1;
                }
                cout << endl;
            }
        }

        index = rows.back().id + 1;
        rows = dataset.load_block(index);
    }

    if (submit) {
        submissions.close();
    } else {
        // Calculate the mean F1
        double f1 = 0;
        for (auto text_score : scores) {
            PR score = text_score.second;
            f1 += 2*score.tp / ( 2*score.tp + score.fp + score.fn );
        }
        f1 /= scores.size();
        printf("Mean f1 %f%%\n", f1);
    }

}
