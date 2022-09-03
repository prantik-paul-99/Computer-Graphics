struct point
{
	double x,y,z;
};

point pos,u,r,l;
double rotate_angle = pi/180.0;

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
