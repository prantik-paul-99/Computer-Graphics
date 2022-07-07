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
double lineLen;
double triBase;
double triMedian;
int rotDir;
double rotSpeed;
double transDist;
double rotAngle;
double lrAngle;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{

		    for(int i=0;i<66;i=i+2){
                glVertex3f(100-3*i,0,0);
                glVertex3f(100-3*(i+1),0,0);
		    }

            for(int i=0;i<66;i=i+2){
                glVertex3f(0,100-3*i,0);
                glVertex3f(0,100-3*(i+1),0);
		    }

            //glVertex3f( 100,0,0);
            //glVertex3f(-100,0,0);

			//glVertex3f(0,-100,0);
			//glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);

		}glEnd();
	}
}


void drawGrid()
{
	int i;
	drawgrid = 1;
	if(drawgrid==1)
	{
		glColor3f(0.5, 0.5, 0.5);	//grey
		glBegin(GL_LINES);{
			for(i=-10;i<=10;i++){

				//lines parallel to Y-axis
				glVertex3f(i*10, -110, 0);
				glVertex3f(i*10,  110, 0);

				//lines parallel to X-axis
				glVertex3f(-110, i*10, 0);
				glVertex3f( 110, i*10, 0);
			}
		}glEnd();
	}
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


void drawSquare(double a)
{
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}

void drawRectangle(double a, double b, double c, double d){
    /// bottom left point (a,b)
    /// top right point (c,d)

    glBegin(GL_QUADS);{
        glVertex3f(a,b,0);
        glVertex3f(c,b,0);
        glVertex3f(c,d,0);
        glVertex3f(a,d,0);
    }
    glEnd();

}

void drawTriangle(double a, double b, double c, double d, double e, double f)
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a, b, 0);
        glVertex3f(c, d, 0);
        glVertex3f(e, f, 0);
    }
    glEnd();
}

void drawLine(double a, double b, double c, double d)
{
    glBegin(GL_LINES);
    {
        glVertex3f(a, b, 0);
        glVertex3f(c, d, 0);
    }
    glEnd();
}

void drawArrow(double lineLen, double triBase, double triMedian){
    glColor3f(1.0, 1.0, 1.0);
    drawLine(10,0, 10+lineLen,0);
    glColor3f(1.0, 1.0, 0);
    drawTriangle(10,triBase/2.0, 10,-triBase/2.0, 10-triMedian,0);
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(0,0,200,	0,0,0,	0,1,0);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight, 0,0,0, 0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

	glPushMatrix();
	{
        drawCircle(100, 50);
	}
	glPopMatrix();

    glPushMatrix();
	{
	    glRotatef(lrAngle, 0,0,1);
	    glTranslatef(transDist,0,0);
        glRotatef(rotAngle, 0,1,0);
        drawArrow(lineLen, triBase, triMedian);
	}
	glPopMatrix();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;


    transDist += rotSpeed * (rotDir);
    if(transDist > 99 || transDist < -99){
        rotDir = (-1) * rotDir;
        rotAngle = 180-rotAngle;

        if(lrAngle != 0){
            lrAngle *= 1.1;
        }
    }
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;

    cameraHeight=150.0;
	cameraAngle=0.5;

	angle=0;


	/** ------------- my change starts here ------------------ **/
    lineLen = 16;
    triBase = lineLen;
    triMedian = lineLen;
    rotDir = -1;
    rotSpeed = 0.08;
    transDist = 0;
    rotAngle = 0;
    lrAngle = 0;

	/** ------------- my change ends here ------------------ **/

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();


	/** ------------- my change starts here ------------------ **/

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000);

	/** ------------- my change ends here ------------------ **/


	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case '1':
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key

            if(lineLen > 0){
                lineLen -= 2;
                triBase -= 2;
                triMedian -= 2;
                rotSpeed -= 0.01;
            }

			break;

		case GLUT_KEY_UP:		// up arrow key
			if(lineLen < 28){
                lineLen += 2;
                triBase += 2;
                triMedian += 2;
                rotSpeed += 0.01;
			}

			break;

		case GLUT_KEY_RIGHT:
            lrAngle -= 1;

			break;

		case GLUT_KEY_LEFT:
            lrAngle += 1;

			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
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

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Moving Arrow");

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
