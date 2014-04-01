#include <stdio.h>

//#include <gl/GL.h>
#include <gl/freeglut.h>

char** values;
int size[2];

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
	for (int i = 0; i < size[1]; i++) {
		for (int j = 0; j < size[0]; j++) {
			fread(&values[i][j], 1, 1, file);
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

void display(void) {

}

void init() {

}

void main(int argc, char **argv) {
	int maxValue;

	maxValue = readFileToArray();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Matt's Images and Textures");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}