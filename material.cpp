#include "material.h"
#include <iostream>

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
	n = n / n.length();
	
	Vec3 unit_r_in_direction = r_in.direction.normalise();
	double cos_theta = std::fmin(-dot(unit_r_in_direction, n), 1.0);
	double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

	Vec3 direction;
	double rindex_fraction = rindex;
	if (!r_in.interior) {
		rindex_fraction = 1.0 / rindex_fraction;
	}

	if (rindex_fraction * sin_theta > 1.0 || 
		reflectance(cos_theta, rindex) > random_num()) {
		// ReFLECted
		direction = reflect(unit_r_in_direction, n);
	}
	else {
		// ReFRACted
		direction = refract(unit_r_in_direction, n, rindex_fraction);
		if (r_in.interior) { r_out.interior = false; }
		else { r_out.interior = true; }
	}

	r_out.direction = direction;
	r_out.origin = p;

	return true;
}

double Dielectric::reflectance(double cosine, double refraction_index) {
	// Use Schlick's approximation for reflectance.
	auto r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}

Light::Light(Colour emission) {
	this->albedo = emission;
}

bool Light::scatter(Ray& r_in, Vec3& p, Vec3& n, Ray& r_out) {
	return false;
}
