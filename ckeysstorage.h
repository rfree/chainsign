#ifndef CKEYSSTORAGE_H
#define CKEYSSTORAGE_H

#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>

class cKeysStorage
{
public:
    cKeysStorage();
    bool generate(std::string pFileName);    // generate pub and prv key
    bool sign(std::string pFileName, unsigned int pNumberOfKey);
    bool verify(std::string pFileName, unsigned int pNumberOfKey);
    //cryptopp
    void RSASignFile(const std::string& messageFilename, const std::string& signatureFilename, unsigned int numberOfKey);
    void RSAVerifyFile(const std::string& fileName);
    void GenerateRSAKey(unsigned int keyLength);
private:
    RSA* createRSAWithFilename(const char * filename, bool pub);
    
    //cryptopp
    std::vector <CryptoPP::RSA::PrivateKey> mPrvKeys;
    //CryptoPP::RSA::PublicKey mCurrentPublicKey;
    void savePubFile(unsigned int numberOfKey, const CryptoPP::RSA::PublicKey& pPubKey);
    CryptoPP::RSA::PublicKey loadPubFile(unsigned int numberOfKey);
	
	//CryptoPP::AutoSeededRandomPool rng;
    
};

#endif // CKEYSSTORAGE_H
