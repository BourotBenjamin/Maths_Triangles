// projet maths
// arretes adjacentes a deux triangles ne sont pas a tester pour le test de la visibilité avec le point courant ( triangulation quelconque )
// 04/12
#ifndef WINDOW_H
#define WINDOW_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GLFWwindow* openGLWindow;
const GLuint WIDTH = 800, HEIGHT = 600;

enum Mode {
	ENVELOPPE_JARVIS,
	ENVELOPPE_GRAHAM_SCAN,
	TRIANGULATION,
	TRIANGULATION_WITH_FLIPPING
};

int window_init();
void updateVBO();
void window_fillVBOPointsClicked();
int window_mainLoop(GLFWwindow* window, Scene& scene);
void window_keyPressed(GLFWwindow* activeWindow, int key, int scancode, int action, int mods);
void window_onClick(GLFWwindow* activeWindow, int button, int action, int mods);

std::vector<float> vboCoords;
std::vector<unsigned int> eboIndices;
std::shared_ptr<Point> p1, p2;
std::shared_ptr<Object> o;
Scene scene;
GLuint VBO, VBO_POINTS, EBO_POINTS;
int pSize;
Mode currentMode = ENVELOPPE_JARVIS;
std::vector<unsigned short> enveloppesSizes;


#endif