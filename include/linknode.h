#include <iostream>
#include <vector>

class LinkNode
{
public:
    std::string value;
    LinkNode *last;
    LinkNode *next;
};

namespace LinkedList
{
    void Add(LinkNode *head, std::string value);
    std::vector<std::string> List(LinkNode *head);
    void Unlink(LinkNode *head);
    int Length(LinkNode *head);
};