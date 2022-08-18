double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

int slices = 50;
int stacks = 50;
int segments = 50;

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


/// rotation and translation functions

point multiply_point(point a, double b)
{
    a.x = a.x * b;
    a.y = a.y * b;
    a.z = a.z * b;
    return a;
}

double dot_multiply_points(point a, point b)
{
    return (a.x*b.x + a.y*b.y + a.z*b.z);
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

point normalize(point a)
{
    double len = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    a.x = a.x / len;
    a.y = a.y / len;
    a.z = a.z / len;

    return a;
}

double get_distance(point a, point b)
{
    return sqrt(pow(a.x-b.x, 2.0)+pow(a.y-b.y, 2.0)+pow(a.z-b.z, 2.0));
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

void drawSphere(point center, double radius, vector<double> color)
{
    glColor3f(color[0], color[1], color[2]);
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
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(center.x+points[i][j].x,center.y+points[i][j].y,center.z+points[i][j].z);
				glVertex3f(center.x+points[i][j+1].x,center.y+points[i][j+1].y,center.z+points[i][j+1].z);
				glVertex3f(center.x+points[i+1][j+1].x,center.y+points[i+1][j+1].y,center.z+points[i+1][j+1].z);
				glVertex3f(center.x+points[i+1][j].x,center.y+points[i+1][j].y,center.z+points[i+1][j].z);
                //lower hemisphere
                glVertex3f(center.x+points[i][j].x,center.y+points[i][j].y,center.z-points[i][j].z);
				glVertex3f(center.x+points[i][j+1].x,center.y+points[i][j+1].y,center.z-points[i][j+1].z);
				glVertex3f(center.x+points[i+1][j+1].x,center.y+points[i+1][j+1].y,center.z-points[i+1][j+1].z);
				glVertex3f(center.x+points[i+1][j].x,center.y+points[i+1][j].y,center.z-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawTriangle(point a, point b, point c, vector<double> color)
{
    glColor3f(color[0] , color[1] , color[2] );

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
    }
    glEnd();
}

void drawFloor(double floor_width, double tile_width, vector<double> color)
{
    int row=(int) floor_width/tile_width, column=(int) floor_width/tile_width;

        for(int i=0; i<row; i++)
        {
            for(int j=0; j<column; j++)
            {
                if((i%2 == 0 && j%2 == 0) || (i%2 == 1 && j%2 == 1)) glColor3f(1,1,1);
                else glColor3f(color[0], color[1], color[2]);
                ///else glColor3f( 0 , 0 , 0);

                point square_vertex;
                square_vertex.x = -floor_width/2.0+tile_width*j;
                square_vertex.y = -floor_width/2.0+tile_width*i;

                glBegin(GL_QUADS);
                {
                    glVertex3f(square_vertex.x, square_vertex.y, 0);
                    glVertex3f(square_vertex.x - tile_width, square_vertex.y, 0);
                    glVertex3f(square_vertex.x - tile_width, square_vertex.y - tile_width, 0);
                    glVertex3f(square_vertex.x, square_vertex.y - tile_width, 0);
                }
                glEnd();
            }
        }
}

