//
// Created by benji on 22.06.17.
//

#include "Node.h"

Node & operator>>(std::istream &stream, Node &n) {
    getline(stream, n.title);
    std::string a;
    while(getline(stream, a)){
        n.text += a + "\n";
    }
    return n;
}

std::ostream &operator<<(std::ostream &stream, Node &n) {
    stream << n.title << std::endl;
    stream << n.text;
    return stream;
}

Node::Node(const std::string &title, const std::string &text) : text(text), title(title) {}

Node::Node(std::istream &stream) {
    stream >> (*this);
}

const std::string &Node::getText() const {
    return text;
}

void Node::setText(const std::string &text) {
    Node::text = text;
}

const std::string &Node::getTitle() const {
    return title;
}

void Node::setTitle(const std::string &title) {
    Node::title = title;
}
