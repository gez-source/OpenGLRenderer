#ifndef __BASE_H 
// Make sure to only declare these classes once 
#define __BASE_H 

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif 
#include <wx/frame.h>

#ifdef __WXGTK__
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <wx/gtk/win_gtk.h>
#endif

#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CharacterController.h"
#include "Editor.h"

#include "SFMLCanvas.h"

class MainFrame;

class MainApp : public wxApp // MainApp is the class for our application 
{
private:
	MainFrame* MainWin;
	// MainApp just acts as a container for the window, or frame in MainFrame
public:
	virtual bool OnInit();
	virtual void CleanUp();
};

class MainFrame : public wxFrame // MainFrame is the class for our window, 
{
	// It contains the window and all objects in it 
public:
	MyCanvas* canvas;
	wxButton* HelloWorld;
	wxMenuBar* MainMenu;
	wxMenu* FileMenu;

	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

	void Tidy();
	void LoadGame();
	void OnExit(wxCommandEvent& event);
	void Quit(wxCommandEvent& event);
	void NewFile(wxCommandEvent& event);
	void OpenFile(wxCommandEvent& event);
	void SaveFile(wxCommandEvent& event);
	void SaveFileAs(wxCommandEvent& event);
	void CloseFile(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};

enum
{
	BUTTON_Hello = wxID_HIGHEST + 1, // declares an id which will be used to call our button
	//TEXT_Main = wxID_HIGHEST + 1, // declares an id which will be used to call our button
	MENU_New,
	MENU_Open,
	MENU_Close,
	MENU_Save,
	MENU_SaveAs,
	MENU_Quit
};
DECLARE_APP(MainApp)

#endif