#include "hittable.h"

hittable_sphere::hittable_sphere(
    const vec3& position,
    const double radius,
    std::shared_ptr<material> mat
) : body(position, radius), mat(mat)
{
}

bool hittable_sphere::hit(ray& r, double t_min, double t_max, double& t)
{
	return body.hit(r, t_min, t_max, t);
}

bool hittable_sphere::scatter(ray& r_in, double t, ray& r_out, colour& attenuation)
{
	vec3 p = r_in.origin + t * r_in.direction;
	vec3 normal = (p - body.position) / body.radius;

	if (dot(r_in.direction, normal) > 0) {
		normal = -normal;
	}

	mat->scatter(r_in, p, normal, r_out);
	attenuation = mat->albedo;

	return true;
}

hittable_light::hittable_light(
	const vec3& position,
	const double radius,
	const colour& emission
) : body(position, radius), emission(emission)
{
}

bool hittable_light::hit(ray& r, double t_min, double t_max, double& t)
{
	return body.hit(r, t_min, t_max, t);
}

bool hittable_light::scatter(ray& r_in, double t, ray& r_out, colour& attenuation)
{
	attenuation = emission;

	return false;
}
