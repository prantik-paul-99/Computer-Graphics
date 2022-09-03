#define infinity 9999999999.0
#define move_forward_const 0.000001

class Object;
class PointLight;
class SpotLight;

int rec_lvl;
vector<Object*> objects;
vector<PointLight*> pointlights;
vector<SpotLight*> spotlights;

/// Ray class

class Ray
{
    point start, dir;
public:
    Ray(point s, point d)
    {
        start = s;
        dir = normalize(d);
    }

    point get_ray_start() {return start;}
    point get_ray_dir() {return dir;}
};

/// Lights
class PointLight
{
    point light_pos;
    vector<double> color;

public:
    PointLight(point pos, vector<double> col)
    {
        light_pos = pos;
        color = col;
    }
    point getPosition() {return light_pos;}
    vector<double> getColor() {return color;}

    void draw()
    {
        drawSphere(getPosition(), 0.5, getColor());
    }

};


class SpotLight
{
    PointLight* point_light;
    point light_dir;
    double cutoff_angle;

public:
    SpotLight(PointLight* pnt_light, point dir, double angle)
    {
        point_light = pnt_light;
        light_dir = normalize(dir);
        cutoff_angle = angle;
    }
    PointLight* getPointlight() {return point_light;}
    point getLightDirection() {return light_dir;}
    double getCutoffAngle() {return cutoff_angle;}

    void draw()
    {
        drawSphere(point_light->getPosition(), 1.5, point_light->getColor());
    }
    delete_pointlight()
    {
        delete this->point_light;
    }
};

vector<double> clip_color(vector<double> color)
{
    for(int i = 0; i < 3; i++)
    {
        if(color[i]<0.0) color[i] = 0.0;
        if(color[i]>1.0) color[i] = 1.0;
    }
    return color;
}

class Object
{
    point reference_point;
    double height, width, length;
    vector<double> color;
    vector<double> coefficients; /// ambience, diffusion, specularity, reflection
    int shine;

public:

    Object() = default;

    double intersect(Ray& r, vector<double>& color, int level);

    virtual void draw(){}
    virtual double get_intersect_param(Ray& r, vector<double>& color, int level){return -1.0;}
    virtual point get_normal(point a){point n; return n;}
    virtual vector<double> getColor_at_point(point a){vector<double> temp; return temp;}

    void setReferencePoint(point a)
    {
        reference_point = a;
    }
    void setLength(double a)
    {
        length = a;
    }
    void setWidth(double a)
    {
        width = a;
    }
    void setHeight(double a)
    {
        height = a;
    }
    void setColor(vector<double> clr)
    {
        color = clr;
    }

    void setShine(int s)
    {
        shine = s;
    }
    void setCoefficients(vector<double> coeffs)
    {
        coefficients = coeffs;
    }

    point getReferencePoint() {return reference_point;}
    double getLength() {return length;}
    double getWidth() {return width;}
    double getHeight() {return height;}
    vector<double> getColor() {return color;}
    vector<double> getCoefficients() {return coefficients;}
    int getShine() {return shine;}

    bool check_if_in_shadow(point light_pos, point light_dir, point intersection_point, double t_min_light)
    {
        point shadow_point = add_points(light_pos, multiply_point(multiply_point(light_dir,-1.0),t_min_light));

        if(get_distance(intersection_point,light_pos)-move_forward_const > get_distance(shadow_point,light_pos)) return true;
        return false;
    }
};

