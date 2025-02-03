#include "shape.h"
#include <iostream>

Sphere::Sphere() : radius(0.0) {
}

Sphere::Sphere(const Vec3& position, const double radius) 
	: position(position), radius(radius) {
}

bool Sphere::hit(Ray& r, double t_min, double t_max, double& t) {
	double closest_t = big;

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

	if (t1 > t_min && t1 < t_max && t1 < closest_t) {
		closest_t = t1;
	}
	if (t2 > t_min && t2 < t_max && t2 < closest_t) {
		closest_t = t2;
	}
	t = closest_t;

	return true;
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
}

bool Quad::hit(Ray& r, double t_min, double t_max, double& t) {
	double denom = dot(normal, r.direction);

	// Get rid of values that are too close to being parallel
	if (absolute(denom) < small) {
		return false;
	}

	t = (dot(normal, Q) - dot(normal, r.origin)) / denom;

	if (t < t_min || t > t_max) {
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

Triangle::Triangle(const Vec3& Q, const Vec3& u, const Vec3& v)
	: Q(Q), u(u), v(v) {
	vertices[0] = Q;
	vertices[1] = Q + u;
	vertices[2] = Q + v;

	normal = cross(u, v);
}

bool Triangle::hit(Ray& r, double t_min, double t_max, double& t) {
	double denom = dot(normal, r.direction);

	// Get rid of values that are too close to being parallel
	if (absolute(denom) < small) {
		return false;
	}

	t = (dot(normal, Q) - dot(normal, r.origin)) / denom;

	if (t < t_min || t > t_max) {
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

bool Cube::hit(Ray& r, double t_min, double t_max, double& t) {
	int n = faces.size();
	int closestFace = -1;
	double closest_t = big;

	for (int i = 0; i < n; ++i) {
		if (faces[i].hit(r, t_min, t_max, t)) {
			if (t < closest_t) {
				closest_t = t;
				closestFace = i;
			}
		}
	}

	if (closestFace == -1) {
		return false;
	}

	if (t < t_min || t > t_max) {
		return false;
	}

	t = closest_t;
	face_hit = closestFace;
	return true;
}

void Cube::scatter(Ray& r_in, double t, Vec3& p, Vec3& n) {
	faces[face_hit].scatter(r_in, t, p, n);
}
