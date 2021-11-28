#include "hashtable.h"

class Hashtable {
    private:
        SLList* arr;
        int size = 0;
    public:
        int Hash(const char* value) {
            int sum = 0;
            for (int i = 0; i < strlen(value); i++) {
                sum += (sum << 5) - sum + (int)value[i];
            }
            return sum;
        }

        void Push(const char* value) {
            int hash = Hash(value);
            if (hash > size) {
                
                size = hash;
            }
        }
};