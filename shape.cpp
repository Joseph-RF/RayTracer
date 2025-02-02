#include "shape.h"
#include <iostream>

sphere::sphere() : radius(0.0)
{
}

sphere::sphere(const vec3& position, const double radius) 
	: position(position), radius(radius)
{
}

bool sphere::hit(ray& r, double t_min, double t_max, double& t)
{
	double closest_t = big;

	vec3 OC = r.origin - position;
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

void sphere::scatter(ray& r_in, double t, vec3& p, vec3& n)
{
	vec3 pos = r_in.origin + t * r_in.direction;
	vec3 normal = (pos - position) / radius;

	if (dot(r_in.direction, normal) > 0) {
		normal = -normal;
	}
	p = pos;
	n = normal;
}

quad::quad()
{
	vertices = std::vector<vec3>(4);
}

quad::quad(const vec3& Q, const vec3& u, const vec3& v)
	: Q(Q), u(u), v(v)
{
	vertices[0] = Q;
	vertices[1] = Q + u;
	vertices[2] = Q + u + v;
	vertices[3] = Q + v;

	normal = cross(u, v);
}

bool quad::hit(ray& r, double t_min, double t_max, double& t)
{
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

void quad::scatter(ray& r_in, double t, vec3& p, vec3& n)
{
	p = r_in.origin + t * r_in.direction;
	n = normal;

	if (dot(r_in.direction, n) > 0) {
		n = -n;
	}
}

triangle::triangle(const vec3& Q, const vec3& u, const vec3& v)
	: Q(Q), u(u), v(v)
{
	vertices[0] = Q;
	vertices[1] = Q + u;
	vertices[2] = Q + v;

	normal = cross(u, v);
}

bool triangle::hit(ray& r, double t_min, double t_max, double& t)
{
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

void triangle::scatter(ray& r_in, double t, vec3& p, vec3& n)
{
	p = r_in.origin + t * r_in.direction;
	n = normal;

	if (dot(r_in.direction, n) > 0) {
		n = -n;
	}
}

bool interior(const std::vector<vec3>& vertices, const vec3& p)
{
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

bool same_side(const vec3& a, const vec3& b, const vec3& c, const vec3& p)
{
	// Note, a, b and c need to be in that order of the shape
	vec3 cross0 = cross(b - a, p - a);
	vec3 cross1 = cross(b - a, c - a);

	return dot(cross0, cross1) >= 0;
}

cube::cube(const vec3& Q, const double length) : Q(Q), length(length)
{
	faces[0] = quad(Q, vec3(length, 0.0, 0.0), vec3(0.0, length, 0.0)); // Front face
	faces[1] = quad(Q, vec3(length, 0.0, 0.0), vec3(0.0, 0.0, length)); // Lower face
	faces[2] = quad(Q, vec3(0.0, 0.0, length), vec3(0.0, length, 0.0)); // Left face
	faces[3] = quad(Q + vec3(0.0, 0.0, length),							// Back face
					vec3(length, 0.0, 0.0),
					vec3(0.0, length, 0.0));
	faces[4] = quad(Q + vec3(0.0, length, 0.0),							// Top face
					vec3(length, 0.0, 0.0),
					vec3(0.0, 0.0, length));							
	faces[5] = quad(Q + vec3(length, 0.0, 0.0),							// Right face
					vec3(0.0, 0.0, length),
					vec3(0.0, length, 0.0));

	face_hit = -1;
}

cube::cube(
	const vec3& Q,
	const vec3& u,
	const vec3& v,
	const vec3& w,
	const double length
)
{
	this->Q = Q;
	this->length = length;

	vec3 u_scaled = u / u.length() * length;
	vec3 v_scaled = v / v.length() * length;
	vec3 w_scaled = w / w.length() * length;

	faces[0] = quad(Q, u_scaled, v_scaled);
	faces[1] = quad(Q, u_scaled, w_scaled);
	faces[2] = quad(Q, w_scaled, v_scaled);
	faces[3] = quad(Q + w_scaled, u_scaled, v_scaled);
	faces[4] = quad(Q + v_scaled, u_scaled, w_scaled);
	faces[5] = quad(Q + u_scaled, w_scaled, v_scaled);

	face_hit = -1;
}

bool cube::hit(ray& r, double t_min, double t_max, double& t)
{
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

void cube::scatter(ray& r_in, double t, vec3& p, vec3& n)
{
	faces[face_hit].scatter(r_in, t, p, n);
}
