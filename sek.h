#ifndef SEK_H
#define SEK_H
#include <string>
#include <vector>
#include "flow.h"
#include "dataset.h"
#include "row.h"

class Sek
{
public:
    Flow<string> flowgraph;
    void train(int row_limit);
    void train_block(vector<Row>&);
};

#endif // SEK_H
