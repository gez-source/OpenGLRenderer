#include "Debug.h"
#include "OpenGlUtils.h"

typedef struct LineRenderable
{
public:
	Vector3 start;
	Vector3 end;
	ColourRGBA colour;
} LineRenderable_T;

std::queue<LineRenderable_T> Debug::renderQueue;
ShaderProgram* Debug::program = nullptr;

//void clearQueue(std::queue<LineRenderable_T>& q)
//{
//	std::queue<LineRenderable_T> empty;
//	std::swap(q, empty);
//}

void Debug::DrawLine(Vector3 start, Vector3 end, ColourRGBA colour)
{
	// Append line to queue for later rendering.
	LineRenderable_T line;
	line.start = start;
	line.end = end;
	line.colour = colour;

	renderQueue.push(line);
}

void Debug::DrawLines_Init()
{
	program = new ShaderProgram("Shaders/line_vs.glsl", "Shaders/line_fs.glsl");
}

void Debug::DrawLines_RenderDispatch(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview)
{
	// Get the lines and buffer them to the GPU.
	std::vector<GLfloat> verticies;
	std::vector<GLfloat> colours;
	bool hasLines = false;

	while (!renderQueue.empty())
	{
		LineRenderable_T line = renderQueue.front();
		renderQueue.pop();
		
		// Vertex 1.
		verticies.push_back(line.start.x);
		verticies.push_back(line.start.y);
		verticies.push_back(line.start.z);

		// Vertex 2.
		verticies.push_back(line.end.x);
		verticies.push_back(line.end.y);
		verticies.push_back(line.end.z);

		// Vertex 1.
		colours.push_back(line.colour.r);
		colours.push_back(line.colour.g);
		colours.push_back(line.colour.b);

		// Vertex 2.
		colours.push_back(line.colour.r);
		colours.push_back(line.colour.g);
		colours.push_back(line.colour.b);

		hasLines = true;
	}

	if (hasLines)
	{
		int verticiesCount = verticies.size() / 3;
		GLuint vao = 0;

		GLuint points_vbo;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticies.size(), verticies.data(), GL_STATIC_DRAW);
		
		GLuint colours_vbo;
		glGenBuffers(1, &colours_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colours.size(), colours.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Render all the lines.
		OpenGlUtils::Antialias(true);
		OpenGlUtils::DisableBlending();
		OpenGlUtils::EnableDepthTesting(true);

		glEnable(GL_CULL_FACE);  // cull face
		glCullFace(GL_BACK);		// cull back face
		glFrontFace(GL_CCW);			// GL_CCW for counter clock-wise

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL GL_LINE


		// Draw object with current shader.
		glUseProgram(program->programID);

		float* arrProjectionMat = projectionMatrix.ToArray();
		float* arrModelviewMat = modelview.ToArray();
		float* arrViewMat = cameraTransform.localMatrix.ToArray();

		int projectionMatrixLocation = glGetUniformLocation(program->programID, "projectionMatrix");
		int modelviewMatrixLocation = glGetUniformLocation(program->programID, "modelviewMatrix");
		int viewMatrixLocation = glGetUniformLocation(program->programID, "viewMatrix");

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, arrProjectionMat);
		glUniformMatrix4fv(modelviewMatrixLocation, 1, GL_FALSE, arrModelviewMat);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, arrViewMat);

		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_LINES, 0, verticiesCount); // GL_LINES GL_TRIANGLES

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);

		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		delete[] arrModelviewMat;
		delete[] arrProjectionMat;
		delete[] arrViewMat;

		// Free/delete line buffer from GPU.
		glDeleteBuffers(1, &points_vbo);
		glDeleteBuffers(1, &colours_vbo);
		glDeleteVertexArrays(1, &vao);

		// Clear queue of lines for next time.
		//clearQueue(renderQueue);
	}
}