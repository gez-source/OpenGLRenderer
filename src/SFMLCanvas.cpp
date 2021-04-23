#include "SFMLCanvas.h"

void wxSFMLCanvas::OnIdle(wxIdleEvent&)
{
    // Send a paint message when the control is idle, to ensure maximum framerate
    Refresh();
}
void wxSFMLCanvas::OnPaint(wxPaintEvent&)
{
    // Prepare the control to be repainted
    wxPaintDC Dc(this);

    // Let the derived class do its specific stuff
    OnUpdate();

    // Display on screen
    display();
}

void wxSFMLCanvas::OnEraseBackground(wxEraseEvent&)
{

}

wxSFMLCanvas::wxSFMLCanvas(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style) :
    wxControl(Parent, Id, Position, Size, Style)
{
#ifdef __WXGTK__

    // GTK implementation requires to go deeper to find the
    // low-level X11 identifier of the widget
    gtk_widget_realize(m_wxwindow);
    gtk_widget_set_double_buffered(m_wxwindow, false);
    GdkWindow* Win = GTK_PIZZA(m_wxwindow)->bin_window;
    XFlush(GDK_WINDOW_XDISPLAY(Win));
    sf::RenderWindow::Create(GDK_WINDOW_XWINDOW(Win));

#else

    // Tested under Windows XP only (should work with X11
    // and other Windows versions - no idea about MacOS)
    sf::RenderWindow::create(GetHandle());

#endif
}

wxSFMLCanvas::~wxSFMLCanvas()
{

}

void wxSFMLCanvas::OnUpdate()
{

}

BEGIN_EVENT_TABLE(wxSFMLCanvas, wxControl)
EVT_IDLE(wxSFMLCanvas::OnIdle)
EVT_PAINT(wxSFMLCanvas::OnPaint)
EVT_ERASE_BACKGROUND(wxSFMLCanvas::OnEraseBackground)
END_EVENT_TABLE() // The button is pressed

void MyCanvas::OnUpdate()
{
	// handle events
	sf::Event event;
	while (pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// end the program
			//running = false;
			setMouseCursorVisible(true);
		}
		else if (event.type == sf::Event::Resized)
		{
			// adjust the viewport when the window is resized
			renderer->OnResize(event.size.width, event.size.height);
		}
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			characterController->cameraTransform.Translate(Vector3(0, 0, event.mouseWheel.delta * 0.01f));
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			renderer->hasFocus = true;
			setMouseCursorVisible(false);
		}
		else if (event.type == sf::Event::LostFocus)
		{
			renderer->hasFocus = false;
			setMouseCursorVisible(true);
		}
	}

	characterController->modelview = renderer->modelview;
	characterController->lightModelView = renderer->lightModelView;

	characterController->OnUpdate();

	if (renderer->hasFocus)
	{
		characterController->OnFocus();

		// Mouse position from center of screen.
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*this);
		Vector2 mouse;
		mouse.y = (1.0f - (mousePosition.y / (float)this->getSize().y)) - 0.5f;
		mouse.x = ((mousePosition.x / (float)this->getSize().x)) - 0.5f;
		mouse = mouse * 10;
		renderer->lightDir.x = mouse.x;
		renderer->lightDir.y = mouse.y;
		renderer->lightDir.z = 1;
		//std::cout << "x: " << mouse.x << " y: " << mouse.y << std::endl;
	}
	renderer->modelview = characterController->modelview;
	renderer->lightModelView = characterController->lightModelView;
	renderer->worldPosition = characterController->worldPosition;

	// draw...
	renderer->Render(characterController->cameraTransform);

	// end the current frame (internally swaps the front and back buffers)
	//this->display();
}