double Object :: intersect(Ray& r, vector<double>& color, int level)
{
    double t_min = this->get_intersect_param(r, color, level);

    if(level == 0) return t_min;

    point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir();

    point intersection_point = add_points(ray_start, multiply_point(ray_dir, t_min));
    point normal = this->get_normal(intersection_point);

    if(dot_multiply_points(normal,multiply_point(ray_dir,-1.0)) < 0.0)
    {
        normal = multiply_point(normal,-1.0);
    }

    ///ambient coefficient work

    color = this->getColor_at_point(intersection_point);

    color[0] = color[0] * getCoefficients()[0];
    color[1] = color[1] * getCoefficients()[0];
    color[2] = color[2] * getCoefficients()[0];

    /// lighting for pointlights

    for(int i = 0; i < pointlights.size(); i++)
    {
        ///cout<<"in point lighting"<<endl;

        point pntlight_pos = pointlights[i]->getPosition(), pntlight_dir = subtract_points(pntlight_pos, intersection_point);
        pntlight_dir = normalize(pntlight_dir);
        pntlight_pos = add_points(pntlight_pos,multiply_point(pntlight_dir, move_forward_const)); ///slightly move forward

        Ray pntlight_ray(pntlight_pos, multiply_point(pntlight_dir, -1.0));

        /// check if is in shadow

        double t_light, t_min_light = infinity;

        for(int j = 0; j < objects.size(); j++)
        {
            vector<double> temp;
            t_light = objects[j]->intersect(pntlight_ray, temp, 0);

            if(t_light > 0.0 && t_light < t_min_light)
            {
                t_min_light = t_light;
            }
        }

        bool in_shadow = check_if_in_shadow(pntlight_pos, pntlight_dir, intersection_point, t_min_light);

        if(t_min_light == infinity || in_shadow) continue;

        double lmbrt_val = max(dot_multiply_points(normal, pntlight_dir), 0.0);

        color[0] += (pointlights[i]->getColor()[0] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[0]);
        color[1] += (pointlights[i]->getColor()[1] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[1]);
        color[2] += (pointlights[i]->getColor()[2] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[2]);

        point light_ray_refl = subtract_points(pntlight_ray.get_ray_dir(), multiply_point(normal, 2.0*dot_multiply_points(normal,pntlight_ray.get_ray_dir())));
        light_ray_refl = normalize(light_ray_refl);

        double phong_val = max(pow(dot_multiply_points(light_ray_refl,multiply_point(ray_dir,-1.0)),getShine()),0.0);

        color[0] += (pointlights[i]->getColor()[0] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[0]);
        color[1] += (pointlights[i]->getColor()[1] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[1]);
        color[2] += (pointlights[i]->getColor()[2] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[2]);

        ///cout<<"lambert "<<lmbrt_val<<" phong "<<phong_val<<endl;
    }

    /// lighting for spotlights

    for(int i = 0; i < spotlights.size(); i++)
    {
        ///cout<<"in spot lighting"<<endl;
        point sptlight_pos = spotlights[i]->getPointlight()->getPosition(), sptlight_dir =spotlights[i]->getLightDirection();
        sptlight_pos = add_points(sptlight_pos,multiply_point(sptlight_dir, move_forward_const)); ///slightly move forward

        /// check angle
        point pos_to_point = subtract_points(intersection_point,sptlight_pos);
        double dot_product = dot_multiply_points(sptlight_dir, pos_to_point);
        double det1 = pow(sptlight_dir.x,2.0) + pow(sptlight_dir.y,2.0) + pow(sptlight_dir.z,2.0);
        double det2 = pow(pos_to_point.x,2.0) + pow(pos_to_point.y,2.0) + pow(pos_to_point.z,2.0);

        ///cout<<dot_product<<" "<<det1<<" "<<det2<<endl;

        double angle = acos(dot_product/sqrt(det1 * det2)) * 180.0 / pi;
        ///cout<<angle<<endl;
        if(angle > spotlights[i]->getCutoffAngle()) continue;


        Ray sptlight_ray(sptlight_pos, pos_to_point);

        /// check if is in shadow

        double t_light, t_min_light = infinity;

        for(int j = 0; j < objects.size(); j++)
        {
            vector<double> temp;
            t_light = objects[j]->intersect(sptlight_ray, temp, 0);

            if(t_light > 0.0 && t_light < t_min_light)
            {
                t_min_light = t_light;
            }
        }

        bool in_shadow = check_if_in_shadow(sptlight_pos, multiply_point(sptlight_ray.get_ray_dir(), -1.0), intersection_point, t_min_light);

        if(t_min_light == infinity || in_shadow) continue;

        point actual_dir = multiply_point(sptlight_ray.get_ray_dir(),-1.0);
        double lmbrt_val = max(dot_multiply_points(normal, actual_dir) , 0.0);

        point light_ray_refl = subtract_points(sptlight_ray.get_ray_dir(), multiply_point(normal, 2.0*dot_multiply_points(normal,sptlight_ray.get_ray_dir())));
        light_ray_refl = normalize(light_ray_refl);

        double phong_val = max(pow(dot_multiply_points(light_ray_refl,multiply_point(ray_dir,-1.0)),getShine()),0.0);

        ///cout<<lmbrt_val<<" lmbrt"<<endl;

        color[0] += (spotlights[i]->getPointlight()->getColor()[0] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[0]);
        color[1] += (spotlights[i]->getPointlight()->getColor()[1] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[1]);
        color[2] += (spotlights[i]->getPointlight()->getColor()[2] * getCoefficients()[1] * lmbrt_val * this->getColor_at_point(intersection_point)[2]);

        color[0] += (spotlights[i]->getPointlight()->getColor()[0] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[0]);
        color[1] += (spotlights[i]->getPointlight()->getColor()[1] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[1]);
        color[2] += (spotlights[i]->getPointlight()->getColor()[2] * getCoefficients()[2] * phong_val * this->getColor_at_point(intersection_point)[2]);

        ///cout<<phong_val<<" phong"<<endl;
    }

    if(level >= rec_lvl) return t_min;


    /// reflection
    point refl_dir = subtract_points(ray_dir, multiply_point(normal, 2.0*dot_multiply_points(normal,ray_dir)));
    refl_dir = normalize(refl_dir);

    ///cout<<"ref "<<refl_dir.x<<" "<<refl_dir.y<<" "<<refl_dir.z<<endl;

    Ray refl_ray(add_points(intersection_point, multiply_point(refl_dir, move_forward_const)), refl_dir);

    /// t_min from nearest reflecting object
    int nearest = -1;
    double t_refl, t_min_refl = infinity;

    vector<double> color_temp;

    for(int k = 0; k < objects.size(); k++)
    {
        t_refl = objects[k]->intersect(refl_ray, color_temp, 0);

        if(t_refl > 0.0 && t_refl < t_min_refl)
        {
            t_min_refl = t_refl;
            nearest = k;
        }
    }

    if(nearest != -1)
    {
        t_min_refl = objects[nearest]->intersect(refl_ray, color_temp, level+1);
        color[0] += color_temp[0] * getCoefficients()[3];
        color[1] += color_temp[1] * getCoefficients()[3];
        color[2] += color_temp[2] * getCoefficients()[3];
    }
    color = clip_color(color);

    ///cout<<"color "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;

    return t_min;
}

