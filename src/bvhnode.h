#pragma once

#include "ray.h"
#include "interval.h"
#include "hittable.h"
#include "aabb.h"
#include "vec3.h"

class BVHNode
{
public:
	BVHNode();
	BVHNode(std::vector<std::shared_ptr<Hittable>> hittable_list);
	BVHNode(
		std::vector<std::shared_ptr<Hittable>>& hittable_list,
		int start,
		int end
	);
	~BVHNode();

	std::shared_ptr<Hittable> hit(Ray& r, Interval t_range, double& t);
private:
	bool isLeaf;

	std::shared_ptr<BVHNode> left_node;
	std::shared_ptr<BVHNode> right_node;

	std::shared_ptr<Hittable> left_hittable;
	std::shared_ptr<Hittable> right_hittable;

	AABB bbox;
};

bool axis_minima_comparator(
	std::shared_ptr<Hittable> hittable_1,
	std::shared_ptr<Hittable> hittable_2, 
	int axis
);