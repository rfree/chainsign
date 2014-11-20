#ifndef CKEYSSTORAGE_H
#define CKEYSSTORAGE_H

#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <crypto++/rsa.h>

class cKeysStorage
{
public:
    cKeysStorage();
    bool generate(std::string pFileName);    // generate pub and prv key
    bool sign(std::string pFileName, unsigned int pNumberOfKey);
    bool verify(std::string pFileName, unsigned int pNumberOfKey);
    //cryptopp
    void RSASignFile(const char *privFilename, const char *messageFilename, const char *signatureFilename);
    void GenerateRSAKey(unsigned int keyLength, const char *pubFilename);
private:
    RSA* createRSAWithFilename(const char * filename, bool pub);
    
    //cryptopp
    std::vector <CryptoPP::RSA::PrivateKey> mPrvKeys;
    
};

#endif // CKEYSSTORAGE_H
