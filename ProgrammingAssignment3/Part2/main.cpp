#include "file.h"
#include <iostream>
#include <random>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/engine.h>
#include <openssl/crypto.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
using namespace std;

uint64_t readSessionKey(string sessionFile)
{
    char * c = new char[1];
    unsigned char character;
    uint8_t cha;
    uint64_t desKey = 0;
    ifstream desKeyIn(sessionFile, ios::binary);
    for(int i = 0; i < 8; i++)
    {
        desKey = desKey << 8;
        desKeyIn.read(c,1);
        character = *c;
        cha = (unsigned int) character;
        desKey = desKey | (((uint64_t) cha) & 0x00000000000000ff);
    }
    desKeyIn.close();
    
    return desKey;
}

uint64_t readIV(string ivFile)
{
    char * c = new char[1];
    unsigned char character;
    uint8_t cha;
    uint64_t iv = 0;
    ifstream ivIn(ivFile, ios::binary);
    for(int i = 0; i < 16; i++)
    {
        iv = iv << 4;
        ivIn.read(c,1);
        character = *c;
        cha = (unsigned int) character;
        if(cha >= 97)
        {
            cha = cha - 87;
        }
        else if(cha >= 48)
        {
            cha = cha - 48;
        }
        iv = iv | (((uint64_t) cha) & 0x00000000000000ff);
    }
    ivIn.close();
    
    return iv;
}

int verifySign(string publicFile, string ciphertextFile, string signatureFile)
{
    EVP_PKEY_CTX *ctx;
    EVP_MD_CTX *md_ctx;
    const EVP_MD *md_ptr = EVP_sha256();
    unsigned char *md, *sig, *des_cip;
    size_t mdlen = 32, siglen, des_ciplen;
    EVP_PKEY *verkey;
    FILE *pub_key = fopen(publicFile.c_str(), "r");
    FILE *cipher = fopen(ciphertextFile.c_str(), "r");
    FILE *sign = fopen(signatureFile.c_str(), "r");
    
    fseek(cipher, 0, SEEK_END);
    des_ciplen = ftell(cipher);
    fseek(cipher, 0, SEEK_SET);
    des_cip = (unsigned char *)malloc(des_ciplen);
    fread(des_cip, 1, des_ciplen, cipher);
    fclose(cipher);
    
    md = (unsigned char *)malloc(mdlen);
    md_ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(md_ctx, md_ptr, NULL);
    EVP_DigestUpdate(md_ctx, des_cip, des_ciplen);
    EVP_DigestFinal_ex(md_ctx, md, NULL);
    EVP_MD_CTX_destroy(md_ctx);
    
    fseek(sign, 0, SEEK_END);
    siglen = ftell(sign);
    fseek(sign, 0, SEEK_SET);
    sig = (unsigned char *)malloc(siglen);
    fread(sig, 1, siglen, sign);
    fclose(sign);
    
    verkey = PEM_read_PUBKEY(pub_key, NULL, NULL, NULL);
    ctx = EVP_PKEY_CTX_new(verkey, NULL);
    EVP_PKEY_verify_init(ctx);
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
    int vercode = EVP_PKEY_verify(ctx, sig, siglen, md, mdlen);
    
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(verkey);
    free(md);
    free(sig);
    free(des_cip);
    
    return vercode;
}
    
//main function
int main(int argc, char **argv)
{
    string ciphertextFile, sessionFile, publicFile, signatureFile, ivFile;
    string temp;
    
    for(int i = 0; i < argc; i++)
    {
        temp = argv[i];
        if(temp == "-in")
        {
            temp = argv[i + 1];
            ciphertextFile = temp;
        }
        else if(temp == "-sess")
        {
            temp = argv[i + 1];
            sessionFile = temp;
        }
        else if(temp == "-pub")
        {
            temp = argv[i + 1];
            publicFile = temp;
        }
        else if(temp == "-s")
        {
            temp = argv[i + 1];
            signatureFile = temp;
        }
        else if(temp == "-iv")
        {
            temp = argv[i + 1];
            ivFile = temp;
        }
    }
    
    //creates an object for reading and writing bytes
    uint64_t key = readSessionKey(sessionFile);
    uint64_t IV = readIV(ivFile);
    
    string decryptedFile = "dec_" + ciphertextFile.substr(4);
    file f(key, IV);
    f.decrypt(ciphertextFile, decryptedFile);
    int verify = verifySign(publicFile, ciphertextFile, signatureFile);
    
    if(verify == 1)
    {
        cout << "Signature verified." << endl;
    }
    else if(verify == 0)
    {
        cout << "Signature not verified." << endl;
    }
    
    return 0;
}
        