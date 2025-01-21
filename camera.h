#pragma once

#include "vec3.h"

class camera
{
public:
	camera();

	vec3 position;
	vec3 lookat;
	vec3 lookup;

	double d;

	vec3 w;
	vec3 v;
	vec3 u;

	double pixel_samples;
	double pixel_sampling_factor;
};
