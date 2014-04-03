#include <stdio.h>

//#include <gl/GL.h>
#include <gl/freeglut.h>

char** values;
char** equalized;
char** thresholdVals;
int size[2];

GLubyte* bitmap;
GLubyte* equalBitmap;
GLubyte** equalTexture;
GLubyte* threshBitmap;

static GLuint texName;

int readFileToArray() {
	char magic1 = 'a';
	int magic2 = 0;
	int maxValue = 0;
	FILE* file;


	file = fopen("doctored_image.ppm", "r");
	if (file == NULL) { printf("File error\n"); while (1); }


	fscanf(file, "%c %d", &magic1, &magic2);
	//printf("%c%d\n", magic1, magic2);

	fscanf(file, "%d %d", &size[0], &size[1]);
	printf("Size: %d x %d\n", size[0], size[1]);

	values = new char*[size[1]];
	for (int i = 0; i < size[1]; i++) {
		values[i] = new char[size[0]];
	}

	fscanf(file, "%d", &maxValue);
	//printf("%d\n", maxValue);

	int maxValInFile = 0;
	bitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			fread(&values[i][j], 1, 1, file);
			bitmap[(i * size[0]) + j] = (GLubyte)values[i][j];
			//printf("%d ", (int)values[i][j]);
			if ((int)values[i][j] > maxValInFile) {
				maxValInFile = (int)values[i][j];
			}
		}
		//printf("\n");
	}

	printf("Max Value: %d\n", maxValInFile);

	return maxValInFile;
}

void equalize(int maxValue) {
	equalized = new char*[size[1]];
	equalBitmap = new GLubyte[size[1] * size[0]];
	equalTexture = new GLubyte*[size[1]];
	for (int i = 0; i < size[1]; i++) {
		equalized[i] = new char[size[0]];
		equalTexture[i] = new GLubyte[size[0]];
	}

	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			equalized[i][j] = values[i][j] * (255 / maxValue);
			equalBitmap[(i * size[0]) + j] = (GLubyte)equalized[i][j];
			equalTexture[i][j] = (GLubyte)equalized[i][j];
		}
	}
}

void threshold() {
	thresholdVals = new char*[size[1]];
	threshBitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		thresholdVals[i] = new char[size[0]];
	}

	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			if (equalized[i][j] < ((char)10)) {
				thresholdVals[i][j] = 0;
			} else {
				thresholdVals[i][j] = 255;
			}
			threshBitmap[(i * size[0]) + j] = (GLubyte)thresholdVals[i][j];
		}
	}
}

void cube() {
	

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);

}

void displayOriginal(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2f(-1.0, -1.0);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap);

	glFlush();
}

void displayEqualized(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2f(-1.0, -1.0);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, equalBitmap);

	glFlush();
}

void displayThreshold(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2f(-1.0, -1.0);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, threshBitmap);

	glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);
}


void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, size[1], size[0], 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, equalTexture);
}

void main(int argc, char **argv) {
	int maxValue;

	maxValue = readFileToArray();
	equalize(maxValue);
	threshold();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	
	glutInitWindowPosition(300, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Textured Image");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(size[0], size[1]);
	glutCreateWindow("Original Image");
	glutDisplayFunc(displayOriginal);

	glutInitWindowPosition(0, 200);
	glutInitWindowSize(size[0], size[1]);
	glutCreateWindow("Equalized Image");
	glutDisplayFunc(displayEqualized);

	glutInitWindowPosition(0, 400);
	glutInitWindowSize(size[0], size[1]);
	glutCreateWindow("Threshold Image");
	glutDisplayFunc(displayThreshold);

	init();
	glutMainLoop();
}