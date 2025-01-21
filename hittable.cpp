#include "hittable.h"

hittable::hittable(
	const vec3& position,
	const double radius,
	std::shared_ptr<material> mat
) : body(position, radius), mat(mat)
{
}

bool hittable::hit(ray& r, double t_min, double t_max, double& t)
{
	return body.hit(r, t_min, t_max, t);
}

bool hittable::scatter(ray& r_in, double t, ray& r_out, colour& attenuation)
{
	vec3 pos;
	vec3 normal;

	body.scatter(r_in, t, pos, normal);

	attenuation = mat->albedo;
	return mat->scatter(r_in, pos, normal, r_out);
}