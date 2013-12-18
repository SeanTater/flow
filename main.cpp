#include <iostream>
#include <vector>
#include <string>

#include "dataset.h"
#include "row.h"
#include "flow.h"

using namespace std;

int main()
{
    Flow flow;
    flow.train(100000);
    flow.relation(vector<string>({"file", "permissions"}));
    flow.relation(vector<string>({"c", "++", "private", "protected"}));
    return 0;
}
