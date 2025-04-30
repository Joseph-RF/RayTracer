#pragma once

#include "vec3.h"

class Ray
{
public:
	Ray(Vec3 origin, Vec3 direction);

	Vec3 origin;
	Vec3 direction;

	float interior;
};
