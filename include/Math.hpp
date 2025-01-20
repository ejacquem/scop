#ifndef MATH_HPP
#define MATH_HPP

# define M_PI		3.14159265358979323846	/* pi */

#include <cmath>
#include <iostream>

struct vec2
{
    float x, y;

    //constructors
    vec2(float x, float y){
        this->x = x;
        this->y = y;
    }
    vec2(float a){
        x = y = a;
    }
    vec2(){
        *this = vec2(0);
    }
    vec2(const vec2& o) = default;

    float operator[](int index) const{
        return (&x)[index];
    }
    float &operator[](int index){
        return (&x)[index];
    }
    float length() const{
        return sqrt(x * x + y * y);
    }
};

struct vec3
{
    float x, y, z;

    //constructors
    vec3(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    vec3(float a){
        x = y = z = a;
    }
    vec3(){
        *this = vec3(0);
    }
    vec3(const vec3& o) = default;

    //operator overload
    float operator[](int index) const{
        return (&x)[index];
    }
    float &operator[](int index){
        return (&x)[index];
    }
    vec3 operator*(float o){
        return {x * o, y * o, z * o};
    }
    vec3 operator/(float o){
        return {x / o, y / o, z / o};
    }
    vec3 operator+(const vec3& o) const{
        return {x + o.x, y + o.y, z + o.z};
    }
    vec3 operator-(const vec3& o) const{
        return {x - o.x, y - o.y, z - o.z};
    }
    vec3 operator*(const vec3& o) const{
        return {x * o.x, y * o.y, z * o.z};
    }
    vec3 operator+=(const vec3& o){
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    vec3 operator-=(const vec3& o){
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    //functions
    float length() const{
        return sqrt(x * x + y * y + z * z);
    }
    float dot(const vec3& o){
        return x * o.x + y * o.y + z * o.z;
    }
};

struct vec4
{
    float x, y, z, w;

    //constructors
    vec4(float x, float y, float z, float w){
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    vec4(float a){
        x = y = z = w = a;
    }
    vec4(){
        *this = vec4(0);
    }
    vec4(const vec4& o) = default;

    //operator overload
    float operator[](int index) const{
        return (&x)[index];
    }
    float &operator[](int index){
        return (&x)[index];
    }
    vec4 operator+(const vec4& o) const{
        return {x + o.x, y + o.y, z + o.z, w + o.w};
    }
    vec4 operator-(const vec4& o) const{
        return {x - o.x, y - o.y, z - o.z, w - o.w};
    }
    vec4 operator*(const vec4& o) const{
        return {x * o.x, y * o.y, z * o.z, w * o.w};
    }
    vec4 operator/(const vec4& o) const{
        return {x / o.x, y / o.y, z / o.z, w / o.w};
    }
    vec4 operator*(float o){
        return {x * o, y * o, z * o, w * o};
    }
    vec4 operator/(float o){
        return {x / o, y / o, z / o, w / o};
    }

    //functions
    vec4 cross(const vec4& o) const{
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x, 0};
    }
    float dot(const vec4& o){
        return x * o.x + y * o.y + z * o.z + w * o.w;
    }
    float length() const{
        return sqrt(x * x + y * y + z * z + w * w);
    }
    //operator << for printing
    friend std::ostream& operator<<(std::ostream& os, const vec4& v){
        os << v.x << " " << v.y << " " << v.z << " " << v.w;
        return os;
    }
};

struct mat4
{
    vec4 data[4] = {0};

    //constructors
    mat4 (float f){
        data[0][0] = data[1][1] = data[2][2] = data[3][3] = f;
    }
    mat4() = default;
    mat4(const mat4& o) = default;
    mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3) {
        data[0] = v0;
        data[1] = v1;
        data[2] = v2;
        data[3] = v3;
    }

    //operator overload
    vec4 operator[](int index) const{
        return data[index];
    }
    vec4 &operator[](int index){
        return data[index];
    }
    mat4 operator+(const mat4& o){
        return {data[0] + o[0], data[1] + o[1], data[2] + o[2], data[3] + o[3]};
    }
    mat4 operator-(const mat4& o){
        return {data[0] - o[0], data[1] - o[1], data[2] - o[2], data[3] - o[3]};
    }
    mat4 operator*(const mat4& o){
        mat4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result[i][j] = 
                data[i][0] * o[0][j] + 
                data[i][1] * o[1][j] + 
                data[i][2] * o[2][j] + 
                data[i][3] * o[3][j];
            }
        }
        return result;
    }

    void translate(vec3 v){
        data[3][0] += v.x;
        data[3][1] += v.y;
        data[3][2] += v.z;
    }
    //operator << for printing
    friend std::ostream& operator<<(std::ostream& os, const mat4& m){
        for (int i = 0; i < 4; i++)
        {
            os << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl;
        }
        return os;
    }
};

float radians(float degree);
float degrees(float radian);
vec3 cross(const vec3& a, const vec3& b);
vec3 normalize(const vec3& a);

mat4 lookAt(vec3 eye, vec3 center, vec3 up);
mat4 perspective(float fovy, float aspect, float near, float far);
mat4 rotate_y(mat4 mat,double radians);


#endif