#include "aabb.h"

AABB::AABB() {}

AABB::AABB(Interval x, Interval y, Interval z) : x(x), y(y), z(z) {}

AABB::AABB(const Vec3& a, const Vec3& b) {
	if (a.get_x() < b.get_x()) {
		x.min = a.get_x();
		x.max = b.get_x();
	} else {
		x.min = b.get_x();
		x.max = a.get_x();
	}

	if (a.get_y() < b.get_y()) {
		y.min = a.get_y();
		y.max = b.get_y();
	} else {
		y.min = b.get_y();
		y.max = a.get_y();
	}

	if (a.get_z() < b.get_z()) {
		z.min = a.get_z();
		z.max = b.get_z();
	} else {
		z.min = b.get_z();
		z.max = a.get_z();
	}

	// need this for thin objects such as quads
	add_min_padding();
}

AABB::AABB(const AABB& bbox1, const AABB& bbox2)
{
	x = Interval(bbox1.x, bbox2.x);
	y = Interval(bbox1.y, bbox2.y);
	z = Interval(bbox1.z, bbox2.z);
}

Interval AABB::get_axis_range(int axis)
{
	if (axis == 0) { return x; }
	if (axis == 1) { return y; }
	return z;
}

bool AABB::hit(const Ray& r, Interval t_range)
{
	for (int i = 0; i < 3; ++i) {
		Interval axis_range = get_axis_range(i);
		double t0 = (axis_range.min - r.origin.get_axis(i)) 
					 / r.direction.get_axis(i);
		double t1 = (axis_range.max - r.origin.get_axis(i))
					 / r.direction.get_axis(i);

		if (t0 < t1) {
			if (t0 > t_range.min) { t_range.min = t0; }
			if (t1 < t_range.max) { t_range.max = t1; }
		}
		else {
			if (t1 > t_range.min) { t_range.min = t1; }
			if (t0 < t_range.max) { t_range.max = t0; }
		}

		if (t_range.max <= t_range.min) {
			return false;
		}
	}
	return true;
}

void AABB::add_min_padding()
{
	double small_amount = 0.001;
	if (x.size() < small_amount) { x.expand(small_amount); }
	if (y.size() < small_amount) { y.expand(small_amount); }
	if (z.size() < small_amount) { z.expand(small_amount); }
}
