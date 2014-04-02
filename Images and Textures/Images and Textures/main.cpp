#include <stdio.h>

//#include <gl/GL.h>
#include <gl/freeglut.h>

char** values;
char** equalized;
char** thresholdVals;
int size[2];

GLubyte* bitmap;
GLubyte* equalBitmap;
GLubyte* threshBitmap;

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

void threshold() {
	thresholdVals = new char*[size[1]];
	threshBitmap = new GLubyte[size[1] * size[0]];
	for (int i = 0; i < size[1]; i++) {
		thresholdVals[i] = new char[size[0]];
	}

	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			printf("Value: %d\n", (int)equalized[i][j]);
			if (equalized[i][j] < ((char)10)) {
				thresholdVals[i][j] = 0;
			} else {
				thresholdVals[i][j] = 255;
			}
			threshBitmap[(i * size[0]) + j] = (GLubyte)thresholdVals[i][j];
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	//glWindowPos2f(0.0, 0.0);

	glRasterPos2f(0.0, 0.8);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap);

	glRasterPos2f(0.0, 0.0);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, equalBitmap);

	glRasterPos2f(0.0, -0.8);
	glDrawPixels(size[0], size[1], GL_LUMINANCE, GL_UNSIGNED_BYTE, threshBitmap);

	glFlush();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void main(int argc, char **argv) {
	int maxValue;

	maxValue = readFileToArray();
	equalize(maxValue);
	threshold();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Matt's Images and Textures");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}