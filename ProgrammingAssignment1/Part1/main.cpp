#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    srand(time(NULL));
    
    string cipherfile;
    string dictionaryfile;
    string s;
    string response;
    string key;
    string result;
    vector<int> vals;
    
    cout << "Enter the filename for the cipher text file you want to use: ";
    cin >> cipherfile;
    cout << "Enter the filename for the dictionary file you want to use: ";
    cin >> dictionaryfile;
    
    cipher c = cipher();
    c.readCiphertext(cipherfile);
    
    c.calculateFreq();
    s = c.getFrequency();
    vals = c.getFreqNum();
    c.generateKey();
    c.decipher();
    
    cout << "Frequency analysis of each character in the cipher: " << endl;
    for(int i = 0; i < 26; i++)
    {
        cout << s[i] << ": " << vals[i] << endl;
    }
    cout << endl;
    
    cout << "Enter 1 to go straight to manual cryptanalysis or enter 2 to try and generate an approximate key." << endl;
    cin >> response;
    if(response == "2")
    {
        int match = c.compare(dictionaryfile);
        int temp_match;
        cipher temp = cipher();
        
        for(int i = 0; i < 100; i++)
        {
	    if(i % 10 == 0)
	    {
		cout << " . ";
	    }

            temp = c;
            temp.permuteKey();
            temp.decipher();
            temp_match = temp.compare(dictionaryfile);
            
            if(temp_match > match)
            {
                c.setKey(temp.getKey());
                c.decipher();
                match = temp_match;
            }
        }
        
	cout << endl;
        cout << "Best possible key found: " << c.getKey() << endl;
        cout << "Resulting plaintext: " << endl << c.getPlaintext() << endl;
        
    }

    if(response == "1")
    {
        cout << "Default key: " << c.getKey() << endl;
        cout << "Resulting plaintext: " << endl << c.getPlaintext() << endl;
    }

    response = "1";

    while(response == "1")
    {
	char a;
	char b;
	
	cout << "Enter a character to substitute (case sensitive): ";
        cin >> a;
	cout << "Enter a character to substitute with (case sensitive): ";
	cin >> b;

	key = c.getKey();
	size_t pos1 = key.find(a);
	size_t pos2 = key.find(b);
	char temp = key.at(pos1);
	key[pos1] = key[pos2];
	key[pos2] = temp;
	
        c.setKey(key);
        c.decipher();
        
        result =  c.getPlaintext();
        
	cout << endl;
        cout << "Result from substitution: " << endl;
        cout << result << endl;
	
	cout << "Enter 1 to continue manual cryptanalysis or enter 2 to stop." << endl;
        cin >> response;
    }

    cout << "Final key: " << key << endl;
    cout << "Final message: " << result << endl;
    
    return 0;
    
}
            
