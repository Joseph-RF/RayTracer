#include "camera.h"

Camera::Camera()
{
	//position = Vec3(-2.0, 2.0, -4.0);
	//lookat = Vec3(-0.5, -0.25, 1.0);
	position = Vec3(1.0, 1.0, -2.0);
	lookat = Vec3(1.0, 1.0, 1.0);

	lookup = Vec3(0.0, 1.0, 0.0);

	d = 1.0;

	w = (lookat - position).normalise();
	u = cross(lookup, w).normalise();
	v = cross(w, u);

	pixel_samples = 10;
	pixel_sampling_factor = 1.0 / pixel_samples;
}
