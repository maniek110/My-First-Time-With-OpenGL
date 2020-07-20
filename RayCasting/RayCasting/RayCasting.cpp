// RayCasting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#define pi 3.14159265359
#define pi2 pi/2
#define pi32 pi*3/2
#define dr 0.0174533

//For FPS Counter
int frame=0,base_time=0;
double time;

//Player variables
float pX, pY,pDirX,pDirY,pAngl;

//Amount of Rays
int r = 60;

int map[] = {
1,1,1,1,1,1,1,1,
1,0,0,0,0,1,0,1,
1,0,1,0,0,0,1,1,
1,0,0,0,1,0,0,1,
1,0,1,0,1,1,0,1,
1,0,1,0,0,0,1,1,
1,0,0,0,1,0,0,1,
1,1,1,1,1,1,1,1,
};
int mapX = 8, mapY = 8, size = 64;

void drawPlayer() {
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(pX, pY);
	glEnd();

	glColor3f(1, 1, 0);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2i(pX, pY);
	glVertex2i(pX+pDirX*10, pY+pDirY*10);
	glEnd();
}



void mapDraw() {
	int x0, y0;
	for (int y = 0; y < mapY; y++) {
		for (int x = 0; x < mapX; x++) {
			if (map[x + y * mapY] == 1)glColor3f(1, 1, 1);
			else glColor3f(0, 0, 0);
			x0 = x * size;
			y0 = y * size;
			glBegin(GL_QUADS);
			glVertex2i(x0+1,y0 + 1);
			glVertex2i(x0 + 1,y0+size -1);
			glVertex2i(x0+size - 1,y0+size -1);
			glVertex2i(x0 + size -1,y0 + 1);
			glEnd();
		}
	}
}

float calcDist(float endx, float endy) {
	return sqrt(pow((pX - endx), 2) + pow((pY - endy), 2));
}


void makeRays() {
	int mx, my, mp, dof;
	float rx, ry, ra, xO, yO,wallDistance;
	ra = pAngl-dr*30;
	if (ra < 0) ra += 2 * pi;
	if (ra > 2*pi) ra -= 2 * pi;
	for (int i = 0; i < r; i++) {
		float rayDist = 1000000, rayx = pX, rayy = pY;
		//Horizontal Lines
		dof = 0;
		float atan = -1 / tan(ra);
		if (ra > pi) {
			ry = (((int)pY >> 6) << 6) - 0.0001;
			rx = (pY - ry) * atan + pX;
			yO = -64;
			xO = -yO * atan;
		}
		if (ra < pi) {
			ry = (((int)pY >> 6) << 6) + 64;
			rx = (pY - ry) * atan + pX;
			yO = 64;
			xO = -yO * atan;
		}
		if (ra == 0 || ra == pi) {
			rx = pX;
			ry = pY;
			dof = 8;
		}
		while (dof < 8) {
			mx = (int)rx >> 6;
			my = (int)ry >> 6;
			mp = my * mapX + mx;
			if (mp>0 && mp < mapX * mapY && map[mp] == 1)dof = 8;
			else {
				rx += xO;
				ry += yO;
				dof += 1;
			}
		}
		if (calcDist(rx, ry) < rayDist) {
			rayx = rx;
			rayy = ry;
			rayDist = calcDist(rx, ry);
			wallDistance = rayDist;
		}
		//*/
		//Vertical Lines
		dof = 0;
		float ntan = -tan(ra);
		//left
		if (ra > pi2 && ra < pi32) {
			rx= (((int)pX >> 6) << 6) - 0.0001;
			ry = (pX - rx) * ntan + pY;
			xO = -64;
			yO = -xO * ntan;
		}
		//right
		if (ra < pi2 || ra > pi32) {
			rx = (((int)pX >> 6) << 6) +64;
			ry = (pX - rx) * ntan + pY;
			xO = 64;
			yO = -xO * ntan;
		}
		if (ra == 0 || ra == pi) {
			rx = pX;
			ry = pY;
			dof = 8;
		}
		while (dof < 8) {
			mx = (int)rx >> 6;
			my = (int)ry >> 6;
			mp = my * mapX + mx;
			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)dof = 8;
			else {
				rx += xO;
				ry += yO;
				dof += 1;
			}
		}
		if (calcDist(rx, ry) < rayDist) {
			rayx = rx;
			rayy = ry;
			rayDist = calcDist(rx, ry);
			wallDistance = rayDist;
		}

		glColor3f(0, 0, 1);
		glLineWidth(2);
		glBegin(GL_LINES);
		glVertex2i(pX, pY);
		glVertex2i(rayx, rayy);
		glEnd();
		//draw 3d walls
		ra += dr;
		if (ra < 0) ra += 2 * pi;
		if (ra > 2 * pi) ra -= 2 * pi;
	}
}

void inputs(unsigned char key, int x, int y) {
	if (key == 'a') {
		pAngl -= 0.1;
		if (pAngl < 0)pAngl += 2 * pi;
		pDirX = cos(pAngl) * 5;
		pDirY = sin(pAngl) * 5;
	}
	if (key == 'd') {
		pAngl += 0.1;
		if (pAngl > 2 * pi)pAngl -= 2 * pi;
		pDirX = cos(pAngl) * 5;
		pDirY = sin(pAngl) * 5;
	}
	if (key == 'w') {
		pX += pDirX;
		pY += pDirY;
	}
	if (key == 's') {
		pX -= pDirX;
		pY -= pDirY;
	}
	glutPostRedisplay();
}

void fpsCounter() {
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - base_time > 1000) {
		std::cout << "FPS: " << frame * 1000.0 / (time - base_time) << std::endl;
		base_time = time;
		frame = 0;
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	mapDraw();
	makeRays();
	drawPlayer();
	fpsCounter();
	glutSwapBuffers();
}

void init() {
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0.0, 1024, 512,0);
	pAngl = 0;
	pDirX = cos(pAngl) * 5;
	pDirY = sin(pAngl) * 5;
	pX = 100;
	pY = 100;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 512);
	glutCreateWindow("Raycast");
	init();
	glutDisplayFunc(display);
	//glutIdleFunc(calculateVars);
	glutKeyboardFunc(inputs);
	glutMainLoop();
}

