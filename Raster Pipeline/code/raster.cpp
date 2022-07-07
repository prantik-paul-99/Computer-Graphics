#include<bits/stdc++.h>
//#define _USE_MATH_DEFINES
//
//#include <cmath>

#include "bitmap_image.hpp"

using namespace std;

#define pi atan(1)*4
#define threshold 0.000001

class point{
public:
    double pnt_x, pnt_y, pnt_z;

    void normalize()
    {
        double value = pow(pow(pnt_x,2)+pow(pnt_y,2)+pow(pnt_z,2), 0.5);
        pnt_x = pnt_x/value;
        pnt_y = pnt_y/value;
        pnt_z = pnt_z/value;
    }
};

class transformation_matrix{
public:

    vector<vector<double>> mat;

    void initialize()
    {
        vector<double> temp;
        for(int i = 0; i < 4; i++)
        {
            vector<double> temp;
            for (int j = 0; j < 4; j++)
            {
                temp.push_back(0.0);
            }
            mat.push_back(temp);
        }
    }

    void initialize_identity()
    {
        vector<double> temp;
        for(int i = 0; i < 4; i++)
        {
            vector<double> temp;
            for (int j = 0; j < 4; j++)
            {
                if(i==j) temp.push_back(1.0);
                else temp.push_back(0.0);
            }
            mat.push_back(temp);
        }
    }

    void initialize_matrix(vector<double> values)
    {
        mat.clear();
        int cnt = 0;

        for(int i = 0; i < 4; i++)
        {
            vector<double> temp;
            for (int j = 0; j < 4; j++)
            {
                temp.push_back(values[cnt]);
                cnt++;
            }
            mat.push_back(temp);
        }
    }

