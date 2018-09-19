#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
using namespace std;

cipher::cipher()
{
    ciphertext = "";
    frequency = "";
    key = "";
    plaintext = "";
}

void cipher::readCiphertext(const string & file)
{
    string line;
    ifstream readFile;
    readFile.open(file.c_str());
    if(readFile.is_open())
    {
        while(!readFile.eof())
        {
            getline(readFile, line);
            ciphertext.append(line);
        }
        readFile.close();
    }
}

int cipher::compare(const string & file)
{
    int match = 0;
    string line;
    int check;
    ifstream readFile;
    readFile.open(file.c_str());
    if(readFile.is_open())
    {
        while(!readFile.eof())
        {
            getline(readFile, line);
            const char * a = plaintext.c_str();
            const char * b = line.c_str();
            if(strstr(a, b) != NULL)
            {
                match;
            }
        }
        readFile.close();
        return match;
    }
}

void cipher::calculateFreq()
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<int> freq;
    int count;
    
    for(int i = 0; i < 26; i++)
    {
        count = 0;
        for(int j = 0; j < ciphertext.length(); j++)
        {
            if(ciphertext[j] == alphabet[i])
            {
                count++;
            }
        }
        freq.push_back(count);
    }
    
    int temp, max, zeroes;
    zeroes = 0;
    
    for(int i = 0; i < 26; i++)
    {
        if(freq[i] == 0)
        {
            zeroes++;
        }
    }
    
    for(int i = 0; i < 26 - zeroes; i++)
    {
        temp = 0;
        max = 0;
        for(int j = 0; j < freq.size(); j++)
        {
            if(freq[j] > max)
            {
                temp = j;
                max = freq[j];
            }
        }
        
        frequency.append(string(1, alphabet[temp]));
        freqNum.push_back(max);
        alphabet.erase(alphabet.begin() + temp);
        freq.erase(freq.begin() + temp);
    }
    
    frequency.append(alphabet);
    for(int i = 0; i < zeroes; i++)
    {
        freqNum.push_back(0);
    }
}

void cipher::generateKey()
{
    size_t pos;
    key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string key2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string sub = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    
    for(int i = 0; i < 26; i++)
    {
        pos = key2.find(frequency.at(i));
        key[pos] = sub[i];
        key2[pos] = ' ';
    }
}

void cipher::permuteKey()
{
    int rand_int1 = 0;
    int rand_int2 = 0;
    int range = 99;
    
    while(rand_int1 == rand_int2 || range > 8)
    {
        rand_int1 = rand() % 26;
        rand_int2 = rand() % 26;

	char a = key.at(rand_int1);
	char b = key.at(rand_int2);
	
	size_t pos1 = frequency.find(a);
	size_t pos2 = frequency.find(b);

	int a1 = freqNum[pos1];
	int b1 = freqNum[pos2];

	range = abs(a1 - b1);	
    }
    
    char temp = key.at(rand_int1);
    key[rand_int1] = key[rand_int2];
    key[rand_int2] = temp;
}

void cipher::decipher()
{
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char a;
    char b;
    
    plaintext = ciphertext;
    
    for(int i = 0; i < 26; i++)
    {
        a = key.at(i);
        b = alphabet.at(i);
        
        for(int j = 0; j < ciphertext.length(); j++)
        {
            if(ciphertext[j] == b)
            {
                plaintext[j] = a;
            }
        }
    }
}

void cipher::setKey(string k)
{
    key = k;
}

string cipher::getKey()
{
    return key;
}

string cipher::getFrequency()
{
    return frequency;
}

vector<int> cipher::getFreqNum()
{
    return freqNum;
}

string cipher::getCiphertext()
{
    return ciphertext;
}

string cipher::getPlaintext()
{
    return plaintext;
}

    
