#include "dataset.h"

Dataset::Dataset(string filename, string table) {
    sqlite3_open(filename.c_str(), &db);
}

Dataset::~Dataset() {
    sqlite3_close(db);
}

vector<Row> Dataset::load_block(int start_id) {
    vector<Row> block;
    int response;
    sqlite3_stmt *statement;
    response = sqlite3_prepare_v2(
        db,
        "SELECT id, title, body, tags FROM train WHERE id >= ? ORDER BY id LIMIT 1000",
        -1,
        &statement,
        NULL);
    if (response != SQLITE_OK)
        throw string(sqlite3_errmsg(db));
    sqlite3_bind_int(statement, 1, start_id);

    response = sqlite3_step(statement);
    while (response != SQLITE_DONE) {
        block.emplace_back(
          sqlite3_column_int(statement, 0), // id
          string((char *)sqlite3_column_text(statement, 1)), // title
          string((char *)sqlite3_column_text(statement, 2)), // body
          string((char *)sqlite3_column_text(statement, 3)) // tags
          );
        if (response != SQLITE_ROW && response != SQLITE_DONE) {
            string errmsg(sqlite3_errmsg(db));
            sqlite3_finalize(statement);
            throw errmsg;
        }
        response = sqlite3_step(statement);
    }

    sqlite3_finalize(statement);
    return block;
}

