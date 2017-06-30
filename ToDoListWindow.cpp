//
// Created by benji on 22.06.17.
//

#include "ToDoListWindow.h"

static int SEPERATION = 8;
static Fl_Double_Window* addNodeWindow = NULL;

static Fl_Input* titleInput = NULL;
static Fl_Text_Editor* descriptionEditor = NULL;
static Fl_Text_Buffer* buffer = NULL;
static Fl_Button* submitButton = NULL;
static ListNode* editNode = NULL;

ToDoListWindow::ToDoListWindow(int w, int h, const char *l) : Fl_Double_Window(w, h, l) {
    fl_font(0, 14);
    this->color(FL_WHITE);
    this->list = new ToDoList();
    if (fileExists(".list")){
        std::ifstream list(".list");
        list >> *this->list;
        int height = 0;
        for(Node* node : this->list->nodes){
            if (listnodes.size() == 0)
                listnodes.push_back(new ListNode(80,50,node));
            else{
                height += listnodes.back()->h() + 10;
                listnodes.push_back(new ListNode(80, 50 + height, node));
            }
        }
    }
    this->saveButton = new Fl_Button(10,50,60,32, "Save");
    this->saveButton->callback(saveListCB);

    this->addButton = new Fl_Button(10,90,60,32, "Add");
    this->addButton->callback(addCB);

    this->callback(quitWindowCB);

    this->end();
    this->show();
}

std::pair<double, double> dimensions(std::string s) {
    std::pair<double,double> dim;
    double maxWidth = -1;
    double height = fl_height();
    int index = 0;

    for(char c : s){
        if (c=='\n')
            height += fl_height();
    }
    for(std::string a; true; a = s.substr(index, s.find('\n', index)-index)){
        if (a.size() == 0)
            continue;
        index += a.size();
        maxWidth = std::max(maxWidth, fl_width(a.c_str()));
        index++;
        if (index >= s.size())
            break;
    }
    dim.first = maxWidth;
    dim.second = height;
    return dim;
}

void addNodeWindowCB(Fl_Widget *w) {
    addNodeWindow = NULL;
    Fl::delete_widget(w);
}

void ToDoListWindow::submitCB(Fl_Widget *w, void *v) {
    ToDoListWindow* win = (ToDoListWindow*) v;
    const char* text= titleInput->value();
    if (text == NULL)
        return;
    if (editNode == NULL)
        win->addNode(titleInput->value(), buffer->text());
    else{
        editNode->node->setText(buffer->text());
        editNode->node->setTitle(titleInput->value());
        std::pair<int, int> dims = dimensions(titleInput->value());
        editNode->origW = dims.first; //only the width needs to be updated, because the title is always a single line
        editNode = NULL;
    }
    w->window()->do_callback();
    win->redraw();
}

ListNode::ListNode(int x, int y, Node *node) : Fl_Box(x, y, 0 , 0), node(node), origX(x), origY(y){
    this->box(FL_BORDER_BOX);
    std::pair<int, int> dims = dimensions(node->title);
    origW = dims.first+SEPERATION;
    origH = dims.second+SEPERATION;
    this->size(origW, origH);
    this->redraw();
}

int ListNode::handle(int event) {
    static int offsets[2] = {0,0};

    switch (event){
        case FL_ENTER: {
            drawDescription = 1;
            std::pair<int, int> dims = dimensions(node->text);
            updateSize();
            this->redraw();
            return 1;
        }
        case FL_MOVE:{
            if (Fl::event_x() <= x()+origW + SEPERATION && Fl::event_y() > y()+origH && drawDescription == true){
                drawDescription = 0;
                updateSize();
                this->window()->redraw();
                return 1;
            }
            break;
        }
        case FL_LEAVE: {
            drawDescription = 0;
            updateSize();
            this->window()->redraw();
            return 1;
        }
        case FL_PUSH:{
            switch(Fl::event_key()) {
                case 65257: //LEFT MOUSE
                    if (Fl::event_clicks() == 0) {
                        drawDescription = 0;
                        updateSize();
                        this->window()->insert(*this,
                                               this->window()->children()); //ensure this widget gets drawn on top
                        this->window()->redraw();
                        offsets[0] = x() - Fl::event_x();
                        offsets[1] = y() - Fl::event_y();
                        return 1;
                    }else if (Fl::event_clicks() == 1){
                        ToDoListWindow* win = (ToDoListWindow*) window();
                        win->addButton->do_callback();
                        titleInput->value(node->title.c_str());
                        buffer->text(node->text.c_str());
                        editNode = this;
                        addNodeWindow->take_focus();
                        return 0;
                    }
                default:
                    return 0;
            }
        }
        case FL_DRAG:{
            this->position(Fl::event_x()+offsets[0], Fl::event_y()+offsets[1]);
            this->window()->redraw();
            return 1;
        }
        case FL_RELEASE:{
            swap();
            this->window()->redraw();

            return 1;
        }
        default:
            return Fl_Box::handle(event);
    }
    return Fl_Box::handle(event);
}

