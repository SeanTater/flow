#include <iostream>
#include <vector>
#include <string>

#include "dataset.h"
#include "row.h"
#include "flow.h"

using namespace std;

int main()
{
    Dataset train("/tmp/sek.db", "train");
    vector<Row> entries = train.load_block(0);
    cout << entries[0].words[0];
    return 0;
}
