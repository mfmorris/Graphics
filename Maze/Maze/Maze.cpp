#include <gl/freeglut.h>
#include <stack>
#include <vector>
#include <math.h>
#include <time.h>

struct cell{
	int i;
	int j;
	bool north;
	bool east;
	bool south;
	bool west;
	bool visited;
};

struct cell maze[1000][1000];
int mazeHeight;
int mazeWidth;

static float centerX, centerY;
static float eyeX, eyeY, eyeZ;
static int locX, locY;
static int facing;

/*
 * Generates a random maze using a recursive backtracking algorithm.
 */
void generateMaze(int height, int width) {
	mazeHeight = height;
	mazeWidth = width;
	int cells = height * width;
	int visitedCells = 1;
	for (int i =0; i < mazeHeight; i++) {
		for (int j = 0; j < mazeWidth; j++) {
			maze[i][j].i = i;
			maze[i][j].j = j;
			maze[i][j].east = TRUE;
			maze[i][j].north = TRUE;
			maze[i][j].south = TRUE;
			maze[i][j].west = TRUE;
			maze[i][j].visited = FALSE;
		}
	}

	// Set up entrance cell and exit cell
	std::stack<struct cell> mazeStack;
	maze[0][0].north = FALSE;
	maze[height-1][width-1].south = FALSE;
	maze[0][0].visited = TRUE;
	mazeStack.push(maze[0][0]);
	srand(time(NULL));

	// Generate the maze
	while (visitedCells < cells) {
		int north = 0;
		int south = 0;
		int west = 0;
		int east = 0;
		struct cell thisCell = (struct cell)mazeStack.top();

		// Identify a random cell to explore
		if (thisCell.i > 0 && !maze[thisCell.i - 1][thisCell.j].visited) {
			north = rand() % 100 + 1;
		}
		if (thisCell.i < mazeHeight - 1 && !maze[thisCell.i + 1][thisCell.j].visited) {
			south = rand() % 100 + 1;
		}
		if (thisCell.j > 0 && !maze[thisCell.i][thisCell.j - 1].visited) {
			west = rand() % 100 + 1;
		}
		if (thisCell.j < mazeWidth - 1 && !maze[thisCell.i][thisCell.j + 1].visited) {
			east = rand() % 100 + 1;
		}

		// Identify a new cell, if all adjacent are visited
		if (north == 0 && south == 0 && west == 0 && east == 0) {
			if (!mazeStack.empty()) {
				mazeStack.pop();
			} else {
				bool found = FALSE;
				for (int i = 0; i < mazeHeight; i++) {
					for (int j = 0; j < mazeWidth; j++) {
						if (!maze[i][j].visited) {
							mazeStack.push(maze[i][j]);
							found = TRUE;
							break;
						}
					}
					if (found) {
						break;
					}
				}
			}
		}

		// Select the next cell and modify
		else if (north > south && north > east && north > west) {
			maze[thisCell.i - 1][thisCell.j].visited = TRUE;
			maze[thisCell.i - 1][thisCell.j].south = FALSE;
			maze[thisCell.i][thisCell.j].north = FALSE;
			mazeStack.push(maze[thisCell.i - 1][thisCell.j]);
			visitedCells++;
		} else if (south > north && south > east && south > west) {
			maze[thisCell.i + 1][thisCell.j].visited = TRUE;
			maze[thisCell.i + 1][thisCell.j].north = FALSE;
			maze[thisCell.i][thisCell.j].south = FALSE;
			mazeStack.push(maze[thisCell.i + 1][thisCell.j]);
			visitedCells++;
		} else if (east > south && east > north && east > west) {
			maze[thisCell.i][thisCell.j + 1].visited = TRUE;
			maze[thisCell.i][thisCell.j + 1].west = FALSE;
			maze[thisCell.i][thisCell.j].east = FALSE;
			mazeStack.push(maze[thisCell.i][thisCell.j + 1]);
			visitedCells++;
		} else if (west > south && west > north && west > east) {
			maze[thisCell.i][thisCell.j - 1].visited = TRUE;
			maze[thisCell.i][thisCell.j - 1].east = FALSE;
			maze[thisCell.i][thisCell.j].west = FALSE;
			mazeStack.push(maze[thisCell.i][thisCell.j - 1]);
			visitedCells++;
		}
	}
}

