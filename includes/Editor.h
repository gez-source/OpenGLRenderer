#ifndef EDITOR_H
#define EDITOR_H

#include <iostream>

#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Button.H"



class Editor
{
	Fl_Window* win;
	Fl_Button* but;
public:
	Editor();
	~Editor();
};


#endif