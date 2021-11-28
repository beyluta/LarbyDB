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
            std::cout << "Inserting: " << value << std::endl;
            int hash = Hash(value);

            if (hash > size) {
                std::cout << "Allocating new memory" << std::endl;
                SLList* newArr = new SLList[hash];
                std::cout << "Getting old values" << std::endl;
                std::string* nodes = arr->ToArray();

                // std::string* listArray = arr->ToArray();
                // for (int i = 0; i < arr->GetLength(); i++) {
                //     newArr->Push(listArray[i]);
                // }
                // delete[] listArray;
                // delete[] arr;
                // arr = newArr;
                // delete[] newArr;

                delete[] nodes;
                delete[] newArr;
                size = hash;
            }
        }
};