class Sphere : public Object
{
public:
    Sphere(point center, double radius)
    {
        setReferencePoint(center);
        setLength(radius);
    }
    void draw()
    {
        point center = getReferencePoint();

        drawSphere(center, getLength(),getColor());
    }

    point get_normal(point a)
    {
        point temp = subtract_points(a,getReferencePoint());
        temp = normalize(temp);
        return temp;
    }

    vector<double> getColor_at_point(point a)
    {
        return getColor();
    }

    double get_intersect_param(Ray &r, vector<double> &color, int level)
    {

        ///cout<<infinity<<endl;

        point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir(), center = getReferencePoint();

        /// rd.rd.t^2(a) + 2.rd.(ro-c)(b) +(ro-c)^2-r^2(c)
        double a, b, c, t_min = -1.0, radius = getLength(), val;

        a = dot_multiply_points(ray_dir,ray_dir);
        b = 2.0 * dot_multiply_points(ray_dir, subtract_points(ray_start,center));
        c = dot_multiply_points(subtract_points(ray_start,center),subtract_points(ray_start,center)) - radius*radius;

        ///cout<<"abc"<<" "<<a<<" "<<b<<" "<<c<<endl;
        val = b*b - 4*a*c;

        if(val >= 0.0)
        {
            double val1, val2;

            val1 = (-b+sqrt(val))/(2.0 * a);
            val2 = (-b-sqrt(val))/(2.0 * a);

            if(val2 > 0.0) t_min = val2;
            else t_min = val1;

        }

        return t_min;
    }
};

class Triangle : public Object
{
    point vertex1, vertex2, vertex3;
public:

    Triangle(point v1, point v2, point v3)
    {
        vertex1 = v1;
        vertex2 = v2;
        vertex3 = v3;
    }

    void draw()
    {
        drawTriangle(vertex1, vertex2, vertex3, getColor());
    }

    point get_normal(point a)
    {
        point normal;
        point e1 = subtract_points(vertex2, vertex1), e2 = subtract_points(vertex3, vertex1);
        normal = cross_multiply_points(e1, e2);
        normal = normalize(normal);
        return normal;
    }

    vector<double> getColor_at_point(point a)
    {
        return getColor();
    }

    double get_determinant(double mat[][3])
    {
        double x = ((mat[1][1] * mat[2][2]) - (mat[2][1] * mat[1][2]));
        double y = ((mat[1][0] * mat[2][2]) - (mat[2][0] * mat[1][2]));
        double z = ((mat[1][0] * mat[2][1]) - (mat[2][0] * mat[1][1]));

	    double determinant = ((mat[0][0] * x) - (mat[0][1] * y) + (mat[0][2] * z));

        return determinant;
    }

