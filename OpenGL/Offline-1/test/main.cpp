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

double leftRightRotate_angle = 0, rotate_angle = 0, leftRightRotateStep = 2, rotate_angleStep = 2;

int segments = 60;

double height = 10;
double radius = 20;
double one_click_dist = 2*pi*radius*leftRightRotateStep/360;

struct point
{
	double x,y,z;
};

point forward_backward;


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
			for(i=-20;i<=20;i++){
				//lines parallel to Y-axis
				glVertex3f(i*10, -200, 0);
				glVertex3f(i*10,  200, 0);

				//lines parallel to X-axis
				glVertex3f(-200, i*10, 0);
				glVertex3f( 200, i*10, 0);
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

void drawCylinder(double radius,double height,int segments = segments)
{
    height = height/2;
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

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,height);
			glVertex3f(points[i].x,points[i].y,height);

			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,-height);
			glVertex3f(points[i].x,points[i].y,-height);
        }
        glEnd();
    }
}

void drawSpike(double height, double radius){

    glColor3f(0.6,0.6,0.6);

	glBegin(GL_QUADS);{
		glVertex3f(0, height, radius);
		glVertex3f(0, -height, radius);
		glVertex3f(0, -height, -radius);
		glVertex3f(0, height, -radius);
	}glEnd();

}
void drawSpikes(double height, double radius){

    glPushMatrix();
    drawSpike(height, radius);
    glPopMatrix();
    glPushMatrix();
    glRotatef(-90, 0, 1, 0);
    drawSpike(height, radius);
    glPopMatrix();

}

void drawWheel()
{
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    drawCylinder(radius,height);
    glPopMatrix();

    glPushMatrix();
    drawSpikes(height/4, radius);
    glPopMatrix();
}

void drawSS()
{
    glTranslatef(0, 0, radius);
    glTranslatef(0, height/2, 0);
    glTranslatef(forward_backward.x, forward_backward.y, forward_backward.z);

    glRotatef(leftRightRotate_angle, 0, 0, 1);
    glRotatef(rotate_angle, 0, 1, 0);

    glPushMatrix();
    drawWheel();
    glPopMatrix();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case 'w':
            rotate_angle-=rotate_angleStep;
            forward_backward.x -= one_click_dist *(cos(leftRightRotate_angle*pi/180));
            forward_backward.y -= one_click_dist *(sin(leftRightRotate_angle*pi/180));
			break;
        case 's':
            rotate_angle+=rotate_angleStep;
            forward_backward.x += one_click_dist *(cos(leftRightRotate_angle*pi/180));
            forward_backward.y += one_click_dist *(sin(leftRightRotate_angle*pi/180));
			break;
        case 'a':
            leftRightRotate_angle+=leftRightRotateStep;
			break;
        case 'd':
            leftRightRotate_angle-=leftRightRotateStep;
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;
		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=drawaxes;
			}
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
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(pos.x, pos.y, pos.z,   pos.x + l.x, pos.y + l.y, pos.z + l.z,   u.x, u.y, u.z);
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
	drawgrid=1;
	drawaxes=0;
	cameraHeight=100.0;
	cameraAngle=1.0;
	angle=0;

	forward_backward.x = 0;
	forward_backward.y = 0;
	forward_backward.z = 0;

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
	gluPerspective(60,	1,	1,	1000.0);
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

	glutCreateWindow("1705071_Wheel");

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
