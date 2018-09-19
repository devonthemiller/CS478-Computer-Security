#include "file.h"
#include <bitset>
#include <cstdio>

//default constructor
file::file(uint64_t key, uint64_t IV) : des(key, IV)
{
}

//function for encrypting data given by inputFile
int file::encrypt(string inputFile, string outputFile)
{
    return operation(inputFile, outputFile, false);
}

//function for decrypting data given by inputFile
int file::decrypt(string inputFile, string outputFile)
{
    return operation(inputFile, outputFile, true);
}

//main function that reads, encrypts/decrypts, and then writes data
int file::operation(string inputFile, string outputFile, bool mode)
{
    ifstream readFile;
    ofstream writeFile;
    uint64_t buff = 0;
    
    readFile.open(inputFile, ios::binary | ios::in | ios::ate);
    writeFile.open(outputFile, ios::binary | ios::out);
    
    if(!readFile)
    {
        cout << "Error: Input file not found." << endl;
        return 0;
    }
    
    uint64_t size = readFile.tellg();
    readFile.seekg(0, ios::beg);
    
    char * c = new char[1];
    unsigned char character;
    uint8_t cha;
    uint64_t block = size / 8;
    //if(mode) block --;
    
    for(uint64_t i = 0; i < block; i++)
    {
    	for(int j = 0; j < 8; j++)
    	{
                buff = buff << 8;
                readFile.read(c,1);
                character = *c;
                cha = (unsigned int) character;
                buff = buff | (((uint64_t) cha) & 0x00000000000000ff);
    	}
        
        if(mode)
            buff = des.CBCdecrypt(buff); //decrypt
        else
            buff = des.CBCencrypt(buff); //encrypt
        
    	for(int j = 7; j >= 0; j--)
    	{
                cha = (buff >> (j * 8)) & 0x00000000000000ff;
                character = (char) cha;
                c[0] = character;
                writeFile.write(c,1);
    	}
    }
    
    //if encrypting then add necessary padding to message
    if(mode == false)
    {
        uint8_t pad = 8 - (size % 8);
        
        if(pad == 8)
            pad = 0;
        
        buff = (uint64_t) 0;
        c[0] = 0;
        for(uint8_t i = 0; i < pad; i++)
        {
            writeFile.write(c,1);
        }
    }
    
    
    readFile.close();
    writeFile.close();
    return 0;
}
