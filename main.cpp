#include <iostream>
#include "ToDoList.h"
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "ToDoListWindow.h"

int main() {
    ToDoListWindow* win = new ToDoListWindow(1920,1080);

    return Fl::run();
}