#include "EntryPoint.h"

#ifdef GAME_APPLICATION

//#include <thread>

void init_renderer()
{
	Renderer* renderer = new Renderer();
	sf::RenderWindow* window = new sf::RenderWindow();
	CharacterController* characterController = new CharacterController();

	// create the window
	sf::ContextSettings contextSettings = sf::ContextSettings(32);
	window->create(sf::VideoMode(800, 600), "Software Renderer", sf::Style::Default, contextSettings);
	window->setVerticalSyncEnabled(true);
	window->setActive(true);
	//window->setMouseCursorVisible(false);

	// load resources, initialize the OpenGL states, ...
	renderer->window = window;
	renderer->hasFocus = true;
	renderer->Initilise();

	int width = window->getSize().x;
	int height = window->getSize().y;

	// run the main loop
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
				window->setMouseCursorVisible(true);
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
				window->setMouseCursorVisible(false);
			}
			else if (event.type == sf::Event::LostFocus)
			{
				renderer->hasFocus = false;
				window->setMouseCursorVisible(true);
			}
		}

		characterController->modelview = renderer->modelview;
		characterController->lightModelView = renderer->lightModelView;

		characterController->OnUpdate();

		if (renderer->hasFocus)
		{
			characterController->OnFocus();

			// Mouse position from center of screen.
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
			Vector2 mouse;
			mouse.y = (1.0f - (mousePosition.y / (float)window->getSize().y)) - 0.5f;
			mouse.x = ((mousePosition.x / (float)window->getSize().x)) - 0.5f;
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
		window->display();
	}

	delete characterController;
	delete renderer;
	delete window;
}

int main()
{
	// Load the game in the current thread.
	init_renderer();

	return 0;
}
#endif

#ifndef GAME_APPLICATION
IMPLEMENT_APP_CONSOLE(MainApp) // A macro that tells wxWidgets to create an instance of our application 

bool MainApp::OnInit()
{
	// Create an instance of our frame, or window 
	MainWin = new MainFrame(_("Hello World!"), wxDefaultPosition, wxSize(800, 600));
	MainWin->Show(true); // show the window 
	SetTopWindow(MainWin); // and finally, set it as the main window 

	return true;
}

void MainApp::CleanUp()
{
	//delete MainWin;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_Hello, MainFrame::OnExit) // Tell the OS to run MainFrame::OnExit when
EVT_MENU(MENU_New, MainFrame::NewFile)
EVT_MENU(MENU_Open, MainFrame::OpenFile)
EVT_MENU(MENU_Close, MainFrame::CloseFile)
EVT_MENU(MENU_Save, MainFrame::SaveFile)
EVT_MENU(MENU_SaveAs, MainFrame::SaveFileAs)
EVT_MENU(MENU_Quit, MainFrame::Quit)
END_EVENT_TABLE() // The button is pressed

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
	// normally we would initialize objects such as buttons and textboxes here 
	CreateStatusBar(2);

	//HelloWorld = new wxButton(this, BUTTON_Hello, _T("Hello World"), wxDefaultPosition, wxDefaultSize, 0);

	MainMenu = new wxMenuBar();
	FileMenu = new wxMenu();
	FileMenu->Append(MENU_New, wxT("&New"), wxT("Create a new file"));
	FileMenu->Append(MENU_Open, wxT("&Open"), wxT("Open an existing file"));
	FileMenu->Append(MENU_Close, wxT("&Close"), wxT("Close the current document"));
	FileMenu->Append(MENU_Save, wxT("&Save"), wxT("Save the current document"));
	FileMenu->Append(MENU_SaveAs, wxT("Save &As"), wxT("Save the current document under a new file name"));
	FileMenu->AppendSeparator();
	FileMenu->Append(MENU_Quit, "&Quit", "Quit the editor");
	
	MainMenu->Append(FileMenu, "&File");
	SetMenuBar(MainMenu);
	
	LoadGame();
}

void MainFrame::Tidy()
{
	delete FileMenu;
	delete MainMenu;

	if (canvas != nullptr)
	{
		delete canvas;
	}
}

void MainFrame::LoadGame()
{
	// Load SFML Canvas
	if (canvas != nullptr)
	{
		delete canvas;
	}

	auto size = this->GetSize();
	wxPoint p = wxPoint(50, 0);
	wxSize s = wxSize(size.x - 50, size.y);
	canvas = new MyCanvas(this, wxID_ANY, p, s);
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close(TRUE); // Tells the OS to quit running this process

	//std::exit(1);
}

void MainFrame::Quit(wxCommandEvent& event)
{
	Close(TRUE); // Tells the OS to quit running this process
}

void MainFrame::NewFile(wxCommandEvent& WXUNUSED(event))
{
	LoadGame();
}

void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event))
{

}

void MainFrame::CloseFile(wxCommandEvent& WXUNUSED(event))
{

}

void MainFrame::SaveFile(wxCommandEvent& WXUNUSED(event))
{

}

void MainFrame::SaveFileAs(wxCommandEvent& WXUNUSED(event))
{
}
#endif