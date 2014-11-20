#include <iostream>
#include <chrono>
#include "ckeysstorage.h"

using namespace std;

int main()
{
    cKeysStorage keyStorage = cKeysStorage();
    for (int i = 0; i < 100; ++i)
        keyStorage.generate(std::to_string(i));

    return 0;
}

