#include "material.h"

Diffuse::Diffuse(Colour albedo) {
	this->albedo = albedo;
}

bool Diffuse::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	n = n / n.length();
	r_out.direction = n + random_unit_vector();
	r_out.origin = p;

	return true;
}

Metal::Metal(Colour albedo) {
	this->albedo = albedo;
}

bool Metal::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	n = n / n.length();
	r_out.direction = reflect(r_in.direction, n);
	r_out.origin = p;

	return true;
}

Dielectric::Dielectric(Colour albedo, double rindex) : rindex(rindex) {
	this->albedo = albedo;
}

bool Dielectric::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	return true;
}

Light::Light(Colour emission) {
	this->albedo = emission;
}

bool Light::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	return false;
}
