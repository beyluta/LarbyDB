#include "linknode.h"

void LinkedList::add(LinkNode *head, std::string value)
{
    if (head->value == "")
    {
        head->value = value;
        return;
    }

    if (head->next != nullptr)
    {
        add(head->next, value);
    } else {
        LinkNode *node = new LinkNode();
        node->value = value;
        head->next = node;
    }
}

std::vector<std::string> LinkedList::list(LinkNode *head) {
    std::vector<std::string> list;
    while (head != nullptr) {
        if (head->value != "") {
            list.push_back(head->value);
        }
        head = head->next;
    }
    return list;
}

void LinkedList::unlink(LinkNode *head) {
    std::vector<LinkNode*> nodePtrs;
    while (head != nullptr) {
        nodePtrs.push_back(head);
        head = head->next;
    }

    for (int i = 1; i < nodePtrs.size(); i++) {
        delete nodePtrs.at(i);
    }

    nodePtrs.clear();
}