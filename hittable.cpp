#include "hittable.h"

Hittable::Hittable(
	std::shared_ptr<Shape> body,
	std::shared_ptr<Material> mat
) : body(body), mat(mat) {}

bool Hittable::hit(Ray& r, Interval t_range, double& t) {
	return body->hit(r, t_range, t);
}

bool Hittable::scatter(Ray& r_in, double t, Ray& r_out, Colour& attenuation) {
	Vec3 pos;
	Vec3 normal;

	body->scatter(r_in, t, pos, normal);

	attenuation = mat->albedo;
	return mat->scatter(r_in, pos, normal, r_out);
}