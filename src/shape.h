#pragma once

#include <memory>

#include "vec3.h"
#include "material.h"
#include "interval.h"
#include "aabb.h"

class Material;

class Shape
{
public:
	virtual bool hit(Ray& r, Interval t_range, double& t) = 0;
	virtual void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) = 0;

	virtual AABB get_bounding_box() = 0;
};

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const Vec3& position, const double radius);

	bool hit(Ray& r, Interval t_range, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

	AABB get_bounding_box() override;

	void bounding_box();

	Vec3 position;
	double radius;
	AABB bbox;
};

class Quad : public Shape
{
public:
	Quad();
	Quad(const Vec3& Q, const Vec3& u, const Vec3& v);

	bool hit(Ray& r, Interval t_range, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

	AABB get_bounding_box() override;

	void bounding_box();

private:
	Vec3 Q;
	Vec3 u;
	Vec3 v;
	AABB bbox;

	std::vector<Vec3> vertices = std::vector<Vec3>(4);

	Vec3 normal;
};

class Triangle : public Shape
{
public:
	Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3);

	bool hit(Ray& r, Interval t_range, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

	AABB get_bounding_box() override;

	void bounding_box();

private:
	AABB bbox;

	std::vector<Vec3> vertices = std::vector<Vec3>(3);

	Vec3 normal;
};

bool interior(const std::vector<Vec3>& vertices, const Vec3& p);
bool same_side(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p);
