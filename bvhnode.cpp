#include "bvhnode.h"

BVHNode::BVHNode() {
	// Consider getting rid of this default constructor.
	// Error occurs when empty hittable_list is passed. Should fix
	// Consider using INDICES instead of pointers to the objects. TEST
	left_node = nullptr;
	right_node = nullptr;
	left_hittable = nullptr;
	right_hittable = nullptr;

	isLeaf = false;
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>> hittable_list)
	: BVHNode(hittable_list, 0, hittable_list.size()) {
	// Initial constructor used for the root of the BVH tree.
	// Creates a copy of the hittable_list passed to it. Then passes references
	//for the rest of the constructors.
	// Not making a copy of hittable_list since we will be rearranging it.
	// Consider testing with and without making the copy to see what changes.
}

BVHNode::BVHNode(
	std::vector<std::shared_ptr<Hittable>>& hittable_list,
	int start,
	int end) {
	left_node = nullptr;
	right_node = nullptr;
	left_hittable = nullptr;
	right_hittable = nullptr;

	int size = end - start;

	if (size < 2) {
		isLeaf = true;
		left_hittable = hittable_list[start];
		right_hittable = hittable_list[start];

		bbox = AABB(left_hittable->get_bounding_box(),
					right_hittable->get_bounding_box());
	}
	else if (size == 2) {
		isLeaf = true;
		left_hittable = hittable_list[start];
		right_hittable = hittable_list[start + 1];

		bbox = AABB(left_hittable->get_bounding_box(),
					right_hittable->get_bounding_box());
	}
	else {
		isLeaf = false;
		// Sort the list using one of the three axes chosen randomly
		// SEE IF THERE IS A BETTER WAY TO DIVIDE
		int axis = random_int(0, 2);

		// This lambda function is horrible to look at.
		auto comparator = [axis](
			std::shared_ptr<Hittable> hittable_1,
			std::shared_ptr<Hittable> hittable_2) {
				return axis_minima_comparator(hittable_1, hittable_2, axis);
			};

		std::sort(std::begin(hittable_list) + start, std::begin(hittable_list) + end, comparator);

		int mid_point = start + size / 2;
		left_node = std::make_shared<BVHNode>(hittable_list, start, mid_point);
		right_node = std::make_shared<BVHNode>(hittable_list, mid_point, end);

		bbox = AABB(left_node->bbox, right_node->bbox);
	}
}

BVHNode::~BVHNode() {
}

std::shared_ptr<Hittable> BVHNode::hit(Ray& r, Interval t_range, double& t) {
	// Improve names "left_hittable_hit
	// See how removing temp_t affects the outcome once code works as intented
	if (!bbox.hit(r, t_range)) {
		return nullptr;
	}

	if (!isLeaf) {
		double temp_t = 0.0;
		std::shared_ptr<Hittable> hittable_hit = nullptr;

		std::shared_ptr<Hittable> left_node_hit = left_node->hit(r, t_range, temp_t);
		if (left_node_hit) {
			hittable_hit = left_node_hit;
			t = temp_t;
		}

		std::shared_ptr<Hittable> right_node_hit = right_node->hit(r, 
			Interval(t_range.min, left_node_hit ? temp_t : t_range.max), temp_t);
		if (right_node_hit) {
			hittable_hit = right_node_hit;
			t = temp_t;
		}

		return hittable_hit;

		// May need to set t in the arguments as a temp_t. t is only set to a
		//value if a hit is confirmed.
		// If right_hit valid i.e. NOT nullptr, return right_hit, set t
		// If left_hit valid, return left_hit, set t
	} else {
		double temp_t = 0.0;
		std::shared_ptr<Hittable> hittable_hit = nullptr;

		bool left_hit = left_hittable->hit(r, t_range, temp_t);
		if (left_hit) {
			hittable_hit = left_hittable;
			t = temp_t;
		}

		bool right_hit = right_hittable->hit(r, 
			Interval(t_range.min, left_hit ? temp_t : t_range.max), temp_t);
		if (right_hit) {
			hittable_hit = right_hittable;
			t = temp_t;
		}
		return hittable_hit;
	}
}

bool axis_minima_comparator(
	std::shared_ptr<Hittable> hittable_1, 
	std::shared_ptr<Hittable> hittable_2,
	int axis) {
	return (hittable_1->get_bounding_box().get_axis_range(axis).min 
		< hittable_2->get_bounding_box().get_axis_range(axis).min);
}
