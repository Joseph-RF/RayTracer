#pragma once

#include <cmath>

#include <limits>
#include <random>

class Vec3
{
public:
	Vec3();
	Vec3(double x, double y, double z);
	Vec3(const Vec3& v);

	double get_x() const;
	double get_y() const;
	double get_z() const;

	double get_axis(int index) const;

	void set_x(double x);
	void set_y(double y);
	void set_z(double z);

	// Overloading operators
	Vec3& operator=(const Vec3& v);
	Vec3& operator+=(const Vec3& v);
	Vec3& operator*=(const double a);
	Vec3& operator*=(const Vec3& v);

	Vec3 operator-() const;

	double length() const; 
	double length_sq();
	Vec3 normalise();

	Vec3 y_rotation(const double angle_degrees) const;

	void clampColour();

private:
	double x, y, z;
};

// Arithmetic operator overloading
Vec3 operator+(const Vec3& u, const Vec3& v);
Vec3 operator-(const Vec3& u, const Vec3& v);
Vec3 operator*(const double a, const Vec3& v);
Vec3 operator*(const Vec3& v, const double a);
Vec3 operator*(const Vec3& u, const Vec3& v);
Vec3 operator/(const double a, const Vec3& v);
Vec3 operator/(const Vec3& v, const double a);
Vec3 operator/(const Vec3& u, const Vec3& v);

double dot(const Vec3& u, const Vec3& v);
Vec3 cross(const Vec3& u, const Vec3& v);

Vec3 random_unit_vector();

Vec3 reflect(const Vec3& a, const Vec3& b);
Vec3 refract(const Vec3& d, const Vec3& n, const double rindexin_over_rindexout);

const double pi = 3.141592;

// Code below is not strictly Vec3. Consider moving to another file
static std::random_device rd;

int random_int(int a, int b);
double random_num();
double random_num(double a, double b);

double absolute(const double& a);

double gammaCorrection(double linear);

const double big = std::numeric_limits<double>::infinity();
const double small = 0.00001;

using Colour = Vec3;