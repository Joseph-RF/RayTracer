#include "hittable.h"

Hittable::Hittable(
	std::shared_ptr<Shape> body,
	std::shared_ptr<Material> mat
) : body(body), mat(mat) {}

bool Hittable::hit(Ray& r, double t_min, double t_max, double& t) {
	return body->hit(r, t_min, t_max, t);
}

bool Hittable::scatter(Ray& r_in, double t, Ray& r_out, Colour& attenuation) {
	Vec3 pos;
	Vec3 normal;

	body->scatter(r_in, t, pos, normal);

	attenuation = mat->albedo;
	return mat->scatter(r_in, pos, normal, r_out);
}