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
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO_POINTS);
	updateVBO();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(openGLWindow))
	{
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        ourShader.Use();
		glBindVertexArray(VAO_POINTS);
		glDrawArrays(GL_POINTS, 0, pSize);
		unsigned short currentSize = pSize;
		for each (unsigned short size in enveloppesSizes)
		{
			glDrawArrays(GL_LINE_STRIP, currentSize, size);
			currentSize += size;
		}
		glfwSwapBuffers(openGLWindow);
	}

	glDeleteVertexArrays(1, &VAO_POINTS);
	glDeleteBuffers(1, &VBO_POINTS);
	glfwTerminate();
	return 0;
}

void updateVBO()
{
	vboCoords.clear();
	enveloppesSizes.clear();
	pSize = scene.getPoints(vboCoords);
	scene.getJarvisEnveloppes(vboCoords, enveloppesSizes);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_POINTS);
	glBufferData(GL_ARRAY_BUFFER, vboCoords.size() * sizeof(GLfloat) , vboCoords.data(), GL_STATIC_DRAW);
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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(activeWindow, GL_TRUE);
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
		o = std::shared_ptr<Object>(new Object());
		scene.addObject(o);
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
