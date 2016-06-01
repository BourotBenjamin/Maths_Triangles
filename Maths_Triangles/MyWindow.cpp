#include "MyWindow.h"

int window_init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	openGLWindow = glfwCreateWindow(WIDTH, HEIGHT, "Triangles", nullptr, nullptr);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(openGLWindow))
	{
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        ourShader.Use();
		glBindVertexArray(VAO_POINTS);
		unsigned short currentSize = pSize;
		switch (currentMode)
		{
		case SIMPLE_DRAW:
			glDrawArrays(GL_POINTS, 0, pSize);
			break;
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
		case TRIANGULATION_WITH_VORONOI:
			glDrawElements(GL_TRIANGLES, eboIndices.size(), GL_UNSIGNED_INT, 0);
			auto voronoiSizePtr = voronoisSizes.begin();
			auto beforeVoronoiSizePtr = triangulationsSizes.begin();
			while (voronoiSizePtr != voronoisSizes.end())
			{
				glDrawArrays(GL_LINES, *beforeVoronoiSizePtr, *voronoiSizePtr);
				voronoiSizePtr++;
				beforeVoronoiSizePtr++;
			}
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
	vboVoronoi.clear();
	eboIndices.clear();
	enveloppesSizes.clear();
	triangulationsSizes.clear();
	voronoisSizes.clear();
	switch (currentMode)
	{
	case SIMPLE_DRAW:
		pSize = scene.getPoints(vboCoords);
		break;
	case ENVELOPPE_JARVIS:
		pSize = scene.getPoints(vboCoords);
		scene.getJarvisEnveloppes(vboCoords, enveloppesSizes);
		break;
	case ENVELOPPE_GRAHAM_SCAN:
		pSize = scene.getPoints(vboCoords);
		scene.getGrahamScanEnveloppes(vboCoords, enveloppesSizes);
		break;
	case TRIANGULATION:
		scene.simpleTriangulation(vboCoords, eboIndices, false, false, triangulationsSizes, voronoisSizes);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_POINTS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboIndices.size() * sizeof(unsigned int), eboIndices.data(), GL_STATIC_DRAW);
		break;
	case TRIANGULATION_WITH_FLIPPING:
		scene.simpleTriangulation(vboCoords, eboIndices, true, false, triangulationsSizes, voronoisSizes);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_POINTS);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboIndices.size() * sizeof(unsigned int), eboIndices.data(), GL_STATIC_DRAW);
		break;
	case TRIANGULATION_WITH_VORONOI:
		scene.simpleTriangulation(vboCoords, eboIndices, true, true, triangulationsSizes, voronoisSizes);
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
		if (currentEditMode == FIND)
		{
			/*if (selectedPoint != nullptr)
				selectedPoint->setSelected(false);*/
			selectedPoint = o->findNearestPoint(x, y, 0.0);
			// TODO : getZ
			selectedPoint->setSelected(true);
			currentEditMode = MOVE;
		}
		else if (currentEditMode == MOVE)
		{
			selectedPoint->setPos(x, y, 0.0);
			// TODO : getZ
			selectedPoint->setSelected(false);
			currentEditMode = ADD;
		}
		else if (currentEditMode == MULTIPLE)
		{
			o->generatePoints(x, y, 0.0, MAX_DIST, NB_POINTS);
		}
		else
		{
			p1 = std::shared_ptr<Point>(new Point(x, y, rand() % 100 / 100.f));
			// TODO : getZ
			o->addPoint(p1);
		}
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
		case GLFW_KEY_C:
			scene.removeAll();
			o = std::shared_ptr<Object>(new Object());
			scene.addObject(o);
			updateVBO();
			break;
		case GLFW_KEY_E:
			currentEditMode = FIND;
			break;
		case GLFW_KEY_R:
			o->removePoint(selectedPoint);
			updateVBO();
			currentEditMode = ADD;
			break;
		case GLFW_KEY_KP_9:
			currentEditMode = MULTIPLE;
			break;
		case GLFW_KEY_KP_8:
			currentEditMode = ADD;
			break;
		case GLFW_KEY_KP_0:
			currentMode = SIMPLE_DRAW;
			updateVBO();
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
		case GLFW_KEY_KP_5:
			currentMode = TRIANGULATION_WITH_VORONOI;
			updateVBO();
			break;
		case GLFW_KEY_KP_ADD:
			++NB_POINTS;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			--NB_POINTS;
			break;
		case GLFW_KEY_KP_MULTIPLY:
			MAX_DIST+=10;
			break;
		case GLFW_KEY_KP_DIVIDE:
			MAX_DIST-=10;
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
