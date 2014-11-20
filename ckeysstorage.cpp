#include "ckeysstorage.h"

#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

cKeysStorage::cKeysStorage()
{
}

// http://www.codepool.biz/tech-frontier/how-to-use-openssl-generate-rsa-keys-cc.html
bool cKeysStorage::generate(std::string pFileName)
{
    int             ret = 0;
    RSA             *rsa = NULL;
    BIGNUM          *bne = NULL;
    BIO             *bp_public = NULL, *bp_private = NULL;

    int             bits = 4096;
    unsigned long   e = RSA_F4;

    // 1. generate rsa key
    bne = BN_new();
    ret = BN_set_word(bne, e);

    rsa = RSA_new();
    ret = RSA_generate_key_ex(rsa, bits, bne, NULL);

    // 2. save public key
    std::string mFileName = pFileName;
    mFileName += "_pub.pem";
    bp_public = BIO_new_file(mFileName.c_str(), "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, rsa);

    // 3. save private key
    mFileName = pFileName;
    mFileName += "_prv.pem";
    bp_private = BIO_new_file(mFileName.c_str(), "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, rsa, NULL, NULL, 0, NULL, NULL);

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(rsa);
    BN_free(bne);

    return (ret == 1);
}

bool cKeysStorage::sign(std::string pFileName, unsigned int pNumberOfKey)
{
    std::ifstream mInputFile;
    std::stringstream ss;
    ss << pNumberOfKey;
    ss << "_prv.pem";
    RSA *rsa = createRSAWithFilename(ss.str().c_str(), true);
    if (rsa == NULL)
        return false;

    mInputFile.open(pFileName, std::ios::in | std::ios::binary);
    if (!mInputFile.is_open())
        return false;

    // get length of file:
    mInputFile.seekg (0, mInputFile.end);
    int mLength = mInputFile.tellg();
    mInputFile.seekg (0, mInputFile.beg);

    std::shared_ptr<void> mBuffer (new char[mLength]);
    std::shared_ptr<void> mOutBuffer (new char[mLength]);
    mInputFile.read(std::static_pointer_cast<char>(mBuffer).get(), mLength);
    mInputFile.close();

    RSA_private_encrypt(mLength,
                        std::static_pointer_cast<unsigned char>(mBuffer).get(),
                        std::static_pointer_cast<unsigned char>(mOutBuffer).get(),
                        rsa,
                        RSA_NO_PADDING);
    RSA_free(rsa);
    std::ofstream mOutFile;
    std::string mOutName(pFileName);
    mOutName += ".pub";
    mOutFile.open(mOutName);
    //save header
    mOutFile << "version 1" << std::endl;
    mOutFile << "crypto rsa" << std::endl;
    mOutFile << "size 4096" << std::endl;
    mOutFile << "END" << std::endl;
    mOutFile.close();
    //save data
    mOutFile.open(mOutName, std::ios::out | std::ios::app | std::ios::binary);
    mOutFile.write(std::static_pointer_cast<char>(mOutBuffer).get(), mLength);
    mOutFile.close();
    return true;
}

bool cKeysStorage::verify(std::string pFileName, unsigned int pNumberOfKey)
{

    return true;
}


RSA* cKeysStorage::createRSAWithFilename(const char * filename, bool pub)
{
    FILE * fp = fopen(filename,"rb");

    if(fp == NULL)
    {
        return NULL;
    }
    RSA *rsa= RSA_new() ;

    if(pub)
    {
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa,NULL, NULL);
    }

    return rsa;
}
