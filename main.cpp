#include <iostream>
#include <vector>
#include <string>

#include "sek.h"

using namespace std;

int main()
{
    Sek sek;
    sek.train(10000);
    sek.test(10000);
    return 0;
}
