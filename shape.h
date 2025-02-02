#pragma once

#include <memory>

#include "vec3.h"
#include "material.h"

class material;

class shape
{
public:
	virtual bool hit(ray& r, double t_min, double t_max, double& t) = 0;
	virtual void scatter(ray& r_in, double t, vec3& p, vec3& n) = 0;
};

class sphere : public shape
{
public:
	sphere();
	sphere(const vec3& position, const double radius);

	bool hit(ray& r, double t_min, double t_max, double& t) override;
	void scatter(ray& r_in, double t, vec3& p, vec3& n) override;

	vec3 position;
	double radius;
};

class quad : public shape
{
public:
	quad();
	quad(const vec3& Q, const vec3& u, const vec3& v);

	bool hit(ray& r, double t_min, double t_max, double& t) override;
	void scatter(ray& r_in, double t, vec3& p, vec3& n) override;

private:
	vec3 Q;
	vec3 u;
	vec3 v;

	std::vector<vec3> vertices = std::vector<vec3>(4);

	vec3 normal;
};

class triangle : public shape
{
public:
	triangle(const vec3& Q, const vec3& u, const vec3& v);

	bool hit(ray& r, double t_min, double t_max, double& t) override;
	void scatter(ray& r_in, double t, vec3& p, vec3& n) override;

private:
	vec3 Q;
	vec3 u;
	vec3 v;

	std::vector<vec3> vertices = std::vector<vec3>(3);

	vec3 normal;
};

class cube : public shape
{
public:
	cube(const vec3& Q, const double length);
	cube(
		const vec3& Q,
		const vec3& u,
		const vec3& v,
		const vec3& w,
		const double length
	);

	bool hit(ray& r, double t_min, double t_max, double& t) override;
	void scatter(ray& r_in, double t, vec3& p, vec3& n) override;

private:
	vec3 Q;
	double length;
	
	std::vector<quad> faces = std::vector<quad>(6);
	// Used to pass the information from hit function to scatter function
	//which face was hit by the ray of light
	int face_hit;
};

bool interior(const std::vector<vec3>& vertices, const vec3& p);
bool same_side(const vec3& a, const vec3& b, const vec3& c, const vec3& p);
