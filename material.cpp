#include "material.h"

diffuse::diffuse(colour albedo)
{
	this->albedo = albedo;
}

void diffuse::scatter(
	ray& r_in,
	vec3& p,
	vec3& n,
	ray& r_out
)
{
	r_out.direction = n + random_unit_vector();
	r_out.origin = p;
}

metal::metal(colour albedo)
{
	this->albedo = albedo;
}

void metal::scatter(ray& r_in, vec3& p, vec3& n, ray& r_out)
{
	r_out.direction = reflect(r_in.direction, n);
	r_out.origin = p;
}
