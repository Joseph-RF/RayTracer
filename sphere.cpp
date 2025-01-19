#include "sphere.h"

sphere::sphere() : radius(0.0)
{
}

sphere::sphere(
	vec3 position,
	double radius,
	std::shared_ptr<material> mat
) : position(position), radius(radius), mat(mat)
{
}

void sphere::hit(ray& r_in, double t, ray& r_out, colour& attenuation)
{
	vec3 p = r_in.origin + t * r_in.direction;
	vec3 normal = (p - position) / radius;

	if (dot(r_in.direction, normal) > 0) {
		normal = -normal;
	}

	mat->scatter(r_in, p, normal, r_out);
	attenuation = mat->albedo;
}
