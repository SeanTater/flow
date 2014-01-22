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
    flow.train(10000);
    flow.test(10000);
    return 0;
}
