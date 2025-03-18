#include "vec3.h"

Vec3::Vec3() : x(0.0), y(0.0), z(0.0) {}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

Vec3::Vec3(const Vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

double Vec3::get_x() const {return x;}

double Vec3::get_y() const {return y;}

double Vec3::get_z() const {return z;}

double Vec3::get_axis(int index) const
{
	if (index == 0) { return x; }
	if (index == 1) { return y; }
	return z;
}

void Vec3::set_x(double x) {this->x = x;}

void Vec3::set_y(double y) {this->y = y;}

void Vec3::set_z(double z) {this->z = z;}

Vec3& Vec3::operator=(const Vec3& v) {
	if (this == &v) {
		return *this;
	}
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& v) {
	if (this == &v) {
		return *this;
	}
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator*=(const double a) {
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

Vec3& Vec3::operator*=(const Vec3& v) {
	if (this == &v) {
		return *this;
	}
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vec3 Vec3::operator-() const {
	return Vec3(-this->x, -this->y, -this->z);
}

double Vec3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

double Vec3::length_sq() {
	return (x * x + y * y + z * z);
}

Vec3 Vec3::normalise() {
	return Vec3(x, y, z) / this->length();
}

Vec3 Vec3::y_rotation(const double angle_degrees) {
	double angle_radians = angle_degrees * pi / 180.0;
	double sin_theta = std::sin(angle_radians);
	double cos_theta = std::cos(angle_radians);

	return Vec3(
		cos_theta * x + sin_theta * z,
		y,
		-sin_theta * x + cos_theta * z);
}

void Vec3::clampColour() {
	// Clamp the RGB values such that they aren't greater than 255
	if (x > 1.0) { x = 1.0; }
	if (x < 0.0) { x = 0.0; }
	if (y > 1.0) { y = 1.0; }
	if (y < 0.0) { y = 0.0; }
	if (z > 1.0) { z = 1.0; }
	if (z < 0.0) { z = 0.0; }
}

Vec3 operator+(const Vec3& u, const Vec3& v) {
	return Vec3(u.get_x() + v.get_x(),
				u.get_y() + v.get_y(),
				u.get_z() + v.get_z());
}

Vec3 operator-(const Vec3& u, const Vec3& v) {
	return Vec3(u.get_x() - v.get_x(),
				u.get_y() - v.get_y(),
				u.get_z() - v.get_z());
}

Vec3 operator*(const double a, const Vec3& v) {
	return Vec3(a * v.get_x(),
				a * v.get_y(),
				a * v.get_z());
}

Vec3 operator*(const Vec3& v, const double a) {
	return a * v;
}

Vec3 operator*(const Vec3& u, const Vec3& v) {
	return Vec3(u.get_x() * v.get_x(),
				u.get_y() * v.get_y(),
				u.get_z() * v.get_z());
}

Vec3 operator/(const double a, const Vec3& v) {
	return Vec3(a / v.get_x(),
				a / v.get_y(),
				a / v.get_z());
}

Vec3 operator/(const Vec3& v, const double a) {
	return v * (1 / a);
}

double dot(const Vec3& u, const Vec3& v) {
	return (u.get_x() * v.get_x() + 
			u.get_y() * v.get_y() + 
			u.get_z() * v.get_z());
}

Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.get_y() * v.get_z() - u.get_z() * v.get_y(),
				u.get_z() * v.get_x() - u.get_x() * v.get_z(),
				u.get_x() * v.get_y() - u.get_y() * v.get_x());
}

Vec3 random_unit_vector() {
	while (true) {
		Vec3 v(random_num(-1, 1), random_num(-1, 1), random_num(-1, 1));
		double v_length_sq = v.length_sq();
		// 1e-160 is the precision for double. Avoiding errors with using 0
		if (1e-160 < v_length_sq && v_length_sq <= 1.0) {
			return v / std::sqrt(v_length_sq);
		}
	}
}

Vec3 reflect(const Vec3& a, const Vec3 b) {
	double a_dot_b = dot(a, b);

	return a - 2 * a_dot_b * b;
}

double random_num() {
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	static std::mt19937 generator;

	return dist(generator);
}

double random_num(double a, double b) {
	std::uniform_real_distribution<double> dist(a, b);
	static std::mt19937 generator;

	return dist(generator);
}

double absolute(const double& a) {
	if (a < 0.0) {
		return -a;
	}
	return a;
}

double gammaCorrection(double linear) {
	if (linear > 0.0) {
		return std::sqrt(linear);
	}
	return 0.0;
}
