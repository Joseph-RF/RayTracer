#pragma once

#include "vec3.h"
#include "ray.h"
#include "shape.h"

class material
{
public:
	virtual bool scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	) = 0;

	colour albedo;
};

class diffuse : public material
{
public:
	diffuse(colour albedo);

	bool scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	);
};

class metal : public material
{
public:
	metal(colour albedo);

	bool scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	);
};

class dielectric : public material
{
public:
	dielectric(colour albedo, double rindex);

	bool scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	);

private:
	double rindex;
};

class light : public material
{
public:
	light(colour emission);

	bool scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	);
};

// at least 4 sets to failure in a week separated across 2 days
// 1 g/kg - 1.5 g/kg
// 8 sets , 1.3g/kg