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
	// Use dielectric's transmittance factor T. Where T = 1 - R. R being
	//reflectance. They are the proportion of rays that are transmitted T
	//and reflected R
	// If this ray is transmitted, refract it. Need to check for crit angles
	// If this ray is reflected, reflect it.
	return true;
}

Light::Light(Colour emission) {
	this->albedo = emission;
}

bool Light::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	return false;
}
