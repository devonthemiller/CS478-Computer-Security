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
#include <openssl/rand.h>
using namespace std;

//function for calculating a random number
//that is exactly 64 bits long
uint64_t random_64bit()
{
    random_device rd;
    
    mt19937_64 generator(rd());
    
    uniform_int_distribution<long long int> dist( llround(pow(2,61)), llround(pow(2,62)) );
    
    return dist(generator);
}

uint64_t decryptSessionKey(string sessionKeyFile, string publicKeyFile)
{
    EVP_PKEY_CTX *ctx;
    EVP_PKEY *key;
    unsigned char *out, *in;
    size_t inlen, outlen;
    FILE *sessionFile = fopen(sessionKeyFile.c_str(), "r");
    FILE *publicFile = fopen(publicKeyFile.c_str(), "r");
    
    fseek(sessionFile, 0, SEEK_END);
    inlen = ftell(sessionFile);
    fseek(sessionFile, 0, SEEK_SET);
    in = (unsigned char *)malloc(inlen);
    fread(in, 1, inlen, sessionFile);
    fclose(sessionFile);
    
    key = PEM_read_PUBKEY(publicFile, NULL, NULL, NULL);
    fclose(publicFile);
    ctx = EVP_PKEY_CTX_new(key, NULL);
    EVP_PKEY_encrypt_init(ctx);
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING);
    EVP_PKEY_encrypt(ctx, NULL, &outlen, in, inlen);
    out = (unsigned char *)OPENSSL_malloc(outlen);
    EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen);
    
    FILE *sessionKeyOut = fopen("decrypted_session_key.txt", "w");
    fwrite(out, 1, 8, sessionKeyOut);
    fclose(sessionKeyOut);
    
    char * c = new char[1];
    unsigned char character;
    uint8_t cha;
    uint64_t pubKey = 0;
    ifstream pubKeyIn("decrypted_session_key.txt", ios::binary);
    for(int i = 0; i < 8; i++)
    {
        pubKey = pubKey << 8;
        pubKeyIn.read(c,1);
        character = *c;
        cha = (unsigned int) character;
        pubKey = pubKey | (((uint64_t) cha) & 0x00000000000000ff);
    }
    pubKeyIn.close();
    
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(key);
    free(in);
    OPENSSL_free(out);
    
    return pubKey;
}
    
void sign(string privateFile, string ciphertextFile)
{
    EVP_PKEY_CTX *ctx;
    EVP_MD_CTX *md_ctx;
    const EVP_MD *md_ptr = EVP_sha256();
    unsigned char *md, *sig, *des_cip;
    size_t mdlen = 32, siglen, des_ciplen;
    EVP_PKEY *sigkey;
    FILE *des_cipher_in = fopen(ciphertextFile.c_str(), "r");
    FILE *priv_key_in = fopen(privateFile.c_str(), "r");
    
    fseek(des_cipher_in, 0, SEEK_END);
    des_ciplen = ftell(des_cipher_in);
    fseek(des_cipher_in, 0, SEEK_SET);
    des_cip = (unsigned char *)malloc(des_ciplen);
    fread(des_cip, 1, des_ciplen, des_cipher_in);
    fclose(des_cipher_in);
    
    md = (unsigned char *)malloc(mdlen);
    md_ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(md_ctx, md_ptr, NULL);
    EVP_DigestUpdate(md_ctx, des_cip, des_ciplen);
    EVP_DigestFinal_ex(md_ctx, md, NULL);
    EVP_MD_CTX_destroy(md_ctx);
    
    sigkey = PEM_read_PrivateKey(priv_key_in, NULL, NULL, NULL);
    ctx = EVP_PKEY_CTX_new(sigkey, NULL);
    EVP_PKEY_sign_init(ctx);
    EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
    EVP_PKEY_sign(ctx, NULL, &siglen, md, mdlen);
    sig = (unsigned char *)OPENSSL_malloc(siglen);
    EVP_PKEY_sign(ctx, sig, &siglen, md, mdlen);
    
    FILE *sign_out = fopen("signature.txt", "w");
    fwrite(sig, 1, siglen, sign_out);
    fclose(sign_out);
    fclose(priv_key_in);
    
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(sigkey);
    free(md);
    free(des_cip);
    OPENSSL_free(sig);
}
    
//main function
int main(int argc, char **argv)
{
    string plaintextFile, sessionFile, publicFile, privateFile;
    string temp;
    
    for(int i = 0; i < argc; i++)
    {
        temp = argv[i];
        if(temp == "-in")
        {
            temp = argv[i + 1];
            plaintextFile = temp;
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
        else if(temp == "-priv")
        {
            temp = argv[i + 1];
            privateFile = temp;
        }
    }
    
    //creates an object for reading and writing bytes
    uint64_t key = decryptSessionKey(sessionFile, publicFile);
    uint64_t IV = random_64bit();
    
    ofstream iv("iv.txt");
    stringstream stream;
    stream << hex << IV;
    string result(stream.str());
    iv << result;
    iv.close();
    
    string ciphertextFile = "enc_" + plaintextFile;
    file f(key, IV);
    f.encrypt(plaintextFile, ciphertextFile);
    sign(privateFile, ciphertextFile);
    
    return 0;
}
        