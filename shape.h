#pragma once

#include <memory>

#include "vec3.h"
#include "material.h"

class Material;

class Shape
{
public:
	virtual bool hit(Ray& r, double t_min, double t_max, double& t) = 0;
	virtual void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) = 0;
};

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const Vec3& position, const double radius);

	bool hit(Ray& r, double t_min, double t_max, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

	Vec3 position;
	double radius;
};

class Quad : public Shape
{
public:
	Quad();
	Quad(const Vec3& Q, const Vec3& u, const Vec3& v);

	bool hit(Ray& r, double t_min, double t_max, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

private:
	Vec3 Q;
	Vec3 u;
	Vec3 v;

	std::vector<Vec3> vertices = std::vector<Vec3>(4);

	Vec3 normal;
};

class Triangle : public Shape
{
public:
	Triangle(const Vec3& Q, const Vec3& u, const Vec3& v);

	bool hit(Ray& r, double t_min, double t_max, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

private:
	Vec3 Q;
	Vec3 u;
	Vec3 v;

	std::vector<Vec3> vertices = std::vector<Vec3>(3);

	Vec3 normal;
};

class Cube : public Shape
{
public:
	Cube(const Vec3& Q, const double length);
	Cube(const Vec3& Q, const Vec3& u,
		 const Vec3& v, const Vec3& w,
		 const double length);

	bool hit(Ray& r, double t_min, double t_max, double& t) override;
	void scatter(Ray& r_in, double t, Vec3& p, Vec3& n) override;

private:
	Vec3 Q;
	double length;
	
	std::vector<Quad> faces = std::vector<Quad>(6);
	// Used to pass the information from hit function to scatter function
	//which face was hit by the Ray of light
	int face_hit;
};

bool interior(const std::vector<Vec3>& vertices, const Vec3& p);
bool same_side(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& p);
