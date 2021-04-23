//#include "Editor.h"
//#include "IOHelpers.h"
//
//EditorWindow::EditorWindow(int w, int h, const char* name) : Fl_Double_Window(w, h, name)
//{
//	begin();
//	menu = new Fl_Menu_Bar(0, 0, 400, 30, "Menu system");
//	menu->add("&File/&Open", "^o", open_cb);
//	menu->add("&File/&Save", "^s", save_cb, 0, FL_MENU_DIVIDER);
//	menu->add("&File/&New Level", "^n", new_cb, 0, FL_MENU_DIVIDER);
//	menu->add("&File/&Quit", "^q", Window_Close);
//	menu->add("&Edit/Radio 1", 0, edit_cb, 0, FL_MENU_RADIO);
//	menu->add("&Edit/Radio 2", 0, edit_cb, 0, FL_MENU_RADIO | FL_MENU_DIVIDER);
//	menu->add("&Edit/Toggle 1", 0, edit_cb, 0, FL_MENU_TOGGLE);			// Default: off 
//	menu->add("&Edit/Toggle 2", 0, edit_cb, 0, FL_MENU_TOGGLE);			// Default: off
//	menu->add("&Edit/Toggle 3", 0, edit_cb, 0, FL_MENU_TOGGLE | FL_MENU_VALUE);	// Default: on
//	menu->add("&Help/About", 0, edit_cb);
//
//	but = new Fl_Button(10, 35, 100, 25, "Testing");
//	but->callback(MyFunction, this);
//
//	scroll = new EditorScroll(0, 60, 300, 300);
//	scroll->type(Fl_Scroll::BOTH_ALWAYS);
//	scroll->begin();
//
//	int n = 0;
//	//for (int y = 0; y < 16; y++) 
//	//{
//	//	for (int x = 0; x < 5; x++) 
//	//	{
//	//		char buf[20]; sprintf(buf, "%d", n++);
//	//		Fl_Button* b = new Fl_Button(x * 75, y * 25, 75, 25);
//	//		b->copy_label(buf);
//	//		b->color(n);
//	//		b->labelcolor(FL_WHITE);
//	//	}
//	//}
//
//	std::vector<std::string> files;
//	GetDirectoryFiles("Assets/test/Textures", files);
//
//	n = 0;
//	int x = 0;
//	int y = 0;
//	for (auto file : files)
//	{
//		auto ext = GetFileExtension(file);
//
//		ext = ToLowerCaseCopy(ext);
//
//		if (ext == ".png") 
//		{
//			x = (n % 4);
//			y = (n / 4);
//
//			image = new Fl_PNG_Image(file.c_str());
//
//			//mypicturebox = new Fl_Box((x * 32), (y * 32), 32, 32, file.c_str());
//			mypicturebox = new Fl_Button((x * 32), (y * 32), 32, 32, 0);
//			//mypicturebox->color(FL_WHITE);
//
//			mypicturebox->image(image);
//			mypicturebox->callback(MyFunction2, mypicturebox);
//			mypicturebox->set_visible_focus();
//			//mypicturebox->box(FL_FLAT_BOX);
//			//mypicturebox->image(image);
//			//mypicturebox->position(x * 32, y * 32);
//
//			//mypicturebox->redraw();
//			scroll->add(mypicturebox);
//			boxes.push_back(mypicturebox);
//
//			//x = n % 4;
//			//if (n > 0 && n % 4 == 0) 
//			//{
//			//	y++;
//			//}
//			//x++;
//			//if (n > 0 && n % 4 == 0) // (w / 32)
//			//{
//			//	y++;
//			//	x = 0;
//			//}
//
//			n++;
//		}
//
//	}
//	
//	scroll->end();
//
//	resizable(scroll);
//	callback(Window_Close);
//
//	end();
//	show();
//}
//
//EditorWindow::~EditorWindow()
//{
//	delete but;
//
//	but = nullptr;
//}
//
//void EditorWindow::Run()
//{
//	Fl::run();
//}