cmake_minimum_required(VERSION 3.6)
project(To_DoList)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Node.cpp Node.h ToDoList.cpp ToDoList.h ToDoListWindow.cpp ToDoListWindow.h UsefulFunctions.cpp UsefulFunctions.h)

include_directories(FLTK)
link_directories(usr/local/lib)
add_executable(To_DoList ${SOURCE_FILES})
target_link_libraries(To_DoList fltk_images fltk X11 Xext pthread Xfixes dl m)