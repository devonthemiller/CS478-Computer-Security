#include "des_cbc.h"
using namespace std;

//default constructor
DES_CBC::DES_CBC(uint64_t key, uint64_t IV) : des(key), IV(IV), endBlock(IV)
{
}

//encrypts a given block using DES in CBC mode
uint64_t DES_CBC::CBCencrypt(uint64_t block)
{
    endBlock = des.encrypt(block ^ endBlock);
    return endBlock;
}

//decrypts a given block using DES in CBC mode
uint64_t DES_CBC::CBCdecrypt(uint64_t block)
{
    uint64_t decryption = des.decrypt(block) ^ endBlock;
    endBlock = block;
    return decryption;
}

//resets CBC mode
void DES_CBC::resetCBC()
{
    endBlock = IV;
}
