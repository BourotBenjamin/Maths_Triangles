#include "MyWindow.h"

int window_init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	openGLWindow = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (openGLWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(openGLWindow); 
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);


	glfwSetKeyCallback(openGLWindow, window_keyPressed);
	glfwSetMouseButtonCallback(openGLWindow, window_onClick);
	std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
	return 0;
}


int window_mainLoop(GLFWwindow* openGLWindow)
{

	Shader ourShader("./VertexShader.txt", "./FragmentShader.txt");
	ourShader.Use();
	glm::mat4 proj = glm::ortho(0.0f, WIDTH * 1.0f, HEIGHT * 1.0f, 0.0f);
	GLint projlLoc = glGetUniformLocation(ourShader.Program, "projection");
	glUniformMatrix4fv(projlLoc, 1, GL_FALSE, glm::value_ptr(proj));
	GLint heightlLoc = glGetUniformLocation(ourShader.Program, "height");
	glUniform1i(heightlLoc, HEIGHT);
	GLint widthlLoc = glGetUniformLocation(ourShader.Program, "width");
	glUniform1i(widthlLoc, WIDTH);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(10);
	
	GLuint VAO_POINTS;
	glGenVertexArrays(1, &VAO_POINTS);
	glGenBuffers(1, &VBO_POINTS);
	glGenBuffers(1, &EBO_POINTS);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_POINTS);
	updateVBO();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(openGLWindow))
	{
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        ourShader.Use();
		glBindVertexArray(VAO_POINTS);
		unsigned short currentSize = pSize;
		switch (currentMode)
		{
		case ENVELOPPE_JARVIS:
		case ENVELOPPE_GRAHAM_SCAN:
			glDrawArrays(GL_POINTS, 0, pSize);
			for each (unsigned short size in enveloppesSizes)
			{
				glDrawArrays(GL_LINE_STRIP, currentSize, size);
				currentSize += size;
			}
			break;
		case TRIANGULATION:
		case TRIANGULATION_WITH_FLIPPING:
			glDrawElements(GL_TRIANGLES, eboIndices.size(), GL_UNSIGNED_INT, 0);
			break;
		}
		glfwSwapBuffers(openGLWindow);
	}

	glDeleteVertexArrays(1, &VAO_POINTS);
	glDeleteBuffers(1, &VBO_POINTS);
	glDeleteBuffers(1, &EBO_POINTS);
	glfwTerminate();
	return 0;
}

void updateVBO()
{
	vboCoords.clear();
	eboIndices.clear();
	enveloppesSizes.clear();
	switch (currentMode)
	{
	case ENVELOPPE_JARVIS:
		pSize = scene.getPoints(vboCoords);
		scene.getJarvisEnveloppes(vboCoords, enveloppesSizes);
		break;
	case ENVELOPPE_GRAHAM_SCAN:
		pSize = scene.getPoints(vboCoords);
		scene.getGrahamScanEnveloppes(vboCoords, enveloppesSizes);
		break;
	case TRIANGULATION:
		scene.simpleTriangulation(vboCoords, eboIndices, false);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_POINTS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboIndices.size() * sizeof(unsigned int), eboIndices.data(), GL_STATIC_DRAW);
		break;
	case TRIANGULATION_WITH_FLIPPING:
		scene.simpleTriangulation(vboCoords, eboIndices, true);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_POINTS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboIndices.size() * sizeof(unsigned int), eboIndices.data(), GL_STATIC_DRAW);
		break;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POINTS);
	glBufferData(GL_ARRAY_BUFFER, vboCoords.size() * sizeof(GLfloat), vboCoords.data(), GL_STATIC_DRAW);
}



void window_onClick(GLFWwindow* activeWindow, int button, int action, int mods)
{
	if (action == GLFW_RELEASE)
	{
		double x, y;
		glfwGetCursorPos(activeWindow, &x, &y);
		p1 = std::shared_ptr<Point>(new Point(x, y));
		o->addPoint(p1);
		updateVBO();
	}
}



void window_keyPressed(GLFWwindow* activeWindow, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(activeWindow, GL_TRUE);
			break;
		case GLFW_KEY_N:
			o = std::shared_ptr<Object>(new Object());
			scene.addObject(o);
			break;
		case GLFW_KEY_KP_1:
			currentMode = ENVELOPPE_JARVIS;
			updateVBO();
			break;
		case GLFW_KEY_KP_2:
			currentMode = ENVELOPPE_GRAHAM_SCAN;
			updateVBO();
			break;
		case GLFW_KEY_KP_3:
			currentMode = TRIANGULATION;
			updateVBO();
			break;
		case GLFW_KEY_KP_4:
			currentMode = TRIANGULATION_WITH_FLIPPING;
			updateVBO();
			break;
		}
	}
    
    
}

int main()
{
	scene = Scene();
	o = std::shared_ptr<Object>(new Object());
	scene.addObject(o);
	int r = window_init();
	if (r < 0)
		return r;
	else
		return window_mainLoop(openGLWindow);
	return 0;
}
