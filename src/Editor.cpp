#include "Editor.h"

void MyFunction(Fl_Widget* w, void* data)
{
	exit(1);
}

Editor::Editor()
{
	std::cout << "Loading level editor..." << std::endl;
	win = new Fl_Window(400, 200, "Hello world");
	but = new Fl_Button(10, 0, 100, 25, "Testing");
	but->callback(MyFunction);
	win->show();
	Fl::run();
}

Editor::~Editor()
{
	delete win;
	delete but;
}