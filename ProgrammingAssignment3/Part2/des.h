#include <cstdint>
using namespace std;

//DES class instantiates an object which handles
//all of the operations, permuations, and substitutions
//that are required for DES.
class DES
{
    private:
        uint64_t subKey[16]; //list of all generated subkeys
        
    public:
        DES(uint64_t key);
        
        //main encryption+decryption functions
        uint64_t encrypt(uint64_t block);
        uint64_t decrypt(uint64_t block);
        
        //subkey function
        void generateSubKeys(uint64_t key);
        
        //permutation functions
        uint64_t initialPerm(uint64_t block);
        uint64_t finalPerm(uint64_t block);
        
        //round function functions
        void switchHalves(uint32_t &left, uint32_t &right, uint32_t F);
        uint32_t function(uint32_t right, uint64_t key);
        uint64_t expand(uint32_t right);
        uint32_t round(uint32_t s);
};
        