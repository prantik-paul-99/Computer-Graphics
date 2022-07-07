#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int slices = 50;
int stacks = 50;
int segments = 50;

double length_bound = 30.0;
double curr_len;

struct point
{
	double x,y,z;
};

/// camera controls ///
point pos,u,r,l;
double rotate_angle = pi/180.0;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawSphereOneEighth(double radius,int slices = slices,int stacks = stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;

	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi/4.0);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi/4.0);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinderOnefourth(double radius,double height,int segments = segments)
{
    int i;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi/4);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi/4);
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i].x,points[i].y,height);
        }
        glEnd();
    }
}

void drawSixSquares()
{
    glColor3f(1,1,1);
    ///up
    glPushMatrix();
    glTranslatef(0, 0, length_bound);
    drawSquare(curr_len);
    glPopMatrix();

    ///down
    glPushMatrix();
    glTranslatef(0, 0, -length_bound);
    drawSquare(curr_len);
    glPopMatrix();

    ///front
    glPushMatrix();
    glTranslatef(0, -length_bound, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(curr_len);
    glPopMatrix();

    ///back
    glPushMatrix();
    glTranslatef(0, length_bound, 0);
    glRotatef(90, 1, 0, 0);
    drawSquare(curr_len);
    glPopMatrix();

    ///left
    glPushMatrix();
    glTranslatef(length_bound, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(curr_len);
    glPopMatrix();

    ///right
    glPushMatrix();
    glTranslatef(-length_bound, 0, 0);
    glRotatef(90, 0, 1, 0);
    drawSquare(curr_len);
    glPopMatrix();

}

void drawUpperHemisphere()
{
    glColor3f(1,0,0);
    ///up front left
    glPushMatrix();
    glRotatef(-90, 0, 0, 1);
    glTranslatef(curr_len, curr_len, curr_len);
    drawSphereOneEighth(length_bound - curr_len);
    glPopMatrix();

    ///up front right
    glPushMatrix();
    glTranslatef(curr_len, curr_len, curr_len);
    drawSphereOneEighth(length_bound - curr_len);
    glPopMatrix();

    ///up back left
    glPushMatrix();
    glRotatef(-180, 0, 0, 1);
    glTranslatef(curr_len, curr_len, curr_len);
    drawSphereOneEighth(length_bound - curr_len);
    glPopMatrix();

    ///up back right
    glPushMatrix();
    glRotatef(-270, 0, 0, 1);
    glTranslatef(curr_len, curr_len, curr_len);
    drawSphereOneEighth(length_bound - curr_len);
    glPopMatrix();
}
 void drawLowerHemisphere()
 {
    glPushMatrix();
    glRotatef(180,1,0,0);
    drawUpperHemisphere();
    glPopMatrix();
 }

void drawCylinderLeft()
{
    glColor3f(0,1,0);

    glPushMatrix();
    glTranslatef(curr_len, curr_len, 0);
    drawCylinderOnefourth(length_bound-curr_len, curr_len);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 0, 0, 1);
    glTranslatef(curr_len, curr_len, 0);
    drawCylinderOnefourth(length_bound-curr_len, curr_len);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, curr_len);
    glRotatef(90, 1, 0, 0);
    glTranslatef(curr_len, 0,0);
    drawCylinderOnefourth(length_bound-curr_len, curr_len);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, curr_len,0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(curr_len, curr_len, 0);
    drawCylinderOnefourth(length_bound-curr_len, curr_len);
    glPopMatrix();
}

void drawCylinderRight()
{
    glPushMatrix();
    glRotatef(180, 0, 0, 1);
    drawCylinderLeft();
    glPopMatrix();
}

void drawCylinderFront()
{
    glPushMatrix();
    glRotatef(90, 0, 0, 1);
    drawCylinderLeft();
    glPopMatrix();
}

void drawCylinderBack()
{
    glPushMatrix();
    glRotatef(270, 0, 0, 1);
    drawCylinderLeft();
    glPopMatrix();
}

void drawcylinderUp()
{
    glPushMatrix();
    drawCylinderLeft();
    drawCylinderRight();
    drawCylinderFront();
    drawCylinderBack();
    glPopMatrix();
}

void drawcylinderDown()
{
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    drawCylinderLeft();
    drawCylinderRight();
    drawCylinderFront();
    drawCylinderBack();
    glPopMatrix();
}


void drawSS()
{
    drawUpperHemisphere();
    drawLowerHemisphere();
    drawSixSquares();
    drawcylinderUp();
    drawcylinderDown();
}
/// rotation and translation functions

point multiply_point(point a, double b)
{
    a.x = a.x * b;
    a.y = a.y * b;
    a.z = a.z * b;
    return a;
}

point cross_multiply_points(point a, point b)
{
    point c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}

point add_points(point a, point b)
{
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
    return a;
}

point subtract_points(point a, point b)
{
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
    return a;
}

point rotate_a_wrt_b(point a, point b, int dir)
{
    point c = add_points(multiply_point(a,cos(rotate_angle*dir)),multiply_point(cross_multiply_points(b,a),sin(rotate_angle*dir)));
    return c;
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case '1':
            r = rotate_a_wrt_b(r,u,1);
            l = rotate_a_wrt_b(l,u,1);
			break;
        case '2':
            r = rotate_a_wrt_b(r,u,-1);
            l = rotate_a_wrt_b(l,u,-1);
			break;
        case '3':
            l = rotate_a_wrt_b(l,r,1);
            u = rotate_a_wrt_b(u,r,1);
			break;
        case '4':
            l = rotate_a_wrt_b(l,r,-1);
            u = rotate_a_wrt_b(u,r,-1);
			break;
        case '5':
            u = rotate_a_wrt_b(u,l,1);
            r = rotate_a_wrt_b(r,l,1);
			break;
        case '6':
            u = rotate_a_wrt_b(u,l,-1);
            r = rotate_a_wrt_b(r,l,-1);
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:
            pos = subtract_points(pos, l);
			break;
		case GLUT_KEY_UP:
            pos = add_points(pos, l);
			break;
		case GLUT_KEY_RIGHT:
		    pos = add_points(pos, r);
			break;
		case GLUT_KEY_LEFT:
		    pos = subtract_points(pos, r);
			break;
		case GLUT_KEY_PAGE_UP:
		    pos = add_points(pos, u);
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos = subtract_points(pos, u);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    curr_len -= 1;
		    if(curr_len < 0){
                curr_len = 0;
		    }
			break;
        case GLUT_KEY_END:
			curr_len += 1;
		    if(curr_len > length_bound){
                curr_len = length_bound;
		    }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here-1.0/sqrt(2.0);
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z,   pos.x + l.x, pos.y + l.y, pos.z + l.z,   u.x, u.y, u.z);
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
    drawSS();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	/// camera control initialize ///
	pos.x = 100;
    pos.y = 100;
    pos.z = 20;
    l.x = -1.0/sqrt(2.0);
    l.y = -1.0/sqrt(2.0);
    l.z = 0;
    u.x = 0;
    u.y = 0;
    u.z = 1;
    r.x = -1.0/sqrt(2.0);;
    r.y = 1.0/sqrt(2.0);;
    r.z = 0;

    /// sphere and cubes
    curr_len = length_bound/2;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("1705071_Sphere_to_Cube_Fully_Controllable_Camera");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
