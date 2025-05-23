#pragma once

#include "vec3.h"
#include "ray.h"
#include "shape.h"

class Material
{
public:
	virtual bool scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) = 0;

	Colour albedo;
};

class Diffuse : public Material
{
public:
	Diffuse(Colour albedo);

	bool scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out);
};

class Metal : public Material
{
public:
	Metal(Colour albedo);

	bool scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out);
};

class Dielectric : public Material
{
public:
	Dielectric(Colour albedo, double rindex);

	bool scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out);

private:
	double rindex;

	static double reflectance(double cos, double rindex);
};

class Light : public Material
{
public:
	Light(Colour emission);

	bool scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out);
};