//
// Created by benji on 22.06.17.
//

#ifndef TO_DOLIST_WINDOW_H
#define TO_DOLIST_WINDOW_H

#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Select_Browser.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Button.H"
#include "FL/names.h"
#include "ToDoList.h"
#include "UsefulFunctions.h"
#include <sys/stat.h>
#include <algorithm>

class ListNode : public Fl_Box{
private:
    Node* node;

    bool drawDescription = false;

    int origW, origH, origX, origY;
public:
    friend class ToDoListWindow;

    ListNode(int x, int y, Node* node);

    int handle(int event);

    void draw();

    void updateSize();

    void swap();
};


class ToDoListWindow : public Fl_Double_Window{
private:
    ToDoList* list;
    std::vector<ListNode*> listnodes;

    static void saveListCB(Fl_Widget* w);

    static void addCB(Fl_Widget* w);

    static void quitWindowCB(Fl_Widget* w);
public:
    ToDoListWindow(int w, int h, const char* l = 0);

    friend class ListNode;

    Fl_Button* saveButton = NULL;

    Fl_Button* addButton = NULL;

    void addNode(std::string title, std::string text);

    bool unsavedChanges();
};


std::pair<double, double> dimensions(std::string s);

#endif //TO_DOLIST_WINDOW_H
