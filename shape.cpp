#include "shape.h"
#include <iostream>

Sphere::Sphere() : radius(0.0) {
}

Sphere::Sphere(const Vec3& position, const double radius) 
	: position(position), radius(radius) {
	bounding_box();
}

bool Sphere::hit(Ray& r, Interval t_range, double& t) {
	// [Done, add to commit message] Need to adjust for the case where the upper edge of t_range is the closer of t1 / t2
	// So, hit is registered, but because it isn't in the valid range, closest_t never set
	//to either t1 nor t2 resulting in closest_t remaining as infinity, t is set to infinity
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

Triangle::Triangle(const Vec3& Q, const Vec3& u, const Vec3& v)
	: Q(Q), u(u), v(v) {
	vertices[0] = Q;
	vertices[1] = Q + u;
	vertices[2] = Q + v;

	normal = cross(u, v);

	bounding_box();
}

bool Triangle::hit(Ray& r, Interval t_range, double& t) {
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
	AABB bbox1(Q, Q + u + v);
	AABB bbox2(Q + u, Q + v);

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

Cube::Cube(const Vec3& Q, const double length) : Q(Q), length(length) {
	faces[0] = Quad(Q, Vec3(length, 0.0, 0.0), Vec3(0.0, length, 0.0)); // Front face
	faces[1] = Quad(Q, Vec3(length, 0.0, 0.0), Vec3(0.0, 0.0, length)); // Lower face
	faces[2] = Quad(Q, Vec3(0.0, 0.0, length), Vec3(0.0, length, 0.0)); // Left face
	faces[3] = Quad(Q + Vec3(0.0, 0.0, length),							// Back face
					Vec3(length, 0.0, 0.0),
					Vec3(0.0, length, 0.0));
	faces[4] = Quad(Q + Vec3(0.0, length, 0.0),							// Top face
					Vec3(length, 0.0, 0.0),
					Vec3(0.0, 0.0, length));							
	faces[5] = Quad(Q + Vec3(length, 0.0, 0.0),							// Right face
					Vec3(0.0, 0.0, length),
					Vec3(0.0, length, 0.0));

	face_hit = -1;
}

Cube::Cube(const Vec3& Q, const Vec3& u,
		   const Vec3& v, const Vec3& w,
		   const double length) {
	this->Q = Q;
	this->length = length;

	Vec3 u_scaled = u / u.length() * length;
	Vec3 v_scaled = v / v.length() * length;
	Vec3 w_scaled = w / w.length() * length;

	faces[0] = Quad(Q, u_scaled, v_scaled);
	faces[1] = Quad(Q, u_scaled, w_scaled);
	faces[2] = Quad(Q, w_scaled, v_scaled);
	faces[3] = Quad(Q + w_scaled, u_scaled, v_scaled);
	faces[4] = Quad(Q + v_scaled, u_scaled, w_scaled);
	faces[5] = Quad(Q + u_scaled, w_scaled, v_scaled);

	face_hit = -1;
}

bool Cube::hit(Ray& r, Interval t_range, double& t) {
	int n = faces.size();
	int closestFace = -1;
	double closest_t = big;

	for (int i = 0; i < n; ++i) {
		if (faces[i].hit(r, t_range, t)) {
			if (t < closest_t) {
				closest_t = t;
				closestFace = i;
			}
		}
	}

	if (closestFace == -1) {
		return false;
	}

	if (!t_range.contains_exclusive(t)) {
		return false;
	}

	t = closest_t;
	face_hit = closestFace;
	return true;
}

void Cube::scatter(Ray& r_in, double t, Vec3& p, Vec3& n) {
	faces[face_hit].scatter(r_in, t, p, n);
}

AABB Cube::get_bounding_box() {
	return bbox;
}

void Cube::bounding_box() {
	bbox = AABB(faces[0].get_bounding_box(), faces[1].get_bounding_box());
	bbox = AABB(bbox, faces[2].get_bounding_box());
	bbox = AABB(bbox, faces[3].get_bounding_box());
	bbox = AABB(bbox, faces[4].get_bounding_box());
	bbox = AABB(bbox, faces[5].get_bounding_box());
}
