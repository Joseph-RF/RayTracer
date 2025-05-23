#include "shape.h"
#include <iostream>

Sphere::Sphere() : radius(0.0) {
}

Sphere::Sphere(const Vec3& position, const double radius) 
	: position(position), radius(radius) {
	bounding_box();
}

bool Sphere::hit(Ray& r, Interval t_range, double& t) {

	double closest_t = big;
	bool ray_hit = false;

	Vec3 OC = r.origin - position;
	double a = dot(r.direction, r.direction);
	double b = 2 * dot(OC, r.direction);
	double c = dot(OC, OC) - (radius * radius);

	double discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0) {
		return false;
	}

	double discriminant_sqrt = std::sqrt(discriminant);

	double t1 = (-b + discriminant_sqrt) / (2 * a);
	double t2 = (-b - discriminant_sqrt) / (2 * a);

	if (t_range.contains_exclusive(t1) && t1 < closest_t) {
		closest_t = t1;
		ray_hit = true;
	}
	if (t_range.contains_exclusive(t2) && t2 < closest_t) {
		closest_t = t2;
		ray_hit = true;
	}
	t = closest_t;

	return ray_hit;
}

void Sphere::scatter(Ray& r_in, double t, Vec3& p, Vec3& n) {
	Vec3 pos = r_in.origin + t * r_in.direction;
	Vec3 normal = (pos - position) / radius;

	if (dot(r_in.direction, normal) > 0) {
		normal = -normal;
	}
	p = pos;
	n = normal;
}

AABB Sphere::get_bounding_box() {
	return bbox;
}

void Sphere::bounding_box() {
	Vec3 radius_vector(radius, radius, radius);
	bbox = AABB(position - radius_vector, position + radius_vector);
}

Quad::Quad() {
	vertices = std::vector<Vec3>(4);
}

Quad::Quad(const Vec3& Q, const Vec3& u, const Vec3& v)
	: Q(Q), u(u), v(v) {
	vertices[0] = Q;
	vertices[1] = Q + u;
	vertices[2] = Q + u + v;
	vertices[3] = Q + v;

	normal = cross(u, v);

	bounding_box();
}

bool Quad::hit(Ray& r, Interval t_range, double& t) {
	double denom = dot(normal, r.direction);

	// Get rid of values that are too close to being parallel
	if (absolute(denom) < small) {
		return false;
	}

	t = (dot(normal, Q) - dot(normal, r.origin)) / denom;

	if (!t_range.contains_exclusive(t)) {
		return false;
	}

	return interior(vertices, r.origin + t * r.direction);
}

void Quad::scatter(Ray& r_in, double t, Vec3& p, Vec3& n) {
	p = r_in.origin + t * r_in.direction;
	n = normal;

	if (dot(r_in.direction, n) > 0) {
		n = -n;
	}
}

AABB Quad::get_bounding_box() {
	return bbox;
}

void Quad::bounding_box() {
	// to create the bounding box for a quad, create a bbox using
	//one axis and create another using the second axis.
	// combine the two bounding boxes to create an overall bounding box

	AABB bbox1(Q, Q + u + v);
	AABB bbox2(Q + u, Q + v);

	bbox = AABB(bbox1, bbox2);
}

Triangle::Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	normal = cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);

	bounding_box();
}

bool Triangle::hit(Ray& r, Interval t_range, double& t) {

	double denom = dot(normal, r.direction);

	// Get rid of values that are too close to being parallel
	if (absolute(denom) < small) {
		return false;
	}

	t = (dot(normal, vertices[0]) - dot(normal, r.origin)) / denom;

	if (!t_range.contains_exclusive(t)) {
		return false;
	}

	return interior(vertices, r.origin + t * r.direction);

	/*
	* Code below is the Möller–Trumbore intersection algorithm implementation
	* which can be found on Wikipedia. Presents roughly a 30% reduction in
	* computation time.
	*/
	
	/*
	Vec3 edge1 = vertices[1] - vertices[0];
	Vec3 edge2 = vertices[2] - vertices[0];
	Vec3 ray_cross_e2 = cross(r.direction, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -small && det < small) {
		return false;
	}

	float inv_det = 1.0 / det;
	Vec3 s = r.origin - vertices[0];
	float u = inv_det * dot(s, ray_cross_e2);

	if ((u < 0 && abs(u) > small) || (u > 1 && abs(u - 1) > small)) {
		return false;
	}

	Vec3 s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(r.direction, s_cross_e1);

	if ((v < 0 && abs(v) > small) || (u + v > 1 && abs(u + v - 1) > small)) {
		return false;
	}

	// At this stage we can compute t to find out where the intersection point is on the line.
	t = inv_det * dot(edge2, s_cross_e1);

	if (t_range.contains_exclusive(t)) {
		// Ray intersection
		return true;
	} else {
		return false;
	}
	*/
}

void Triangle::scatter(Ray& r_in, double t, Vec3& p, Vec3& n) {
	p = r_in.origin + t * r_in.direction;
	n = normal;

	if (dot(r_in.direction, n) > 0) {
		n = -n;
	}
}

AABB Triangle::get_bounding_box() {
	return bbox;
}

void Triangle::bounding_box() {
	// refer to bounding_box method for quad for details on bounding box
	//construction of flat shapes.
	AABB bbox1(vertices[0], vertices[1] + vertices[2] - vertices[0]);
	AABB bbox2(vertices[1], vertices[2]);

	bbox = AABB(bbox1, bbox2);
}

bool interior(const std::vector<Vec3>& vertices, const Vec3& p) {
	// Tells returns whether a point on a plane is inside a 2D shape with
	// vertices vertices.

	int n = vertices.size();

	for (int i = 0; i < n; ++i) {
		if (!same_side(
			vertices[i],
			vertices[(i + 1) % n],
			vertices[(i + 2) % n],
			p)) {
			return false;
		}
	}
	return true;
}

bool same_side(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p) {
	// Note, a, b and c need to be in that order of the shape
	Vec3 cross0 = cross(b - a, p - a);
	Vec3 cross1 = cross(b - a, c - a);

	return dot(cross0, cross1) >= 0;
}
