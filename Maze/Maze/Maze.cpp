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
				glBegin(GL_LINES);
				GLfloat v[2];
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				glVertex2fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				glVertex2fv(v);
				glEnd();
			}
			if (maze[i][j].north) {
				glBegin(GL_LINES);
				GLfloat v[2];
				v[0] = j;
				v[1] = mazeHeight - i;
				glVertex2fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i;
				glVertex2fv(v);
				glEnd();
			}
			if (maze[i][j].south) {
				glBegin(GL_LINES);
				GLfloat v[2];
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				glVertex2fv(v);
				v[0] = j + 1;
				v[1] = mazeHeight - i - 1;
				glVertex2fv(v);
				glEnd();
			}
			if (maze[i][j].west) {
				glBegin(GL_LINES);
				GLfloat v[2];
				v[0] = j;
				v[1] = mazeHeight - i;
				glVertex2fv(v);
				v[0] = j;
				v[1] = mazeHeight - i - 1;
				glVertex2fv(v);
				glEnd();
			}
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT, GL_LINE);
	drawMaze();

	glFlush();
}

void myinit() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int size = fmax(mazeWidth, mazeHeight);
	gluOrtho2D(-1.0, size + 1.0, -1.0, size + 1.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT, GL_LINE);
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