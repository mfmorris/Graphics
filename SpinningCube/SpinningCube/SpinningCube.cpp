#include <gl/freeglut.h>

// Vertices of a unit cube centered at origin, sides aligned with axes
GLfloat vertices[8][3] = {
	{ -0.5, -0.5, 0.5 }, // a 0  b - c
	{ -0.5, 0.5, 0.5 },  // b 1  | f + g
	{ 0.5, 0.5, 0.5 },   // c 2  a + d |
	{ 0.5, -0.5, 0.5 },  // d 3    e - h
	{ -0.5, -0.5, -0.5 },// e 4
	{ -0.5, 0.5, -0.5 }, // f 5
	{ 0.5, 0.5, -0.5 },  // g 6
	{ 0.5, -0.5, -0.5 }  // h 7
};

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

void cube() {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[3]);

	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[3]);

	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);
	glVertex3fv(vertices[7]);
	glVertex3fv(vertices[4]);

	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[4]);

	glVertex3fv(vertices[1]);
	glVertex3fv(vertices[2]);
	glVertex3fv(vertices[5]);
	glVertex3fv(vertices[6]);

	glVertex3fv(vertices[0]);
	glVertex3fv(vertices[3]);
	glVertex3fv(vertices[4]);
	glVertex3fv(vertices[7]);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube();
	
	glFlush();
}

void init() {
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, -5.0,	// Eye
				0.0, 0.0, 0.0,	// Center
				0.0, 0.0, 1.0);	// Up
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Spinning Cube");

	init();

	glutDisplayFunc(display);

	glutMainLoop();
}