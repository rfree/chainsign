#ifndef CKEYSSTORAGE_H
#define CKEYSSTORAGE_H

#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>

class cKeysStorage
{
public:
    cKeysStorage();
    bool generate(std::string pFileName);    // generate pub and prv key
    bool sign(std::string pFileName, unsigned int pNumberOfKey);
    bool verify(std::string pFileName, unsigned int pNumberOfKey);
private:
    RSA* createRSAWithFilename(const char * filename, bool pub);
};

#endif // CKEYSSTORAGE_H