void drawMaze() {
	for (int i = 0; i < mazeHeight; i++) {
		for (int j = 0; j < mazeWidth; j++) {
			if (maze[i][j].east) {
				glColor3f(0.580, 0.0, 0.827);
				glBegin(GL_POLYGON);
				GLfloat v[3];
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				v[2] = 1;
				glVertex3fv(v);
				//glNormal3f(-1.0, 0.0, 0.0);
				glEnd();
			}

			if (maze[i][j].north) {
				glColor3f(0.545, 0.109, 0.348);
				glBegin(GL_POLYGON);
				GLfloat v[3];
				v[0] = j;
				v[1] = mazeHeight - i;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j;
				v[1] = mazeHeight - i;
				v[2] = 1;
				glVertex3fv(v);
				//glNormal3f(0.0, -1.0, 0.0);
				glEnd();
			}

			if (maze[i][j].south) {
				glColor3f(0.545, 0.109, 0.348);
				glBegin(GL_POLYGON);
				GLfloat v[3];
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				v[2] = 0;
				glVertex3fv(v);
				//glNormal3f(0.0, 1.0, 0.0);
				glEnd();
			}

			if (maze[i][j].west) {
				glColor3f(0.502, 0.0, 0.502);
				glBegin(GL_POLYGON);
				GLfloat v[3];
				v[0] = j;
				v[1] = mazeHeight - i;
				v[2] = 0;
				glVertex3fv(v);
				v[0] = j;
				v[1] = mazeHeight - i;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				v[2] = 1;
				glVertex3fv(v);
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				v[2] = 0;
				glVertex3fv(v);
				//glNormal3f(1.0, 0.0, 0.0);
				glEnd();
			}
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMaze();

	glFlush();
}

void forward() {
	switch (facing) {
	case 0: //NORTH
		if (!maze[locY][locX].north) {
			printf("move north\n");

			for (float i = 0.0001; i <= 1; i += 0.0001) {
				eyeY += 0.0001;
				centerX = eyeX;
				centerY = eyeY + 0.25;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(eyeX, eyeY, eyeZ,	// Eye
					centerX, centerY, 0.5,	// Center
					0.0, 0.0, 1.0);	// Up
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawMaze();
				glFlush();
			}

			locY--;
		}
		break;
	case 1: //EAST
		if (!maze[locY][locX].east) {
			printf("move east\n");

			for (float i = 0.0001; i <= 1; i += 0.0001) {
				eyeX += 0.0001;
				centerX = eyeX + 0.25;
				centerY = eyeY;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(eyeX, eyeY, eyeZ,	// Eye
					centerX, centerY, 0.5,	// Center
					0.0, 0.0, 1.0);	// Up
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawMaze();
				glFlush();
			}
			
			locX++;
		}
		break;
	case 2: //SOUTH
		if (!maze[locY][locX].south) {
			printf("move south\n");

			for (float i = 0.0001; i <= 1; i += 0.0001) {
				eyeY -= 0.0001;
				centerX = eyeX;
				centerY = eyeY - 0.25;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(eyeX, eyeY, eyeZ,	// Eye
					centerX, centerY, 0.5,	// Center
					0.0, 0.0, 1.0);	// Up
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawMaze();
				glFlush();
			}

			locY++;
		}
		break;
	case 3: //WEST
		if (!maze[locY][locX].west) {
			printf("move west\n");

			for (float i = 0.0001; i <= 1; i += 0.0001) {
				eyeX -= 0.0001;
				centerX = eyeX - 0.25;
				centerY = eyeY;
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(eyeX, eyeY, eyeZ,	// Eye
					centerX, centerY, 0.5,	// Center
					0.0, 0.0, 1.0);	// Up
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawMaze();
				glFlush();
			}

			locX--;
		}
		break;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,	// Eye
		centerX, centerY, 0.5,	// Center
		0.0, 0.0, 1.0);	// Up
	glutPostRedisplay();
}

void turnLeft() {
	switch (facing) {
	case 0:
		printf("facing west\n");
		facing = 3;
		//centerX = eyeX - 0.25;
		//centerY = eyeY;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX -= 0.0001;
			centerY -= 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 1:
		printf("facing north\n");
		facing = 0;
		//centerX = eyeX;
		//centerY = eyeY + 0.25;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX -= 0.0001;
			centerY += 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 2:
		printf("facing east\n");
		facing = 1;
		//centerX = eyeX + 0.25;
		//centerY = eyeY;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX += 0.0001;
			centerY += 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 3:
		printf("facing south\n");
		facing = 2;
		//centerX = eyeX;
		//centerY = eyeY - 0.25;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX += 0.0001;
			centerY -= 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,	// Eye
		centerX, centerY, 0.5,	// Center
		0.0, 0.0, 1.0);	// Up
	glutPostRedisplay();
}

void turnRight() {
	switch (facing) {
	case 0:
		printf("facing east\n");
		facing = 1;
		//centerX = eyeX + 0.25;
		//centerY = eyeY;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX += 0.0001;
			centerY -= 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 1:
		printf("facing south\n");
		facing = 2;
		//centerX = eyeX;
		//centerY = eyeY - 0.25;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX -= 0.0001;
			centerY -= 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 2:
		printf("facing west\n");
		facing = 3;
		//centerX = eyeX - 0.25;
		//centerY = eyeY;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX -= 0.0001;
			centerY += 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	case 3:
		printf("facing north\n");
		facing = 0;
		//centerX = eyeX;
		//centerY = eyeY + 0.25;

		for (float i = 0.0001; i <= 0.25; i += 0.0001) {
			centerX += 0.0001;
			centerY += 0.0001;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(eyeX, eyeY, eyeZ,	// Eye
				centerX, centerY, 0.5,	// Center
				0.0, 0.0, 1.0);	// Up
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawMaze();
			glFlush();
		}

		break;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,	// Eye
		centerX, centerY, 0.5,	// Center
		0.0, 0.0, 1.0);	// Up
	glutPostRedisplay();
}

void mazeKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		forward();
		break;
	case 'a':
		turnLeft();
		break;
	case 'd':
		turnRight();
		break;
	}
}

void myinit() {
	int size = fmax(mazeWidth, mazeHeight);

	eyeX = 0.5;
	eyeY = mazeHeight - 0.5;
	eyeZ = 0.5;

	locX = 0;
	locY = 0;

	centerX = eyeX;
	centerY = eyeY - 0.25;

	facing = 2;

	glutKeyboardFunc(mazeKeys);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, -1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,	// Eye
		centerX, centerY, 0.5,	// Center
		0.0, 0.0, 1.0);	// Up
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

int main(int argc, char **argv) {
	int height = 0, width = 0;
	argc = 0;

	// Read configuration file and generate maze based on the sizes specified.
	FILE* file = fopen("..\\Debug\\maze.txt", "r");
	if (file != NULL) {
		fscanf(file, "%d %d", &height, &width);
		fclose(file);
	}
	generateMaze(height, width);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Matt's Maze");
	glutDisplayFunc(display);
	myinit();
	glutMainLoop();
}