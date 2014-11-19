#include <iostream>
#include <chrono>
#include "ckeysstorage.h"

using namespace std;

int main()
{
    //std::chrono::steady_clock::time_point begin = std::chrono::system_clock::now();
    cout << "Hello World!" << endl;
    cKeysStorage keyStorage = cKeysStorage();
    for (int i = 0; i < 100; ++i)
        keyStorage.generate(std::to_string(i));

    return 0;
}

