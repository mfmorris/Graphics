#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 72; //(6 faces)(2 triangles/face)(3 vertices/triangle)(2 cubes)

point4 points[NumVertices];
vec3   normals[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

point4 vertices2[8] = {
	point4(-0.25, -0.25, 0.25, 1.0),
	point4(-0.25, 0.25, 0.25, 1.0),
	point4(0.25, 0.25, 0.25, 1.0),
	point4(0.25, -0.25, 0.25, 1.0),
	point4(-0.25, -0.25, -0.25, 1.0),
	point4(-0.25, 0.25, -0.25, 1.0),
	point4(0.25, 0.25, -0.25, 1.0),
	point4(0.25, -0.25, -0.25, 1.0)
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices

int Index = 0;

float lightX = 0;
float lightY = 0;

GLuint program;

void
quad( int a, int b, int c, int d )
{
    // Initialize temporary vectors along the quad's edge to
    //   compute its face normal 
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];

    vec3 normal = normalize( cross(u, v) );

    normals[Index] = normal; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; points[Index] = vertices[b]; Index++;
    normals[Index] = normal; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; points[Index] = vertices[d]; Index++;
}

void
quad2(int a, int b, int c, int d)
{
	// Initialize temporary vectors along the quad's edge to
	//   compute its face normal 
	vec4 u = vertices2[b] - vertices2[a];
	vec4 v = vertices2[c] - vertices2[b];

	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; points[Index] = vertices2[a]; Index++;
	normals[Index] = normal; points[Index] = vertices2[b]; Index++;
	normals[Index] = normal; points[Index] = vertices2[c]; Index++;
	normals[Index] = normal; points[Index] = vertices2[a]; Index++;
	normals[Index] = normal; points[Index] = vertices2[c]; Index++;
	normals[Index] = normal; points[Index] = vertices2[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

void
colorcube2()
{
	quad2(1, 0, 3, 2);
	quad2(2, 3, 7, 6);
	quad2(3, 0, 4, 7);
	quad2(6, 5, 1, 2);
	quad2(4, 5, 6, 7);
	quad2(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    GLuint vao;
    glGenVertexArrays( 1, &vao );

	GLuint buffer;
	glGenBuffers(1, &buffer);
    
	colorcube();
	colorcube2();

	glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
		  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),
		     sizeof(normals), normals );

	program = InitShader("vshader53.glsl", "fshader53.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(0.0, 1.0, 0.0, 1.0);
	color4 material_diffuse(0.0, 1.0, 0.8, 1.0);
	color4 material_specular(0.0, 1.0, 0.8, 1.0);
	float  material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
		material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST | GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);

    glShadeModel(GL_SMOOTH);

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  Generate the model-view matrix

    const vec3 viewer_pos( 0.0, 0.0, 2.0 );
    mat4  model_view = ( Translate( -viewer_pos ) *
			 RotateX( Theta[Xaxis] ) *
			 RotateY( Theta[Yaxis] ) *
			 RotateZ( Theta[Zaxis] ) );
    
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

	point4 light_position(lightX, lightY, 2.0, 0.0);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

    glDrawArrays( GL_TRIANGLES, 0, NumVertices);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
		switch( button ) {
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
			case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}	
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += 0.5;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;
    }
}

//----------------------------------------------------------------------------

void
move(int x, int y)
{
	lightX = x;
	lightY = y;

	printf("x: %f, y: %f\n", lightX, lightY);

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    GLfloat aspect = GLfloat(width)/height;
    mat4  projection = Perspective( 45.0, aspect, 0.5, 3.0 );

    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

	glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
	glutPassiveMotionFunc(move);

    glutMainLoop();
    return 0;
}
