#include <iostream>
#include <vector>

class LinkNode 
{
public:
    std::string value;
    LinkNode *next;
};

namespace LinkedList 
{
    void add(LinkNode *head, std::string value);
    std::vector<std::string> list(LinkNode *head);
    void unlink(LinkNode *head);
};