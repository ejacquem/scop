// #include "Math.hpp"
#include "../include/Math.hpp"
#include <iostream>
#include <cassert>

float radians(float degree)
{
    return degree * M_PI / 180;
}

float degrees(float radian)
{
    return radian * 180 / M_PI;
}

vec3 cross(const vec3& a, const vec3& b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

vec3 normalize(const vec3& a)
{
    float len = a.length();
    return {a.x / len, a.y / len, a.z / len};
}

/*
rotation matrix:
x1 y1 z1 0​
x2 y2 z2 0​
x3 y3 z3 0​
0  0  0  1​
*/

/*
translation matrix:
1  0  0  x​
0  1  0  y​
0  0  1  z​
0  0  0  1​
*/

//lookAt function, creates a Matrix that rotates the camera and translates it to the correct position
mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
    vec3 X, Y, Z;

    // get Z with camera direction
    Z = normalize(eye - center);
    // get X by crossing up and Z
    X = normalize(cross(up, Z));
    // get Y by crossing Z and X
    Y = normalize(cross(Z, X));

    mat4 mat;

    mat[0] = vec4(X.x, Y.x, Z.x, 0);
    mat[1] = vec4(X.y, Y.y, Z.y, 0);
    mat[2] = vec4(X.z, Y.z, Z.z, 0);
    mat[3] = vec4(-X.dot(eye), -Y.dot(eye), -Z.dot(eye),1);

    return mat;
}

/*
f = cotangent ⁡ fovy / 2

a =  f/aspect
b =  (zFar + zNear) / (zNear - zFar)
c = 2(zFar * zNear) / (zNear - zFar)

perspective matrix
a 0 0 0 
0 f 0 0 
0 0 b c 
0 0 -1 0
*/
mat4 perspective(float fovy, float aspect, float near, float far)
{
    float f = 1 / tanf(fovy / 2);
    float a = f / aspect;
    float b =     (far + near) / (near - far);
    float c = 2 * (far * near) / (near - far);
    mat4 mat;
    mat[0] = vec4(a, 0, 0, 0);
    mat[1] = vec4(0, f, 0, 0);
    mat[2] = vec4(0, 0, b,-1);
    mat[3] = vec4(0, 0, c, 0);
    return mat;
}

/*
rotation_y matrix
 cos 0 sin 0 
   0 1 0   0 
-sin 0 cos 0 
   0 0  -1 0
*/
//create a rotation matrix around the y axis
mat4 rotate_y(mat4 mat, double radians)
{
    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    mat4 rotation = {
        vec4(cos_theta, 0, -sin_theta, 0),
        vec4(0, 1, 0, 0),
        vec4(sin_theta, 0, cos_theta, 0),
        vec4(0, 0, 0, 1)
    };

    return mat * rotation;
}

// int main() {
//     // Test vec4 constructors
//     vec4 v1; // Default constructor
//     assert(v1.x == 0 && v1.y == 0 && v1.z == 0 && v1.w == 0);

//     vec4 v2(1); // Constructor with single float
//     assert(v2.x == 1 && v2.y == 1 && v2.z == 1 && v2.w == 1);

//     vec4 v3(1, 2, 3, 4); // Constructor with four floats
//     assert(v3.x == 1 && v3.y == 2 && v3.z == 3 && v3.w == 4);

//     vec4 v4(v3); // Copy constructor
//     assert(v4.x == 1 && v4.y == 2 && v4.z == 3 && v4.w == 4);

//     // Test vec4 operators
//     vec4 v5 = v3 + v2;
//     assert(v5.x == 2 && v5.y == 3 && v5.z == 4 && v5.w == 5);

//     vec4 v6 = v3 - v2;
//     assert(v6.x == 0 && v6.y == 1 && v6.z == 2 && v6.w == 3);

//     vec4 v7 = v3 * v2;
//     assert(v7.x == 1 && v7.y == 2 && v7.z == 3 && v7.w == 4);

//     vec4 v8 = v3 / v2;
//     assert(v8.x == 1 && v8.y == 2 && v8.z == 3 && v8.w == 4);

//     vec4 v9 = v3 * 2.0f;
//     assert(v9.x == 2 && v9.y == 4 && v9.z == 6 && v9.w == 8);

//     vec4 v10 = v3 / 2.0f;
//     assert(v10.x == 0.5 && v10.y == 1 && v10.z == 1.5 && v10.w == 2);

//     assert(v3.dot(v2) == 10); // Dot product

//     vec4 cross = v3.cross(v2); // Cross product
//     assert(cross.x == -1 && cross.y == 2 && cross.z == -1 && cross.w == 0);

//     // Test vec4 indexing
//     assert(v3[0] == 1 && v3[1] == 2 && v3[2] == 3 && v3[3] == 4);

//     // Test mat4 constructors
//     mat4 m1; // Default constructor
//     for (int i = 0; i < 4; i++) {
//         assert(m1[i][0] == 0 && m1[i][1] == 0 && m1[i][2] == 0 && m1[i][3] == 0);
//     }

//     mat4 m2(1); // Constructor with single float
//     for (int i = 0; i < 4; i++) {
//         assert(m2[i][0] == 1 && m2[i][1] == 1 && m2[i][2] == 1 && m2[i][3] == 1);
//     }

//     mat4 m3(v1, v2, v3, v4); // Constructor with vec4s
//     assert(m3[0].x == 0 && m3[1].x == 1 && m3[2].x == 1 && m3[3].x == 1);

//     // Test mat4 addition and subtraction
//     mat4 m4 = m3 + m2;
//     assert(m4[1][0] == 2 && m4[2][0] == 2);

//     mat4 m5 = m3 - m2;
//     assert(m5[1][0] == 0 && m5[2][0] == 0);

//     // Test mat4 multiplication
//     mat4 m6 = m3 * m2;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             assert(m6[i][j] == m3[i][0] * m2[0][j] + m3[i][1] * m2[1][j] + m3[i][2] * m2[2][j] + m3[i][3] * m2[3][j]);
//         }
//     }

//     std::cout << "All tests passed successfully!" << std::endl;
//     return 0;
// }
