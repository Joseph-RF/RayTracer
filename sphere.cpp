#include "sphere.h"

sphere::sphere() : radius(0.0)
{
}

sphere::sphere(const vec3& position, const double radius) 
	: position(position), radius(radius)
{
}

bool sphere::hit(ray& r, double t_min, double t_max, double& t)
{
	double closest_t = big;

	vec3 OC = r.origin - position;
	double a = dot(r.direction, r.direction);
	double b = 2 * dot(OC, r.direction);
	double c = dot(OC, OC) - (radius * radius);

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0) {
		return false;
	}

	double discriminant_sqrt = std::sqrt(discriminant);

	double t1 = (-b + discriminant_sqrt) / (2 * a);
	double t2 = (-b - discriminant_sqrt) / (2 * a);

	if (t1 > t_min && t1 < t_max && t1 < closest_t) {
		closest_t = t1;
	}
	if (t2 > t_min && t2 < t_max && t2 < closest_t) {
		closest_t = t2;
	}
	t = closest_t;
}