    double get_intersect_param(Ray& r, vector<double>& color, int level)
    {
        /// build matrices and calculate beta gamma t
        point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir();

        double beta, gamma, t_min, a, beta_up, gamma_up, t_up;

        double temp[3][3];

        /// A
        temp[0][0] = vertex1.x-vertex2.x;
        temp[0][1] = vertex1.x-vertex3.x;
        temp[0][2] = ray_dir.x;

        temp[1][0] = vertex1.y-vertex2.y;
        temp[1][1] = vertex1.y-vertex3.y;
        temp[1][2] = ray_dir.y;

        temp[2][0] = vertex1.z-vertex2.z;
        temp[2][1] = vertex1.z-vertex3.z;
        temp[2][2] = ray_dir.z;

        a = get_determinant(temp);

        /// Beta_up
        temp[0][0] = vertex1.x-ray_start.x;
        temp[0][1] = vertex1.x-vertex3.x;
        temp[0][2] = ray_dir.x;

        temp[1][0] = vertex1.y-ray_start.y;
        temp[1][1] = vertex1.y-vertex3.y;
        temp[1][2] = ray_dir.y;

        temp[2][0] = vertex1.z-ray_start.z;
        temp[2][1] = vertex1.z-vertex3.z;
        temp[2][2] = ray_dir.z;

        beta_up = get_determinant(temp);

        /// Gamma_up
        temp[0][0] = vertex1.x-vertex2.x;
        temp[0][1] = vertex1.x-ray_start.x;
        temp[0][2] = ray_dir.x;

        temp[1][0] = vertex1.y-vertex2.y;
        temp[1][1] = vertex1.y-ray_start.y;
        temp[1][2] = ray_dir.y;

        temp[2][0] = vertex1.z-vertex2.z;
        temp[2][1] = vertex1.z-ray_start.z;
        temp[2][2] = ray_dir.z;

        gamma_up = get_determinant(temp);

        /// t_up
        temp[0][0] = vertex1.x-vertex2.x;
        temp[0][1] = vertex1.x-vertex3.x;
        temp[0][2] = vertex1.x-ray_start.x;

        temp[1][0] = vertex1.y-vertex2.y;
        temp[1][1] = vertex1.y-vertex3.y;
        temp[1][2] = vertex1.y-ray_start.y;

        temp[2][0] = vertex1.z-vertex2.z;
        temp[2][1] = vertex1.z-vertex3.z;
        temp[2][2] = vertex1.z-ray_start.z;

        t_up = get_determinant(temp);

        if(a == 0.0)
        {
            return -1.0;
        }

        beta = beta_up / a;
        gamma = gamma_up / a;
        t_min = t_up / a;

        ///cout<<a<<" "<<beta<<" "<<gamma<<" "<<t_min<<endl;

        if( beta > 0.0 && gamma > 0.0 && (beta+gamma) < 1.0 && t_min > 0.0)
        {
            return t_min;
        }
        return -1.0;
    }


};

class General_Quad : public Object
{
    vector<double> general_quad_coefficients;
public:

    General_Quad(vector<double> coeffs, point ref_pnt, double l, double w, double h)
    {
        general_quad_coefficients = coeffs;
        setReferencePoint(ref_pnt);
        setLength(l);
        setWidth(w);
        setHeight(h);
    }

    void draw()
    {

    }

    point get_normal(point a)
    {
        point normal;
        normal.x = 2.0*general_quad_coefficients[0]*a.x + general_quad_coefficients[3]*a.y + general_quad_coefficients[4]*a.z + general_quad_coefficients[6];
        normal.y = 2.0*general_quad_coefficients[1]*a.y + general_quad_coefficients[3]*a.x + general_quad_coefficients[5]*a.z + general_quad_coefficients[7];
        normal.x = 2.0*general_quad_coefficients[2]*a.z + general_quad_coefficients[4]*a.x + general_quad_coefficients[5]*a.y + general_quad_coefficients[8];
        normal = normalize(normal);

        return normal;
    }

    vector<double> getColor_at_point(point a)
    {
        return getColor();
    }

    bool point_in_ref_cube(point a)
    {
        if(getLength()!=0 && (a.x < getReferencePoint().x || a.x > getReferencePoint().x+getLength())) return false;
        if(getWidth()!=0 && (a.y < getReferencePoint().y || a.y > getReferencePoint().y+getWidth())) return false;
        if(getHeight()!=0 && (a.z < getReferencePoint().z || a.z > getReferencePoint().z+getHeight())) return false;

        return true;
    }

    double determine_t_min_by_clipping(double negve, double posve, Ray& r)
    {
        if(negve < 0.0 && posve < 0.0) return -1.0;

        point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir();

        point p_neg = add_points(ray_start, multiply_point(ray_dir, negve));
        point p_pos = add_points(ray_start, multiply_point(ray_dir, posve));

        if(negve>0.0 && point_in_ref_cube(p_neg)) return negve;
        else if(posve>0.0 && point_in_ref_cube(p_pos)) return posve;
        else return -1.0;

    }

