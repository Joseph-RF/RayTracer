#pragma once

#include "interval.h"
#include "vec3.h"
#include "ray.h"

class AABB
{
public:
	AABB();
	AABB(Interval x, Interval y, Interval z);
	AABB(const Vec3& a, const Vec3& b);
	AABB(const AABB& bbox1, const AABB& bbox2);

	Interval get_axis_range(int axis);
	bool hit(const Ray& r, Interval t_range);

	Interval x, y, z;

private:
	void add_min_padding();
};
