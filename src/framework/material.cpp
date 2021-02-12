#include "material.h"



Material::Material()
{
	//ambient.set(1,1,1); //reflected ambient light
	//diffuse.set(0.9, 0.9, 0.9); //reflected diffuse light
	//specular.set(1, 1, 1); //reflected specular light
	//shininess = 50.0; //glosiness coefficient (plasticity)

	ambient.set(1, 1, 1); //reflected ambient light
	diffuse.set(1, 1, 1); //reflected diffuse light
	specular.set(0.5, 0.5, 0.5); //reflected specular light
	shininess = 20.0; //glosiness coefficient (plasticity)
}