    double get_intersect_param(Ray& r, vector<double>& color, int level)
    {
        point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir();

        double a, b, c, d, val, t_min = infinity;

        a = general_quad_coefficients[0]*pow(ray_dir.x,2.0) + general_quad_coefficients[1]*pow(ray_dir.y,2.0) + general_quad_coefficients[2]*pow(ray_dir.z,2.0) + general_quad_coefficients[3]*ray_dir.x*ray_dir.y + general_quad_coefficients[4]*ray_dir.x*ray_dir.z + general_quad_coefficients[5]*ray_dir.y*ray_dir.z;
        b = general_quad_coefficients[0]*ray_start.x*ray_dir.x*2.0 + general_quad_coefficients[1]*ray_start.y*ray_dir.y*2.0 + general_quad_coefficients[2]*ray_start.z*ray_dir.z*2.0 + general_quad_coefficients[3]*(ray_dir.x*ray_start.y+ray_dir.y*ray_start.x) + general_quad_coefficients[4]*(ray_dir.x*ray_start.z+ray_dir.z*ray_start.x) + general_quad_coefficients[5]*(ray_dir.y*ray_start.z+ray_dir.z*ray_start.y) + general_quad_coefficients[6]*ray_dir.x + general_quad_coefficients[7]*ray_dir.y + general_quad_coefficients[8]*ray_dir.z;
        c = general_quad_coefficients[0]*pow(ray_start.x,2.0) + general_quad_coefficients[1]*pow(ray_start.y,2.0) + general_quad_coefficients[2]*pow(ray_start.z,2.0) + general_quad_coefficients[3]*ray_start.x*ray_start.y + general_quad_coefficients[4]*ray_start.x*ray_start.z + general_quad_coefficients[5]*ray_start.y*ray_start.z + general_quad_coefficients[6]*ray_start.x + general_quad_coefficients[7]*ray_start.y + general_quad_coefficients[8]*ray_start.z + general_quad_coefficients[9];

        if(a == 0.0) return infinity;

        val = pow(b,2.0) - 4*a*c;

        if(val >= 0.0)
        {
            double val1, val2;

            val1 = (-b-sqrt(val))/(2.0 * a);
            val2 = (-b+sqrt(val))/(2.0 * a);

            t_min = determine_t_min_by_clipping(val1, val2, r);

        }

        return t_min;

    }
};

/// Floor Object

class Floor: public Object
{
    double floor_width, tile_width;
public:
    Floor(double f_w, double t_w)
    {
        floor_width = f_w;
        tile_width = t_w;
    }

    void draw()
    {
        drawFloor(floor_width, tile_width, getColor());
    }

    point get_normal(point a)
    {
        point temp;
        temp.x = 0;
        temp.y = 0;
        temp.z = 1;
        return temp;
    }

    bool is_point_on_floor(point a)
    {
        return ((a.x > -floor_width/2.0) && (a.x < floor_width/2.0) && (a.y > -floor_width/2.0) && (a.y < floor_width/2.0));
    }

    vector<double> getColor_at_point(point a)
    {
        vector<double> color;
        color.push_back(0);
        color.push_back(0);
        color.push_back(0);

        if (!is_point_on_floor(a)) return color;

        int i = floor((floor_width/2.0 + a.x) / tile_width);
        int j = floor((floor_width/2.0 + a.y) / tile_width);

        if((i%2 == 0 && j%2 == 0) || (i%2 == 1 && j%2 == 1))
        {
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }
        else
        {
            color[0] = getColor()[0];
            color[1] = getColor()[1];
            color[2] = getColor()[2];
        }

        return color;
    }

    double get_intersect_param(Ray& r, vector<double>& color, int level)
    {
        point ray_start = r.get_ray_start(), ray_dir = r.get_ray_dir(), normal = this->get_normal(ray_start);
        point intersection_point;

        double t_min;

        /// by ray plane intersection method

        if(dot_multiply_points(ray_dir,normal) != 0.0)
        {
            t_min = dot_multiply_points(ray_start,normal)/dot_multiply_points(ray_dir,normal)*(-1.0);
        }
        else t_min = -1.0;

        {
            intersection_point = add_points(multiply_point(ray_dir,t_min), ray_start);

            if(!is_point_on_floor(intersection_point)) t_min = -1.0;
        }

        return t_min;

    }
};