void ListNode::draw() {
    fl_draw_box(box(),x(),y(),origW,origH,color());
    fl_color(FL_BLACK);
    fl_draw(node->title.c_str(),x(),y(), origW, origH, align(), image(), 1);
    if(drawDescription){
        std::pair<int, int> dims = dimensions(node->text);
        fl_draw_box(box(),x()+origW+2,y(), w()-origW, dims.second, FL_RED);
        fl_color(FL_BLACK);
        fl_draw(node->text.c_str(),x()+origW+2+SEPERATION/2,y(), w()-origW, dims.second, FL_ALIGN_LEFT | FL_ALIGN_INSIDE, image(), 1);
    }
    else{
        if (w() != origW || h() != origH){
            this->size(origW, origH);
        }
    }
}

void ListNode::updateSize() {
    if (drawDescription){
        std::pair<int,int> dims = dimensions(node->text);
        if (h() == origH && w() == origW){
            size(origW+dims.first+2+SEPERATION,std::max(origH, dims.second));
        }
    }else{
        if (h() != origH || w() != origW)
            size(origW,origH);
    }
}

void ListNode::swap() {
    /**
     * ONLY TO BE CALLED INSIDE HANDLE OR CALLBACK FUNCTIONS!!
     * **/
    ToDoListWindow* win = (ToDoListWindow*) window();
    ListNode* secondNode = NULL;
    for(ListNode* node : win->listnodes){
        if (node == this)
            continue;
        bool condition = Fl::event_x() > node->x() && Fl::event_x() < node->x()+node->w() && Fl::event_y() > node->y() && Fl::event_y() < node->y()+node->h();
        int centerX, centerY;
        centerX = node->x() + node->w()/2;
        centerY = node->y() + node->h()/2;
        condition = condition || (centerX > x() && centerX < x() + w() && centerY > y() && centerY < y() + h());
        if (condition){
            secondNode = node;
        }
    }

    if (secondNode == NULL ) {
        position(origX,origY);
        return;
    }

    int tempX,tempY;
    tempY = secondNode->origY;
    tempX = secondNode->origX;

    secondNode->position(origX,origY);
    secondNode->origX = origX;
    secondNode->origY = origY;

    origX = tempX;
    origY = tempY;
    position(tempX,tempY);

    int index1, index2;
    std::vector<Node*> nodes = win->list->getNodes();
    index1 = std::distance(nodes.begin(),std::find(nodes.begin(), nodes.end(), node));
    index2 = std::distance(nodes.begin(),std::find(nodes.begin(), nodes.end(), secondNode->node));
    win->list->swap(index1, index2);
}


void ToDoListWindow::saveListCB(Fl_Widget *w) {
    ToDoListWindow* win = (ToDoListWindow*) w->window();
    win->list->output(".nodes/");
}

void ToDoListWindow::addCB(Fl_Widget *w) {
    ToDoListWindow* win = (ToDoListWindow*) w->window();
    if (addNodeWindow != NULL) {
        addNodeWindow->do_callback();
    }
        addNodeWindow = new Fl_Double_Window(200, 200, 500, 500);
        addNodeWindow->callback(addNodeWindowCB);
        titleInput = new Fl_Input(150,50,200,32,"Title");
        titleInput->align(FL_ALIGN_TOP);
        descriptionEditor = new Fl_Text_Editor(40,150,400,200,"Decription");
        buffer = new Fl_Text_Buffer();
        submitButton = new Fl_Button(200,400,100,32, "Submit");
        submitButton->callback(submitCB, win);
        buffer->text("");
        descriptionEditor->buffer(buffer);
        addNodeWindow->show();
}

void ToDoListWindow::addNode(std::string title, std::string text) {
    Node* node = new Node(title, text);
    bool success = this->list->add(node);
    if (!success)
        return;
    int height = 0;
    for(ListNode* node: listnodes){
        height+= node->h() + 10;
    }
    ListNode* LNode = new ListNode(80, 50 + height, node);
    this->add(LNode);
    this->listnodes.push_back(LNode);
}

void ToDoListWindow::quitWindowCB(Fl_Widget *w) {
    ToDoListWindow* win = (ToDoListWindow*) w;
    bool unsavedChanges = win->unsavedChanges();
    if (unsavedChanges){
        int a = fl_choice("There are unsaved changes. Do you really want to quit?", "Yes", "No",0);
        if (a == 0){
            if (addNodeWindow != NULL)
                addNodeWindow->hide();
            win->hide();
        }
    }
    else{
        if (addNodeWindow != NULL)
            addNodeWindow->hide();
        win->hide();
    }

}

bool ToDoListWindow::unsavedChanges() {
    ToDoList * list = new ToDoList;
    std::ifstream stream(".list");
    stream >> *list;
    return !(*list == *(this->list));
}
