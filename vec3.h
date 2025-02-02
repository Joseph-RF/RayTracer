#pragma once

#include <cmath>

#include <limits>
#include <random>

class vec3
{
public:
	vec3();
	vec3(double x, double y, double z);
	vec3(const vec3& v);

	double get_x() const;
	double get_y() const;
	double get_z() const;

	void set_x(double x);
	void set_y(double y);
	void set_z(double z);

	// Overloading operators
	vec3& operator=(const vec3& v);
	vec3& operator+=(const vec3& v);
	vec3& operator*=(const double a);
	vec3& operator*=(const vec3& v);

	vec3 operator-() const;

	double length() const; 
	double length_sq();
	vec3 normalise();

	vec3 y_rotation(const double angle_degrees);

	void clampColour();

private:
	double x, y, z;
};

// Arithmetic operator overloading
vec3 operator+(const vec3& u, const vec3& v);
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const double a, const vec3& v);
vec3 operator*(const vec3& v, const double a);
vec3 operator*(const vec3& u, const vec3& v);
vec3 operator/(const double a, const vec3& v);
vec3 operator/(const vec3& v, const double a);
vec3 operator/(const vec3& u, const vec3& v);

double dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);

vec3 random_unit_vector();

vec3 reflect(const vec3& a, const vec3 b);

const double pi = 3.141592;

// Not strictly vec3
static std::random_device rd;

double random_num();
double random_num(double a, double b);

double absolute(const double& a);

double gammaCorrection(double linear);

const double big = std::numeric_limits<double>::infinity();
const double small = 0.001;

using colour = vec3;