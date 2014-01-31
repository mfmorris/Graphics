#include <gl/freeglut.h>
#include <math.h>

GLfloat v[4][2] = { { -1.0, -1.0 }, { -1.0, 1.0 }, { 1.0, 1.0 }, { 1.0, -1.0 } };
GLfloat t[3][2] = { { -1.0, 0.0 }, { 1.0, 0.0 }, { 0.0, 1.00 } };


float spin;

void square(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d) {
	GLfloat v[2];
	double e;
	glBegin(GL_POLYGON);
	e = sqrt(a[0] * a[0] + a[1] * a[1]);
	v[0] = cos(spin*e)*a[0] - sin(spin*e)*a[1];
	v[1] = sin(spin*e)*a[0] + cos(spin*e)*a[1];
	glVertex2fv(v);
	e = sqrt(b[0] * b[0] + b[1] * b[1]);
	v[0] = cos(spin*e)*b[0] - sin(spin*e)*b[1];
	v[1] = sin(spin*e)*b[0] + cos(spin*e)*b[1];
	glVertex2fv(v);
	e = sqrt(c[0] * c[0] + c[1] * c[1]);
	v[0] = cos(spin*e)*c[0] - sin(spin*e)*c[1];
	v[1] = sin(spin*e)*c[0] + cos(spin*e)*c[1];
	glVertex2fv(v);
	e = sqrt(d[0] * d[0] + d[1] * d[1]);
	v[0] = cos(spin*e)*d[0] - sin(spin*e)*d[1];
	v[1] = sin(spin*e)*d[0] + cos(spin*e)*d[1];
	glVertex2fv(v);
	glEnd();
}

void triangle(GLfloat *a, GLfloat *b, GLfloat *c) {
	GLfloat v[2];
	double e;
	glBegin(GL_POLYGON);
	e = sqrt(a[0] * a[0] + a[1] * a[1]);
	v[0] = cos(-1 * spin*e)*a[0] - sin(-1 * spin*e)*a[1];
	v[1] = sin(-1 * spin*e)*a[0] + cos(-1 * spin*e)*a[1];
	glVertex2fv(v);
	e = sqrt(b[0] * b[0] + b[1] * b[1]);
	v[0] = cos(-1 * spin*e)*b[0] - sin(-1 * spin*e)*b[1];
	v[1] = sin(-1 * spin*e)*b[0] + cos(-1 * spin*e)*b[1];
	glVertex2fv(v);
	e = sqrt(c[0] * c[0] + c[1] * c[1]);
	v[0] = cos(-1 * spin*e)*c[0] - sin(-1 * spin*e)*c[1];
	v[1] = sin(-1 * spin*e)*c[0] + cos(-1 * spin*e)*c[1];
	glVertex2fv(v);
	glEnd();
}

void circle(float x, float y, float r, int numLines) {
	GLfloat v[2];
	glBegin(GL_POLYGON);
	for (int i = 0; i < numLines; i ++) {
		double angle = i * 2 * 3.14159 / numLines;
		v[0] = x + (r * cos(angle));
		v[1] = y + (r * sin(angle));
		glVertex2fv(v);
	}
	glEnd();
}

void display(void) {

	while (spin < 8) {
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(0.0, 0.0, 1.0);
		glPolygonMode(GL_FRONT, GL_LINE);
		square(v[0], v[1], v[2], v[3]);

		glColor3f(1.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT, GL_LINE);
		triangle(t[0], t[1], t[2]);

		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT, GL_LINE);
		circle(0.0, 0.0, 1.0, 30);

		spin += 0.0001;
		glFlush();
	}

}

void myinit() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT, GL_LINE);
}

int main(int argc, char **argv) {
	spin = 0;
	argc = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Square Spin Test");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
}