//
// Created by benji on 22.06.17.
//

#ifndef TO_DOLIST_TO_DOLIST_H
#define TO_DOLIST_TO_DOLIST_H

#include <vector>
#include <fstream>
#include "Node.h"
#include "UsefulFunctions.h"

class ToDoList {
private:
    std::vector<Node*> nodes;
public:
    friend ToDoList operator>>(std::istream& stream, ToDoList& list);

    friend class ToDoListWindow;

    friend bool operator==(ToDoList &list1, ToDoList &list2);

    ToDoList(std::istream& istream);

    ToDoList();

    bool add(Node *node);

    void output(std::string directory = "./");

    void swap(int a, int b);

    void moveUp(int a);

    void moveDown(int a);

    void moveToTop(int a);

    void moveToBottom(int a);

    std::vector<Node *> getNodes() const;
};


#endif //TO_DOLIST_TO_DOLIST_H
