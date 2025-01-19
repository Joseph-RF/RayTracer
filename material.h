#pragma once

#include "vec3.h"
#include "ray.h"
#include "sphere.h"

class material
{
public:
	virtual void scatter(
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

	void scatter(
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

	void scatter(
		ray& r_in,
		vec3& p,
		vec3& n,
		ray& r_out
	);
};

// at least 4 sets to failure in a week separated across 2 days
// 1 g/kg - 1.5 g/kg
// 8 sets , 1.3g/kg