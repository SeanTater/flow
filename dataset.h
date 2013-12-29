#ifndef DATASET_H
#define DATASET_H
#include <vector>
#include <string>
#include <iostream>
#include <sqlite3.h>
#include <sstream>

#include "row.h"
using namespace std;

class Dataset {
private:
    sqlite3 * db;
    string table;
    int start_id = 0;
public:
    Dataset(string filename, string table);
    ~Dataset();
    vector<Row> load_block(int id);
};

#endif // DATASET_H
