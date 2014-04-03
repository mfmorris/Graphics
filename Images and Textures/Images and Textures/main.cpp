#include <stdio.h>

#include <gl/freeglut.h>

char** values;
char** equalized;
char** thresholdVals;
int size[2];

GLubyte* bitmap;
GLubyte* equalBitmap;
GLubyte* threshBitmap;

static GLuint texName;

// I had to hack this real bad
#define imageWidth 292
#define imageHeight 291
static GLubyte textureImage[imageHeight][imageWidth][4];

void makeTexture(void)
{
	int i, j, c;

	for (i = 0; i < imageHeight; i++) {
		for (j = 0; j < imageWidth; j++) {
			c = thresholdVals[i][j];
			textureImage[i][j][0] = (GLubyte)0;
			textureImage[i][j][1] = (GLubyte)c;
			textureImage[i][j][2] = (GLubyte)c/2;
			textureImage[i][j][3] = (GLubyte)255;
		}
	}
}


int readFileToArray() {
	char magic1 = 'a';
	int magic2 = 0;
	int maxValue = 0;
	FILE* file;


	file = fopen("doctored_image.ppm", "r");
	if (file == NULL) { printf("File error\n"); while (1); }


	fscanf(file, "%c %d", &magic1, &magic2);

	fscanf(file, "%d %d", &size[0], &size[1]);
	printf("Size: %d x %d\n", size[0], size[1]);

	values = new char*[size[1]];
	for (int i = 0; i < size[1]; i++) {
		values[i] = new char[size[0]];
	}

	fscanf(file, "%d", &maxValue);

	int maxValInFile = 0;
	bitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			fread(&values[i][j], 1, 1, file);
			bitmap[(i * size[0]) + j] = (GLubyte)values[i][j];
			if ((int)values[i][j] > maxValInFile) {
				maxValInFile = (int)values[i][j];
			}
		}
	}

	printf("Max Value: %d\n", maxValInFile);

	return maxValInFile;
}

void equalize(int maxValue) {
	equalized = new char*[size[1]];
	equalBitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		equalized[i] = new char[size[0]];
	}

	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			equalized[i][j] = values[i][j] * (255 / maxValue);
			equalBitmap[(i * size[0]) + j] = (GLubyte)equalized[i][j];
		}
	}
}

void threshold(int threshold) {
	thresholdVals = new char*[size[1]];
	threshBitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		thresholdVals[i] = new char[size[0]];
	}

	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			if (equalized[i][j] < threshold) {
				thresholdVals[i][j] = 0;
			} else {
				thresholdVals[i][j] = 255;
			}
			threshBitmap[(i * size[0]) + j] = (GLubyte)thresholdVals[i][j];
		}
	}
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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeTexture();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);

}

void main(int argc, char **argv) {
	int maxValue;

	maxValue = readFileToArray();
	equalize(maxValue);
	threshold(10);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(size[1], size[0]);
	glutCreateWindow("Original Image");
	glutDisplayFunc(displayOriginal);

	glutInitWindowPosition(0, 200);
	glutInitWindowSize(size[1], size[0]);
	glutCreateWindow("Equalized Image");
	glutDisplayFunc(displayEqualized);

	glutInitWindowPosition(0, 400);
	glutInitWindowSize(size[1], size[0]);
	glutCreateWindow("Threshold Image");
	glutDisplayFunc(displayThreshold);

	glutInitWindowPosition(300, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Textured Image");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	init();
	glutMainLoop();
}