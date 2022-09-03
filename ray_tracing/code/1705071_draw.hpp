int slices = 50;
int stacks = 50;
int segments = 50;

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

