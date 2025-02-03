#pragma once

#include "vec3.h"

class Camera
{
public:
	Camera();

	Vec3 position;
	Vec3 lookat;
	Vec3 lookup;

	double d;

	Vec3 w;
	Vec3 v;
	Vec3 u;

	double pixel_samples;
	double pixel_sampling_factor;
};
