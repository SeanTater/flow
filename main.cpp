#include <iostream>
#include <vector>
#include <string>

#include "sek.h"

using namespace std;

int main()
{
    Sek sek;
    sek.train(5000);
    sek.test(1000);
    return 0;
}
