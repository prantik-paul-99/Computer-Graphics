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
double grnAngle;
double ylwAngle;
double blueAngle;
double pinkAngle;
double pstAngle;

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


void drawSquare(double a)
{
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
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

	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight, 0,0,0, 0,0,1);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

	///red
	glPushMatrix();
	{
        glColor3f(1.0,0.0,0.0);
        drawSquare(16);
	}
	glPopMatrix();

	///blue
    glPushMatrix();
	{
        glColor3f(0.0,0.0,1.0);
        glTranslatef(-16, 0, 0);
        glRotatef(blueAngle, 0, 1, 0);
        glTranslatef(-16,0,0);
        drawSquare(16);
	}
	glPopMatrix();


	glPushMatrix();
	{
        ///pink
        glColor3f(1.0, 0, 1.0);
        glTranslatef(16, 0, 0);

        /// this rotation will affect the below drawings before pop
        glRotatef(-pinkAngle, 0, 1, 0);
        glTranslatef(16,0,0);

        drawSquare(16);

        ///paste
        glColor3f(0,1.0,1.0);
        glTranslatef(16, 0, 0);
        glRotatef(-pstAngle, 0, 1, 0);
        glTranslatef(16,0,0);
        drawSquare(16);
	}
	glPopMatrix();

	///yellow
    glPushMatrix();
	{
        glColor3f(1,1,0);
        glTranslatef(0, -16, 0);
        glRotatef(-ylwAngle, 1, 0, 0);
        glTranslatef(0,-16,0);
        drawSquare(16);
	}
	glPopMatrix();

	///green
    glPushMatrix();
	{
        glColor3f(0,1,0);
        glTranslatef(0, 16, 0);
        glRotatef(grnAngle, 1, 0, 0);

        /// as the pivot is 16 units far from the origin(0,0,0),
        /// we have to translate the square by 16 units, so that after
        /// 90 deg rotation the square's pivot is aligned with z axis
        glTranslatef(0,16,0);
        drawSquare(16);
	}
	glPopMatrix();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
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
    grnAngle = 0;
    ylwAngle = 0;
    blueAngle = 0;
    pinkAngle = 0;
    pstAngle = 0;

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
	gluPerspective(50,	1,	1,	1000);

	/** ------------- my change ends here ------------------ **/


	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case '1':
            if(grnAngle < 90){
                grnAngle += 5;
            }

            break;

        case '2':
            if(grnAngle > 0){
                grnAngle -= 5;
            }

            break;

        case '3':
            if(blueAngle < 90){
                blueAngle += 5;
            }

            break;

        case '4':
            if(blueAngle > 0){
                blueAngle -= 5;
            }

            break;

        case '5':
            if(ylwAngle < 90){
                ylwAngle += 5;
            }

            break;

        case '6':
            if(ylwAngle > 0){
                ylwAngle -= 5;
            }

            break;

        case '7':
            if(pinkAngle < 90){
                pinkAngle += 5;
            }

            break;

        case '8':
            if(pinkAngle > 0){
                pinkAngle -= 5;
            }

            break;

        case '9':
            if(pstAngle < 90){
                pstAngle += 5;
            }

            break;

        case '0':
            if(pstAngle > 0){
                pstAngle -= 5;
            }

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

	glutCreateWindow("Building Cube");

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
