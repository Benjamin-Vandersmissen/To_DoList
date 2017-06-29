//
// Created by benji on 22.06.17.
//

#include "ToDoList.h"

ToDoList operator>>(std::istream &stream, ToDoList &list) {
    std::string filename;
    while(getline(stream, filename)){
        std::ifstream file(filename);
        Node* n = new Node(file);
        list.nodes.push_back(n);
    }
    return list;
}

ToDoList::ToDoList(std::istream &stream) {
    stream >> *this;
}

ToDoList::ToDoList() {}

void ToDoList::output(std::string directory) {
    std::ofstream list(".list");
    if (!directoryExists(directory))
        system(("mkdir " + directory).c_str());
    for(Node* n : nodes){
        std::string fname = directory + n->getTitle();
        list << fname << std::endl;
        std::ofstream nodeOutput(fname);
        nodeOutput << *n;
        nodeOutput.close();
    }
}

void ToDoList::swap(int a, int b) {
    if (a < 0 || a >= nodes.size() || b < 0 || b >= nodes.size()){
        return;
    }
    Node* n1 = nodes[a];
    Node* n2 = nodes[b];
    nodes[a] = n2;
    nodes[b] = n1;
}

void ToDoList::moveUp(int a) {
    swap(a, a - 1);
}

void ToDoList::moveDown(int a) {
    swap(a, a + 1);

}

void ToDoList::moveToTop(int a) {
    swap(a, 0);
}

void ToDoList::moveToBottom(int a) {
    swap(a, nodes.size() - 1);
}

 std::vector<Node *> ToDoList::getNodes() const {
    return nodes;
}

bool ToDoList::add(Node *node) {
    for(Node* n : nodes){
        if(n->getTitle() == node->getTitle())
            return false;
    }
    nodes.push_back(node);
    return true;
}

bool operator==(ToDoList &list1, ToDoList &list2) {
    if (list1.nodes.size() != list2.nodes.size())
        return false;
    for(int i = 0; i < list1.nodes.size(); i++){
        if (list1.nodes[i]->getText() != list2.nodes[i]->getText() || list1.nodes[i]->getTitle() != list2.nodes[i]->getTitle())
            return false;
    }
    return true;
}


