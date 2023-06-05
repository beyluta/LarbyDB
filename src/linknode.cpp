#include "linknode.h"

void LinkedList::Add(LinkNode *head, std::string value) {
    if (head->last == nullptr) {
        head->last = head;
    }

    if (head->last->value == "") {
        head->last->value = value;
        return;
    } else {
        LinkNode *node = new LinkNode();
        node->value = value;
        head->last->next = node;
    }
}

std::vector<std::string> LinkedList::List(LinkNode *head) {
    std::vector<std::string> list;
    while (head != nullptr) {
        if (head->value != "") {
            list.push_back(head->value);
        }
        head = head->next;
    }
    return list;
}

void LinkedList::Unlink(LinkNode *head) {
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