#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

#include <bits/stdc++.h>

using namespace std;

#include "1705071.hpp"
#include "1705071_classes.hpp"
#include "bitmap_image.hpp"

double fovy = 80.0, aspect = 1.0, znear = 1.0, zfar = 1000.0;

extern int rec_lvl;
int scrn_sz, obj_num, lght_num, wind_sz = 600;

int img_count = 0;

extern vector<Object*> objects;
extern vector<PointLight*> pointlights;
extern vector<SpotLight*> spotlights;

vector<double> color;

void loadData()
{
    ifstream infile;
    string obj_name;

    infile.open("E:\\Ondrive\\OneDrive - BUET\\study\\4-1\\CSE 410\\Ray Tracing\\ray_tracing2\\scene.txt");

    if(!infile.is_open()) {
        cout << "Failed to open file" << endl;
    }

    infile >> rec_lvl >> scrn_sz >> obj_num;

    for (int cnt = 0; cnt < obj_num; cnt++)
    {

        infile >> obj_name;
        Object* temp;

        if(obj_name == "sphere")
        {
            point cntr;
            double rad;

            infile >> cntr.x >> cntr.y >> cntr.z >> rad;

            temp = new Sphere(cntr, rad);
        }

        else if(obj_name == "triangle")
        {
            point v1, v2, v3;

            infile >> v1.x >> v1.y >> v1.z;
            infile >> v2.x >> v2.y >> v2.z;
            infile >> v3.x >> v3.y >> v3.z;

            temp = new Triangle(v1, v2, v3);
        }
        else if (obj_name == "general")
        {
            vector<double> coeffs;
            point ref_point;
            double l, w, h;

            for(int i = 0; i < 10; i++)
            {
                double val;
                infile >> val;
                coeffs.push_back(val);
            }
            infile >> ref_point.x >> ref_point.y >> ref_point.z;
            infile >> l >> w >> h;

            temp = new General_Quad(coeffs, ref_point, l, w, h);

        }

        color.clear();
        for(int i = 0; i < 3; i++)
        {
            double val;
            infile >> val;
            color.push_back(val);
        }
        temp->setColor(color);

        vector<double> coeffs;
        for(int i = 0; i < 4; i++)
        {
            double val;
            infile >> val;
            coeffs.push_back(val);
        }
        temp->setCoefficients(coeffs);

        int shine;
        infile >> shine;
        temp->setShine(shine);

        objects.push_back(temp);
    }

    int pointlight_cnt, spotlight_cnt;

    infile >> pointlight_cnt;
    PointLight* temp_pntlight;

    for(int cnt = 0; cnt < pointlight_cnt; cnt++)
    {
        point pos;
        color.clear();

        infile >> pos.x >> pos.y >> pos.z;
        for(int i = 0; i < 3; i++)
        {
            double val;
            infile >> val;
            color.push_back(val);
        }

        temp_pntlight = new PointLight(pos, color);
        pointlights.push_back(temp_pntlight);
    }

    infile >> spotlight_cnt;
    SpotLight* temp_sptlight;

    for(int cnt = 0; cnt < spotlight_cnt; cnt++)
    {
        point pos, dir;
        color.clear();
        double angle;

        infile >> pos.x >> pos.y >> pos.z;
        for(int i = 0; i < 3; i++)
        {
            double val;
            infile >> val;
            color.push_back(val);
        }

        temp_pntlight = new PointLight(pos, color);

        infile >> dir.x >> dir.y >> dir.z;
        infile >> angle;

        temp_sptlight = new SpotLight(temp_pntlight, dir, angle);
        spotlights.push_back(temp_sptlight);
    }

    /// creating floor

    color.clear();
    color.push_back(0);
    color.push_back(0);
    color.push_back(0);

    Object* temp = new Floor(1000,20);
    temp->setColor(color);
    vector<double> coeffs;
    coeffs.push_back(0.25);
    coeffs.push_back(0.25);
    coeffs.push_back(0.25);
    coeffs.push_back(0.25);
    temp->setCoefficients(coeffs);
    temp->setShine(15);

    objects.push_back(temp);

}

void capture()
{
    cout<<"Starting Capture"<<endl;
    bitmap_image img(scrn_sz,scrn_sz);

    for(int i = 0; i < scrn_sz; i++)
    {
        for(int j = 0; j < scrn_sz; j++)
        {
            img.set_pixel(j, i, 0, 0, 0);
        }
    }


    double plane_dist, du, dv, t, t_min;
    int nearest;
    point top_lft;

    plane_dist = (wind_sz/2.0)/(tan((fovy/2.0)*pi/180.0));
    point temp = subtract_points(multiply_point(u, wind_sz/2.0), multiply_point(r, wind_sz/2.0));
    temp = add_points(multiply_point(l, plane_dist), temp);
    top_lft = add_points(pos, temp);
    du = dv = ((double) wind_sz/scrn_sz);
    temp = subtract_points(multiply_point(r,du/2.0), multiply_point(u,dv/2.0));
    top_lft = add_points(top_lft,temp);


    for(int j = 0; j < scrn_sz; j++)
    {
        for(int i = 0; i < scrn_sz; i++)
        {
            ///calculate curPixel using topleft,r,u,i,j,du,dv
            point temp = subtract_points(multiply_point(r,j*du), multiply_point(u,i*dv));
            point cur_pix = add_points(top_lft, temp);

            Ray ray(pos, subtract_points(cur_pix, pos));

            nearest = -1;
            t_min = infinity;

            for(int k = 0; k < objects.size(); k++)
            {
                color.clear();
                color.push_back(0);
                color.push_back(0);
                color.push_back(0);

                t = objects[k]->intersect(ray, color, 0);

                if(t > 0.0 && t < t_min)
                {
                    t_min = t;
                    nearest = k;
                }
            }


            if(nearest != -1)
            {
                t_min = objects[nearest]->intersect(ray, color, 1);
                img.set_pixel(j, i, round(color[0] * 255), round(color[1] * 255), round(color[2] * 255));
            }

        }
    }
    cout<<"Ended Capture"<<endl;

    img_count++;

    stringstream img_count_str;
    img_count_str << img_count;

    img.save_image("E:\\Ondrive\\OneDrive - BUET\\study\\4-1\\CSE 410\\Ray Tracing\\ray_tracing2\\Output_1"+img_count_str.str()+".bmp");
}



void drawSS()
{
    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->draw();
    }

    for(int i = 0; i < pointlights.size(); i++)
    {
        pointlights[i]->draw();
    }

    for(int i = 0; i < spotlights.size(); i++)
    {
        spotlights[i]->draw();
    }
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){
        case '0':
            capture();
            break;
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
	gluLookAt(pos.x, pos.y, pos.z,   pos.x + l.x, pos.y + l.y, pos.z + l.z,   u.x, u.y, u.z);

	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

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
	drawaxes=0;
	/// camera control initialize ///
	pos.x = 100;
    pos.y = 100;
    pos.z = 50;
    l.x = -1.0/sqrt(2.0);
    l.y = -1.0/sqrt(2.0);
    l.z = 0;
    u.x = 0;
    u.y = 0;
    u.z = 1;
    r.x = -1.0/sqrt(2.0);;
    r.y = 1.0/sqrt(2.0);;
    r.z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, znear, zfar);
}

int main(int argc, char **argv){
    loadData();

	glutInit(&argc,argv);
	glutInitWindowSize(wind_sz, wind_sz);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("1705071_Ray_Tracing");

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
