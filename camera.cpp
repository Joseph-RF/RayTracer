#include "camera.h"

camera::camera()
{
	//position = vec3(-2.0, 2.0, -4.0);
	//lookat = vec3(-0.5, -0.25, 1.0);
	position = vec3(1.0, 1.0, -2.0);
	lookat = vec3(1.0, 1.0, 1.0);

	lookup = vec3(0.0, 1.0, 0.0);

	d = 1.0;

	w = (lookat - position).normalise();
	u = cross(lookup, w).normalise();
	v = cross(w, u);

	pixel_samples = 10;
	pixel_sampling_factor = 1.0 / pixel_samples;
}
