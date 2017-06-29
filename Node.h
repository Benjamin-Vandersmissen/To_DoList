//
// Created by benji on 22.06.17.
//

#ifndef TO_DOLIST_NODE_H
#define TO_DOLIST_NODE_H

#include <iostream>

class Node {
private:
    std::string text;
    std::string title;
public:
    friend Node & operator>>(std::istream &stream, Node &n);
    friend std::ostream& operator<<(std::ostream& stream, Node& n);

    friend class ListNode;

    Node(const std::string &title, const std::string &text);
    Node(std::istream& stream);

    const std::string &getText() const;

    void setText(const std::string &text);

    const std::string &getTitle() const;

    void setTitle(const std::string &title);
};


#endif //TO_DOLIST_NODE_H