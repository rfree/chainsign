#include <iostream>
#include <chrono>
#include "ckeysstorage.h"

using namespace std;

int main()
{
    cKeysStorage keyStorage = cKeysStorage();
    //for (int i = 0; i < 1; ++i)
       // keyStorage.generate(std::to_string(i));

	//keyStorage.sign("test.txt", 0);
	std::srand(time(NULL));
	keyStorage.GenerateRSAKey(4096, "test.pub");

    return 0;
}

