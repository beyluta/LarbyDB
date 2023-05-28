#include "linknode.h"

void LinkedList::Add(LinkNode *head, std::string value) {
    while (true) {
        if (head->value == "") {
            head->value = value;
            break;
        } else if (head->next == nullptr) {
            LinkNode *node = new LinkNode();
            node->value = value;
            head->next = node;
            break;
        }
        head = head->next;
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