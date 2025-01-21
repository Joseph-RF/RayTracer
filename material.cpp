#include "material.h"

diffuse::diffuse(colour albedo)
{
	this->albedo = albedo;
}

bool diffuse::scatter(
	ray& r_in,
	vec3& p,
	vec3& n,
	ray& r_out
)
{
	r_out.direction = n + random_unit_vector();
	r_out.origin = p;

	return true;
}

metal::metal(colour albedo)
{
	this->albedo = albedo;
}

bool metal::scatter(ray& r_in, vec3& p, vec3& n, ray& r_out)
{
	r_out.direction = reflect(r_in.direction, n);
	r_out.origin = p;

	return true;
}

dielectric::dielectric(colour albedo, double rindex) : rindex(rindex)
{
	this->albedo = albedo;
}

bool dielectric::scatter(ray& r_in, vec3& p, vec3& n, ray& r_out)
{
	return true;
}

light::light(colour emission)
{
	this->albedo = emission;
}

bool light::scatter(ray& r_in, vec3& p, vec3& n, ray& r_out)
{
	return false;
}