    void show_matrix()
    {
        for(int i = 0; i < mat.size(); i++)
        {
            for (int j = 0; j < mat[0].size(); j++)
            {
                cout<<mat[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    transformation_matrix multiply_matrix(transformation_matrix mat1)
    {
        transformation_matrix result;
        result.initialize();

        for(int i = 0;  i < mat.size(); i++)
        {
            for (int j = 0; j < mat1.mat[0].size(); j++)
            {
                for(int k = 0; k < mat[0].size(); k++)
                {
                    result.mat[i][j] += mat[i][k] * mat1.mat[k][j];
                }
            }
        }

        return result;
    }

    point multiply_point(point a)
    {
        point temp;
        double scale = mat[3][0]*a.pnt_x+mat[3][1]*a.pnt_y+mat[3][2]*a.pnt_z+mat[3][3];

        temp.pnt_x = (mat[0][0]*a.pnt_x+mat[0][1]*a.pnt_y+mat[0][2]*a.pnt_z+mat[0][3])/scale;
        temp.pnt_y = (mat[1][0]*a.pnt_x+mat[1][1]*a.pnt_y+mat[1][2]*a.pnt_z+mat[1][3])/scale;
        temp.pnt_z = (mat[2][0]*a.pnt_x+mat[2][1]*a.pnt_y+mat[2][2]*a.pnt_z+mat[2][3])/scale;

        return temp;
    }
};

class triangle
{
public:
    vector<point> vertices;
    vector<int> color;
};

point scalar_multiply_point(point a, double val)
{
    point temp;
    temp.pnt_x = a.pnt_x * val;
    temp.pnt_y = a.pnt_y * val;
    temp.pnt_z = a.pnt_z * val;

    return temp;
}

point add_points(point a, point b)
{
    point temp;
    temp.pnt_x = a.pnt_x + b.pnt_x;
    temp.pnt_y = a.pnt_y + b.pnt_y;
    temp.pnt_z = a.pnt_z + b.pnt_z;

    return temp;
}

point subtract_points(point a, point b)
{
    point temp;
    temp.pnt_x = a.pnt_x - b.pnt_x;
    temp.pnt_y = a.pnt_y - b.pnt_y;
    temp.pnt_z = a.pnt_z - b.pnt_z;

    return temp;
}

point cross_multiply_points(point a, point b)
{
    point temp;
    temp.pnt_x = a.pnt_y * b.pnt_z - a.pnt_z * b.pnt_y;
    temp.pnt_y = a.pnt_z * b.pnt_x- a.pnt_x * b.pnt_z;
    temp.pnt_z = a.pnt_x * b.pnt_y - a.pnt_y * b.pnt_x;

    return temp;
}

double dot_multiply_points(point a, point b)
{
    return (a.pnt_x*b.pnt_x+a.pnt_y*b.pnt_y+a.pnt_z*b.pnt_z);
}

point get_rotation_column(string dirc, point axis, double angle)
/// rodrigues formula
{

    point dir, temp, temp1;

    temp.pnt_x = temp.pnt_y = temp.pnt_z = 0;
    temp1.pnt_x = temp1.pnt_y = temp1.pnt_z = 0;

    if(dirc == "x")
    {
        dir.pnt_x = 1.0;
        dir.pnt_y = 0.0;
        dir.pnt_z = 0.0;
    }
    else if(dirc == "y")
    {
        dir.pnt_x = 0.0;
        dir.pnt_y = 1.0;
        dir.pnt_z = 0.0;

    }
    else if(dirc == "z")
    {
        dir.pnt_x = 0.0;
        dir.pnt_y = 0.0;
        dir.pnt_z = 1.0;
    }

    double val = cos(angle*pi/180.0);

    if(val<threshold && val>0) val = 0.0;

    temp = scalar_multiply_point(dir, val);

//    cout<<"temp x "<<temp.pnt_x<<endl;
//    cout<<"temp y "<<temp.pnt_y<<endl;
//    cout<<"temp z "<<temp.pnt_z<<endl;

    val = dot_multiply_points(axis, dir);
    double val1 = cos(angle*pi/180.0);

    if(val1<threshold && val1>0) val1 = 0.0;
    val *= (1-val1);
    temp = add_points(temp,scalar_multiply_point(axis, val));

//    cout<<"temp x "<<temp.pnt_x<<endl;
//    cout<<"temp y "<<temp.pnt_y<<endl;
//    cout<<"temp z "<<temp.pnt_z<<endl;

    val = sin(angle*pi/180.0);
    if(val<threshold && val>0) val = 0.0;
    //cout<<"val "<<val<<endl;
    temp1 = cross_multiply_points(axis, dir);
    temp = add_points(temp, scalar_multiply_point(temp1,val));

//    cout<<"temp x "<<temp.pnt_x<<endl;
//    cout<<"temp y "<<temp.pnt_y<<endl;
//    cout<<"temp z "<<temp.pnt_z<<endl;

    return temp;
}

vector<double> find_ys_zs_for_x(double ys, double min_x_on_scanline, vector<point> vertices)
{
    vector<double> y_z_values;

    for(int v_num = 0; v_num < 3; v_num++)
    {
        ///cout<<"checking "<<v_num+1<<" time"<<endl;
        int num_1 = v_num, num_2 = (v_num+1)%3;

        point p1, p2;
        p1 = vertices[num_1];
        p2 = vertices[num_2];

        if(p1.pnt_x == min_x_on_scanline && p2.pnt_x == min_x_on_scanline && p1.pnt_x == p2.pnt_x)
        {
            y_z_values.push_back(p1.pnt_y);
            y_z_values.push_back(p2.pnt_y);
            y_z_values.push_back(p1.pnt_z);
            y_z_values.push_back(p2.pnt_z);

            ///cout<<"returning x equal"<<endl;

            return y_z_values;
        }
        if(p1.pnt_x == p2.pnt_x)
        {
            ///cout<<"continues bec x equal"<<endl;
            continue;
        }

        double slope = (p1.pnt_y - p2.pnt_y)/(p1.pnt_x - p2.pnt_x);
        double point_slope = (ys - p2.pnt_y)/(min_x_on_scanline - p2.pnt_x);

        ///cout<<slope<<" "<<point_slope<<" "<<slope-point_slope<<endl;
        if(slope == 0)
        {
            ///cout<<"continues bec slope zero"<<endl;
            continue;
        }
        double diff = slope - point_slope;
        if (diff > 0 && diff < threshold) diff = 0;
        if (diff < 0 && diff > -1*threshold ) diff = 0;
        if(diff == 0)
        {
            y_z_values.push_back(p1.pnt_y);
            y_z_values.push_back(p2.pnt_y);
            y_z_values.push_back(p1.pnt_z);
            y_z_values.push_back(p2.pnt_z);

            ///cout<<"returning slope equal"<<endl;

            return y_z_values;
        }
    }
    if(y_z_values.size() == 0)
    {
        cout<<"zero"<<endl;
        ///cout<<min_x_on_scanline<<" "<<ys<<endl;
        ///cout<<vertices[0].pnt_x<<" "<<vertices[0].pnt_y<<endl;
        ///cout<<vertices[1].pnt_x<<" "<<vertices[1].pnt_y<<endl;
        ///cout<<vertices[2].pnt_x<<" "<<vertices[2].pnt_y<<endl<<endl;
    }
    return y_z_values;

}

transformation_matrix matrix;
vector<transformation_matrix> matrix_stack;

int main()
{
    srand(time(0));

    vector<int> push_counts;
    push_counts.push_back(0);

    int trig_count = 0;

    matrix.initialize_identity();
    ///matrix.show_matrix();

    matrix_stack.push_back(matrix);
    ///matrix_stack[matrix_stack.size()-1].show_matrix();

    ifstream in_file;
    ofstream out_file;

    string number = "1";

    in_file.open(number+"/scene.txt");
    out_file.open(number+"/stage1.txt");
//    if(!out_file.is_open()) {
//        cout<<"not opened"<<endl;
//    }

    point eye, look, up;
    double fovY, aspectRatio, near, far;

    in_file >> eye.pnt_x >> eye.pnt_y >> eye.pnt_z;
    in_file >> look.pnt_x >> look.pnt_y >> look.pnt_z;
    in_file >> up.pnt_x >> up.pnt_y >> up.pnt_z;
    in_file >> fovY >> aspectRatio >> near >> far;

    //cout<<fovY<<endl;
    string display_commands;

    in_file >> display_commands;

    /// modelling transformation

    while(!(display_commands=="end"))
    {
        //cout<<display_commands<<endl;
        if(display_commands == "triangle")
        {
            point vertex1, vertex2, vertex3;
            in_file >> vertex1.pnt_x >> vertex1.pnt_y >> vertex1.pnt_z;
            in_file >> vertex2.pnt_x >> vertex2.pnt_y >> vertex2.pnt_z;
            in_file >> vertex3.pnt_x >> vertex3.pnt_y >> vertex3.pnt_z;

            vertex1 = matrix_stack[matrix_stack.size()-1].multiply_point(vertex1);
            //cout<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;
            out_file<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;

            vertex2 = matrix_stack[matrix_stack.size()-1].multiply_point(vertex2);
            //cout<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;
            out_file<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;

            vertex3 = matrix_stack[matrix_stack.size()-1].multiply_point(vertex3);
            //cout<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;
            out_file<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;

            //cout<<endl;
            out_file<<endl;

            trig_count++;

        }

        else if(display_commands == "translate")
        {
            point translate;
            in_file >> translate.pnt_x >> translate.pnt_y >> translate.pnt_z;

            transformation_matrix temp;
            temp.initialize_identity();
            temp.mat[0][3] = translate.pnt_x;
            temp.mat[1][3] = translate.pnt_y;
            temp.mat[2][3] = translate.pnt_z;

            matrix_stack.push_back(matrix_stack[matrix_stack.size()-1].multiply_matrix(temp));

            push_counts[push_counts.size()-1]++;

//            matrix_stack[matrix_stack.size()-1].show_matrix();
//            temp.show_matrix();
//            matrix_stack[matrix_stack.size()-1].show_matrix();


        }
        else if(display_commands == "scale")
        {
            point scale;
            in_file >> scale.pnt_x >> scale.pnt_y >> scale.pnt_z;

            transformation_matrix temp;
            temp.initialize();
            temp.mat[0][0] = scale.pnt_x;
            temp.mat[1][1] = scale.pnt_y;
            temp.mat[2][2] = scale.pnt_z;
            temp.mat[3][3] = 1.0;

            //matrix_stack[matrix_stack.size()-1].show_matrix();

            //cout<<"hello"<<endl;

            matrix_stack.push_back(matrix_stack[matrix_stack.size()-1].multiply_matrix(temp));

            push_counts[push_counts.size()-1]++;

            //temp.show_matrix();
            //matrix_stack[matrix_stack.size()-1].show_matrix();

        }
        else if(display_commands == "rotate")
        {
            double rot_angle;
            point rot_axis;
            in_file >> rot_angle >> rot_axis.pnt_x >> rot_axis.pnt_y >> rot_axis.pnt_z;
            rot_axis.normalize();

            point col1, col2, col3;

            col1 = get_rotation_column("x", rot_axis, rot_angle);
            col2 = get_rotation_column("y", rot_axis, rot_angle);
            col3 = get_rotation_column("z", rot_axis, rot_angle);

            transformation_matrix temp;
            temp.initialize();

            temp.mat[0][0] = col1.pnt_x;
            temp.mat[1][0] = col1.pnt_y;
            temp.mat[2][0] = col1.pnt_z;

            temp.mat[0][1] = col2.pnt_x;
            temp.mat[1][1] = col2.pnt_y;
            temp.mat[2][1] = col2.pnt_z;

            temp.mat[0][2] = col3.pnt_x;
            temp.mat[1][2] = col3.pnt_y;
            temp.mat[2][2] = col3.pnt_z;

            temp.mat[3][3] = 1.00;

//            cout<<endl<<endl;
//            temp.show_matrix();
//            cout<<endl<<endl;

            matrix_stack.push_back(matrix_stack[matrix_stack.size()-1].multiply_matrix(temp));

            push_counts[push_counts.size()-1]++;

        }
        else if(display_commands == "push")
        {
            push_counts.push_back(0);
        }
        else if(display_commands == "pop")
        {
            for(int i = 0; i < push_counts[push_counts.size()-1]; i++)
            {
                matrix_stack.pop_back();
            }
            push_counts.pop_back();
        }
        in_file >> display_commands;
    }

    in_file.close();
    out_file.close();
    {

    /// view transformation

    in_file.open(number+"/stage1.txt");
    out_file.open(number+"/stage2.txt");

    point l, r, u;

    l = subtract_points(look, eye);
    l.normalize();
    r = cross_multiply_points(l, up);
    r.normalize();
    u = cross_multiply_points(r, l);


    transformation_matrix T;
    T.initialize_identity();
    T.mat[0][3] = -eye.pnt_x;
    T.mat[1][3] = -eye.pnt_y;
    T.mat[2][3] = -eye.pnt_z;

    transformation_matrix R;
    R.initialize();

    R.mat[0][0] = r.pnt_x;
    R.mat[1][0] = u.pnt_x;
    R.mat[2][0] = -l.pnt_x;

    R.mat[0][1] = r.pnt_y;
    R.mat[1][1] = u.pnt_y;
    R.mat[2][1] = -l.pnt_y;

    R.mat[0][2] = r.pnt_z;
    R.mat[1][2] = u.pnt_z;
    R.mat[2][2] = -l.pnt_z;

    R.mat[3][3] = 1.00;

    transformation_matrix view = R.multiply_matrix(T);

    for(int i = 0; i < trig_count; i++)
    {
        point vertex1, vertex2, vertex3;
        in_file >> vertex1.pnt_x >> vertex1.pnt_y >> vertex1.pnt_z;
        in_file >> vertex2.pnt_x >> vertex2.pnt_y >> vertex2.pnt_z;
        in_file >> vertex3.pnt_x >> vertex3.pnt_y >> vertex3.pnt_z;

        vertex1 = view.multiply_point(vertex1);
        //cout<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;
        out_file<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;

        vertex2 = view.multiply_point(vertex2);
        //cout<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;
        out_file<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;

        vertex3 = view.multiply_point(vertex3);
        //cout<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;
        out_file<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;

        //cout<<endl;
        out_file<<endl;
    }

    in_file.close();
    out_file.close();

    }


    {
    /// projection transformation

    in_file.open(number+"/stage2.txt");
    out_file.open(number+"/stage3.txt");

    double fovX, t, r1;

    fovX = fovY * aspectRatio;
    t = near * tan(fovY/2*pi/180.0);
    r1 = near * tan(fovX/2*pi/180.0);

    transformation_matrix projection;
    projection.initialize();

    projection.mat[0][0] = near/r1;
    projection.mat[1][1] = near/t;
    projection.mat[2][2] = -(far+near)/(far-near);
    projection.mat[2][3] = -(2.0*far*near)/(far-near);
    projection.mat[3][2] = -1.0;

    for(int i = 0; i < trig_count; i++)
    {
        point vertex1, vertex2, vertex3;
        in_file >> vertex1.pnt_x >> vertex1.pnt_y >> vertex1.pnt_z;
        in_file >> vertex2.pnt_x >> vertex2.pnt_y >> vertex2.pnt_z;
        in_file >> vertex3.pnt_x >> vertex3.pnt_y >> vertex3.pnt_z;

        vertex1 = projection.multiply_point(vertex1);
        //cout<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;
        out_file<<vertex1.pnt_x<<" "<<vertex1.pnt_y<<" "<<vertex1.pnt_z<<endl;

        vertex2 = projection.multiply_point(vertex2);
        //cout<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;
        out_file<<vertex2.pnt_x<<" "<<vertex2.pnt_y<<" "<<vertex2.pnt_z<<endl;

        vertex3 = projection.multiply_point(vertex3);
        //cout<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;
        out_file<<vertex3.pnt_x<<" "<<vertex3.pnt_y<<" "<<vertex3.pnt_z<<endl;

        //cout<<endl;
        out_file<<endl;
    }

    in_file.close();
    out_file.close();

    }

    double scrn_wdth, scrn_hght, lft_lmt_x, rgt_lmt_x, btm_lmt_y, top_lmt_y, frnt_lmt_z, rear_lmt_z;
    double dx, dy, top_y, btm_y, lft_x, rgt_x;

    in_file.open(number+"/config.txt");

    in_file >> scrn_wdth >> scrn_hght;
    in_file >> lft_lmt_x;
    in_file >> btm_lmt_y;
    in_file >> frnt_lmt_z >> rear_lmt_z;

    rgt_lmt_x = -1 * lft_lmt_x;
    top_lmt_y = -1 * btm_lmt_y;


    dx = (rgt_lmt_x - lft_lmt_x)/scrn_wdth;
    dy = (top_lmt_y - btm_lmt_y)/scrn_hght;
    top_y = top_lmt_y - dy/2.0;
    btm_y = btm_lmt_y + dy/2.0;
    lft_x = lft_lmt_x + dx/2.0;
    rgt_x = rgt_lmt_x - dx/2.0;

    in_file.close();

    vector<vector<double>> z_buffer;

    for(int i = 0; i < scrn_hght; i++)
    {
        vector<double> temp;
        for (int j = 0; j < scrn_wdth; j++)
        {
            temp.push_back(rear_lmt_z);
        }
        z_buffer.push_back(temp);
    }

    vector<vector<vector<int>>> frm_buffer;

    for(int i = 0; i < scrn_hght; i++)
    {
        vector<vector<int>> temp;
        for (int j = 0; j < scrn_wdth; j++)
        {
            vector<int> colors;
            for(int k = 0; k < 3; k++)
            {
                colors.push_back(0);
            }
            temp.push_back(colors);
        }
        frm_buffer.push_back(temp);
    }


    in_file.open(number+"/stage3.txt");

    vector<triangle> triangles;

    for(int i = 0; i < trig_count; i++)
    {
        point vertex1, vertex2, vertex3;
        in_file >> vertex1.pnt_x >> vertex1.pnt_y >> vertex1.pnt_z;
        in_file >> vertex2.pnt_x >> vertex2.pnt_y >> vertex2.pnt_z;
        in_file >> vertex3.pnt_x >> vertex3.pnt_y >> vertex3.pnt_z;

        triangle temp_trig;
        temp_trig.vertices.push_back(vertex1);
        temp_trig.vertices.push_back(vertex2);
        temp_trig.vertices.push_back(vertex3);

        int r_val, g_val, b_val;

        r_val = rand()%256;
        g_val = rand()%256;
        b_val = rand()%256;

        temp_trig.color.push_back(r_val);
        temp_trig.color.push_back(g_val);
        temp_trig.color.push_back(b_val);

        triangles.push_back(temp_trig);
    }

    in_file.close();

    for(int cnt = 0; cnt < trig_count; cnt++)
    {
        double max_y, min_y, max_x, min_x;
        int top_scn_ln = 0, btm_scn_ln = scrn_hght;

        max_y = max(triangles[cnt].vertices[0].pnt_y,triangles[cnt].vertices[1].pnt_y);
        max_y = max(max_y, triangles[cnt].vertices[2].pnt_y);

        min_y = min(triangles[cnt].vertices[0].pnt_y,triangles[cnt].vertices[1].pnt_y);
        min_y = min(min_y, triangles[cnt].vertices[2].pnt_y);

        if(max_y < top_y) top_scn_ln = (int) round((top_y - max_y)/dy);
        if(min_y > btm_y) btm_scn_ln = scrn_hght - (int) round((min_y - btm_y)/dy);

        point lft_vert, mid_vert, rgt_vert;

        max_x = max(triangles[cnt].vertices[0].pnt_x,triangles[cnt].vertices[1].pnt_x);
        max_x = max(max_x, triangles[cnt].vertices[2].pnt_x);

        min_x = min(triangles[cnt].vertices[0].pnt_x,triangles[cnt].vertices[1].pnt_x);
        min_x = min(min_x, triangles[cnt].vertices[2].pnt_x);

        for(int i = top_scn_ln; i < btm_scn_ln; i++)
        {
            double ys = top_y - i*dy;

            if(ys > max_y || ys < min_y) continue;

            /// case 1 ys > max_y
            ///-> goto next ys

            /// case 2 ys == max_y and no other vertex
            ///-> only one point, top vertex

            /// case 3 ys == max_y && ys == one other vertex ys > last vertex
            ///-> top two vertex are max take the third vertex y value as the middle point

            /// case 4 ys < max_y && ys >= other two
            ///-> take min_x as left, mid_x as mid, max_x as right or max_y as mid

            /// case 5 ys < min_y
            ///-> end

            /// case 6 ys == min_y and no other vertex
            ///-> only one point, bottom vertex

            /// case 7 ys > min_y && ys == one other vertex and ys < last vertex
            /// bottom two vertex are min take the third vertex y value as the middle point

            /// case 8 ys > min_y && ys <=other two
            /// take min_y as mid and other two as wish

            vector<double> x_vals_on_scn_ln;

            for(int v_num = 0; v_num < 3; v_num++)
            {
                int num_1 = v_num, num_2 = (v_num+1)%3;

                point p1, p2;
                p1 = triangles[cnt].vertices[num_1];
                p2 = triangles[cnt].vertices[num_2];

                if(p1.pnt_y != p2.pnt_y) {
                    double val = p1.pnt_x + ((ys - p1.pnt_y)*(p1.pnt_x - p2.pnt_x)/(p1.pnt_y - p2.pnt_y));
                    if (val<=max_x && val>=min_x) x_vals_on_scn_ln.push_back(val);
                }
            }

            double min_x_on_scanline = *min_element(x_vals_on_scn_ln.begin(), x_vals_on_scn_ln.end());
            double max_x_on_scanline = *max_element(x_vals_on_scn_ln.begin(), x_vals_on_scn_ln.end());
            int lft_int_col, rgt_int_col;

            if(min_x_on_scanline <= lft_x) {
                lft_int_col = 0;
            } else {
                lft_int_col = (int) round((min_x_on_scanline - lft_x)/dx);
            }
            if(max_x_on_scanline >= rgt_x) {
                rgt_int_col = scrn_wdth;
            } else {
                rgt_int_col = scrn_wdth - ((int) round((rgt_x - max_x_on_scanline)/dx));
            }

            /// find out two y values on the line where min_x_on_scanline is placed
            vector<double> min_x_line_y_z_values = find_ys_zs_for_x(ys, min_x_on_scanline, triangles[cnt].vertices);

            /// find out two y values on the line where max_x_on_scanline is placed
            vector<double> max_x_line_y_z_values = find_ys_zs_for_x(ys, max_x_on_scanline, triangles[cnt].vertices);

            double z1_min, z2_min, y1_min, y2_min, z1_max, z2_max, y1_max, y2_max, zb, za;

            y1_min = min_x_line_y_z_values[0];
            y2_min = min_x_line_y_z_values[1];
            z1_min = min_x_line_y_z_values[2];
            z2_min = min_x_line_y_z_values[3];

            y1_max = max_x_line_y_z_values[0];
            y2_max = max_x_line_y_z_values[1];
            z1_max = max_x_line_y_z_values[2];
            z2_max = max_x_line_y_z_values[3];

            za = z1_min + (ys - y1_min) * (z2_min - z1_min) / (y2_min - y1_min);
            zb = z1_max + (ys - y1_max) * (z2_max - z1_max) / (y2_max - y1_max);


            ///cout<<x_vals_on_scn_ln.size()<<endl;

            double zp = za + ((lft_x + lft_int_col *dx) - min_x_on_scanline)*(zb - za)/(max_x_on_scanline- min_x_on_scanline);
            double const_term = dx*(zb - za)/(max_x_on_scanline - min_x_on_scanline);

            for(int j = lft_int_col; j < rgt_int_col; j++)
            {
                zp = zp + const_term;

                if(zp > frnt_lmt_z && zp < z_buffer[i][j])
                {
                    z_buffer[i][j] = zp;

                    frm_buffer[i][j][0] = triangles[cnt].color[0];
                    frm_buffer[i][j][1] = triangles[cnt].color[1];
                    frm_buffer[i][j][2] = triangles[cnt].color[2];
                }
            }

        }

    }

    /// z_buffer txt


    out_file.open(number+"/z_buffer.txt");

    for(int i=0; i< scrn_hght; i++)
    {
        for(int j=0; j< scrn_wdth; j++)
        {
            if(z_buffer[i][j] < rear_lmt_z)
            {
                out_file << z_buffer[i][j] << '\t';
            }
        }
        out_file << endl;
    }
    out_file.close();


    ///generate image

    bitmap_image img(scrn_wdth, scrn_hght);

    for(int i=0; i<scrn_hght; i++)
    {
        for(int j=0; j<scrn_wdth; j++)
        {
            img.set_pixel(j, i, frm_buffer[i][j][0], frm_buffer[i][j][1], frm_buffer[i][j][2]);
        }
    }
    img.save_image(number+"/out.bmp");

    z_buffer.clear();
    frm_buffer.clear();


}











