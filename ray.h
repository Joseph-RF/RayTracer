#pragma once

#include "vec3.h"

class ray
{
public:
	ray(vec3 origin, vec3 direction);

	vec3 origin;
	vec3 direction;
};
