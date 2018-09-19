#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "des_cbc.h"

//File class simply provides a separatemodule for reading 
//and writing the bytes from the input and output files.
//This is kept separate from the main file for added
//readability and intuitiveness.
class file
{
    private:
        DES_CBC des;
        
    public:
        file(uint64_t key, uint64_t IV);
        int encrypt(string inputFile, string outputFile);
        int decrypt(string inputFile, string outputFile);
        int operation(string inputFile, string outputFile, bool mode);
};
        