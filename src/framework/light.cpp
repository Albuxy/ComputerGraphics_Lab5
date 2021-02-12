#include "light.h"



Light::Light()
{
	//position.set(50, 50, 0);
	//diffuse_color.set(0.6f,0.6f,0.6f);
	//specular_color.set(0.6f, 0.6f, 0.6f);

	position.set(-30, 0, 20);
	diffuse_color.set(0.6f, 0.6f, 0.6f);
	specular_color.set(0.6f, 0.6f, 0.6f);
	ambient.set(0.1, 0.1, 0.1);
}


