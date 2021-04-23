#ifndef SFML_CANVAS_H
#define SFML_CANVAS_H

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

class wxSFMLCanvas : public wxControl, public sf::RenderWindow
{
public:

    wxSFMLCanvas(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition,
        const wxSize& Size = wxDefaultSize, long Style = 0);

    virtual ~wxSFMLCanvas();

private:

    virtual void OnUpdate();

    void OnIdle(wxIdleEvent&);

    void OnPaint(wxPaintEvent&);

    void OnEraseBackground(wxEraseEvent&);

    DECLARE_EVENT_TABLE()
};


class MyCanvas : public wxSFMLCanvas
{
public:
	Renderer* renderer;
	CharacterController* characterController;

    MyCanvas(wxWindow* Parent,
        wxWindowID Id,
        wxPoint& Position,
        wxSize& Size,
        long       Style = 0) :
        wxSFMLCanvas(Parent, Id, Position, Size, Style)
    {
		renderer = new Renderer();
		characterController = new CharacterController();

		// load resources, initialize the OpenGL states, ...
		renderer->window = this;
		renderer->hasFocus = true;
		renderer->Initilise();
    }
    ~MyCanvas()
    {
        delete characterController;
        delete renderer;
    }
private:

    virtual void OnUpdate();
};

#endif