#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "sek.h"

using namespace std;

int main()
{
    Sek sek;
    sek.train(5000);
    sek.test(1000, false);
    return 0;
}
