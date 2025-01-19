#include "vec3.h"

vec3::vec3() : x(0.0), y(0.0), z(0.0)
{
}

vec3::vec3(double x, double y, double z) : x(x), y(y), z(z)
{
}

vec3::vec3(const vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

double vec3::get_x() const {return x;}

double vec3::get_y() const {return y;}

double vec3::get_z() const {return z;}

void vec3::set_x(double x) {this->x = x;}

void vec3::set_y(double y) {this->y = y;}

void vec3::set_z(double z) {this->z = z;}

vec3& vec3::operator=(const vec3& v)
{
	if (this == &v) {
		return *this;
	}
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

vec3& vec3::operator+=(const vec3& v)
{
	if (this == &v) {
		return *this;
	}
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

vec3& vec3::operator*=(const double a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

vec3 vec3::operator-() const
{
	return vec3(-this->x, -this->y, -this->z);
}

double vec3::length()
{
	return std::sqrt(x * x + y * y + z * z);
}

double vec3::length_sq()
{
	return (x * x + y * y + z * z);
}

void vec3::clampColour()
{
	// Clamp the RGB values such that they aren't greater than 255
	if (x > 1.0) { x = 1.0; }
	if (x < 0.0) { x = 0.0; }
	if (y > 1.0) { y = 1.0; }
	if (y < 0.0) { y = 0.0; }
	if (z > 1.0) { z = 1.0; }
	if (z < 0.0) { z = 0.0; }
}

vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.get_x() + v.get_x(),
				u.get_y() + v.get_y(),
				u.get_z() + v.get_z());
}

vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.get_x() - v.get_x(),
				u.get_y() - v.get_y(),
				u.get_z() - v.get_z());
}

vec3 operator*(const double a, const vec3& v)
{
	return vec3(a * v.get_x(),
				a * v.get_y(),
				a * v.get_z());
}

vec3 operator*(const vec3& v, const double a)
{
	return a * v;
}

vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.get_x() * v.get_x(),
				u.get_y() * v.get_y(),
				u.get_z() * v.get_z());
}

vec3 operator/(const double a, const vec3& v)
{
	return vec3(a / v.get_x(),
				a / v.get_y(),
				a / v.get_z());
}

vec3 operator/(const vec3& v, const double a)
{
	return v * (1 / a);
}

double dot(const vec3& u, const vec3& v)
{
	return (u.get_x() * v.get_x() + 
			u.get_y() * v.get_y() + 
			u.get_z() * v.get_z());
}

vec3 random_unit_vector()
{
	while (true) {
		vec3 v(random_num(-1, 1), random_num(-1, 1), random_num(-1, 1));
		double v_length_sq = v.length_sq();
		// 1e-160 is the precision for double. Avoiding errors with using 0
		if (1e-160 < v_length_sq && v_length_sq <= 1.0) {
			return v / std::sqrt(v_length_sq);
		}
	}
}

vec3 reflect(const vec3& a, const vec3 b)
{
	double a_dot_b = dot(a, b);

	return a - 2 * a_dot_b * b;
}

double random_num()
{
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	static std::mt19937 generator;

	return dist(generator);
}

double random_num(double a, double b)
{
	std::uniform_real_distribution<double> dist(a, b);
	static std::mt19937 generator;

	return dist(generator);
}
