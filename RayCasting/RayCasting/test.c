// RayCasting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <GL/glut.h>

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(0.0, 1024, 512,0);
}

int main(int argc, char* argv[])
{
    //std::cout << "Hello World!\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 512);
	glutCreateWindow("Raycast");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
}

