#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "CharacterController.h"


int main()
{
	Renderer renderer;
	sf::RenderWindow window;
	CharacterController characterController;

	// create the window
	sf::ContextSettings contextSettings = sf::ContextSettings(32);
	window.create(sf::VideoMode(800, 600), "Software Renderer", sf::Style::Default, contextSettings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	window.setMouseCursorVisible(false);

	// load resources, initialize the OpenGL states, ...
	renderer.window = &window;
	renderer.hasFocus = true;
	renderer.Initilise();

	int width = window.getSize().x;
	int height = window.getSize().y;

	// run the main loop
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
				window.setMouseCursorVisible(true);
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				renderer.OnResize(event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				characterController.cameraTransform.Translate(Vector3(0, 0, event.mouseWheel.delta * 0.01f));
			}
			else if (event.type == sf::Event::GainedFocus)
			{
				renderer.hasFocus = true;
			}
			else if (event.type == sf::Event::LostFocus)
			{
				renderer.hasFocus = false;
			}
		}

		characterController.modelview = renderer.modelview;
		characterController.lightModelView = renderer.lightModelView;

		characterController.OnUpdate();

		if (renderer.hasFocus)
		{
			characterController.OnFocus();

			// Mouse position from center of screen.
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			Vector2 mouse;
			mouse.y = (1.0f - (mousePosition.y / (float)window.getSize().y)) - 0.5f;
			mouse.x = ((mousePosition.x / (float)window.getSize().x)) - 0.5f;
			mouse = mouse * 10;
			renderer.lightDir.x = mouse.x;
			renderer.lightDir.y = mouse.y;
			renderer.lightDir.z = 1;
			//std::cout << "x: " << mouse.x << " y: " << mouse.y << std::endl;
		}
		renderer.modelview = characterController.modelview;
		renderer.lightModelView = characterController.lightModelView;
		renderer.worldPosition = characterController.worldPosition;

		// draw...
		renderer.Render(characterController.cameraTransform);

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	// release resources...

	return 0;
}
