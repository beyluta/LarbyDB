#include "hashtable.h"

class Node {
    private:
        const char* data = nullptr;
        Node* next = nullptr;
    public:
        void Push(Node* node, const char* msg) {
            if (node->data == nullptr) {
                node->data = msg;
            } else if (node->next == nullptr && node->data != nullptr) {
                node->next = new Node();
                Push(node->next, msg);
            } else if (node->next != nullptr && node->data != nullptr) {
                Push(node->next, msg);
            }
        }

        const char* Peek() {
            return data;
        }

        void PrintAll() {
            if (data != nullptr) {
                std::cout << data << std::endl;
                if (next != nullptr) {
                    next->PrintAll();
                }
            }
        }
};

class Hashtable {
    private:
        Node** arr;
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
                
            }
        }
};