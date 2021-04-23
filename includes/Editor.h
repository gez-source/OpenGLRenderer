//#ifndef EDITOR_H
//#define EDITOR_H
//
//#include <iostream>
//#include <string>
//#include <vector>
//
//#include "FL/Fl.H"
//#include "FL/Fl_Window.H"
//#include "FL/Fl_Double_Window.H"
//#include "FL/Fl_Button.H"
//#include "FL/Fl_Menu_Bar.H"
//#include "FL/Fl_Scroll.H"
//#include "FL/Fl_PNG_Image.H"
//#include "FL/Fl_Box.H"
//
//class EditorScroll : public Fl_Scroll
//{
//public:
//	EditorScroll(int x, int y, int w, int h, const char* label = nullptr) : Fl_Scroll(x, y, w, h, label)
//	{
//		
//	}
//protected:
//	//void draw() override
//	//{
//	//	Fl_Scroll::draw();
//
//	//	int size = this->children();
//	//	for (int i = 0; i < size; i++)
//	//	{
//	//		Fl_Widget* ch = this->child(i);
//
//	//		//ch->draw();
//	//		//ch->redraw();
//	//	}
//
//	//}
//};
//
//class EditorWindow : public Fl_Double_Window
//{
//private:
//	Fl_Button* but;
//	Fl_Menu_Bar* menu;
//	EditorScroll* scroll;
//	Fl_Button* mypicturebox;
//	Fl_PNG_Image* image;
//	std::vector<Fl_Button*> boxes;
//public:
//	EditorWindow(int w, int h, const char* name = nullptr);
//	~EditorWindow();
//	void Run();
//
//	static void MyFunction2(Fl_Widget* w, void* data)
//	{
//		Fl_Button* button = (Fl_Button*)data;
//		//button->scroll->redraw();
//		//button->resize(button->x(), button->y(), button->w(), button->h());
//		
//	}
//	static void MyFunction(Fl_Widget* w, void* data)
//	{
//		EditorWindow* editor = (EditorWindow*)data;
//		for (auto box : editor->boxes)
//		{
//			box->redraw();
//		}
//		//std::exit(1);
//	}
//	static void Window_Close(Fl_Widget* w, void* data)
//	{
//		//EditorWindow* editorWindow = (EditorWindow*)data;
//		//editorWindow->hide();
//		//Fl::delete_widget(editorWindow);
//		//delete editorWindow; // BUG: heap corrupted
//		
//		std::cout << "EXIT!" << std::endl;
//
//		std::exit(1);
//	}
//
//	static void new_cb(Fl_Widget* w, void* data)
//	{
//		std::cout << "TEST" << std::endl;
//	}
//	static void open_cb(Fl_Widget* w, void* data)
//	{
//		std::cout << "TEST" << std::endl;
//	}
//	static void save_cb(Fl_Widget* w, void* data)
//	{
//		std::cout << "TEST" << std::endl;
//	}
//	static void close_cb(Fl_Widget* w, void* data)
//	{
//		std::cout << "TEST" << std::endl;
//	}
//	static void edit_cb(Fl_Widget* w, void* data)
//	{
//		std::cout << "TEST" << std::endl;
//	}
//};
//
//#endif