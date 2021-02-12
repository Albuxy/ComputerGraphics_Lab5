#include "application.h"
#include "utils.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "light.h"

const int num_meshs = 2;

Camera* camera = NULL;
Mesh* mesh = NULL;
Matrix44 model_matrix;
Shader* shader = NULL;
Texture* texture = NULL;

const int num_luces = 2;
const int num_materials = 5;

//-------------
Shader* shader2 = NULL;
Shader* phong_shader = NULL;
Texture* texture_normal = NULL;

Material* materials[num_materials] = { NULL };
Light* lights[num_luces] = { NULL };
Vector3 ambient_light(60.0, 60.0, 60.0); //here we can store the global ambient light of the scene
int llum = 0;

//-------------

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,20,20),Vector3(0,10,0),Vector3(0,1,0));
	camera->setPerspective(60,window_width / window_height,0.1,10000);

	//-----Mesh
	mesh = new Mesh();
	mesh->loadOBJ("lee.obj");


	//-----Textures
	texture = new Texture();
	texture_normal = new Texture();

	if(!texture->load("lee_color_specular.tga"))
	{
		std::cout << "Texture not found" << std::endl;
		exit(1);
	}

	texture_normal->load("lee_normal.tga");

	//-----Shaders
	shader = Shader::Get("texture.vs","texture.ps");
	shader2 = Shader::Get("texture.vs", "texture.ps");//PRUEBA
	phong_shader= Shader::Get("phong.vs", "phong.ps");

	//-----Materials
	Material* material1 = new Material();
	material1->ambient.set(1,1,1);
	material1->diffuse.set(1,1,1);
	material1->specular.set(0.5f, 0.5f, 0.5f);
	
	Material* material2 = new Material();
	material2->ambient.set(0.5f, 0.5f, 0.5f);
	material2->diffuse.set(0.8f, 0.2f, 0.3f);
	material2->specular.set(1, 1, 1);

	Material* material3 = new Material();
	material3->ambient.set(0.25f, 0.25f, 0.25f);
	material3->diffuse.set(0.3f, 0.5f, 0.7f);
	material3->specular.set(1, 1, 1);

	Material* material4 = new Material();
	material4->ambient.set(0.25f, 0.25f, 0.25f);
	material4->diffuse.set(0.3f, 0.8f, 0.3f);
	material4->specular.set(1, 1, 1);

	Material* material5 = new Material();
	material5->ambient.set(0.8f, 0.8f, 0.8f);
	material5->diffuse.set(0.9f, 0.9f, 0.5f);
	material5->specular.set(1, 1, 1);

	materials[0] = material1;
	materials[1] = material2;
	materials[2] = material3;
	materials[3] = material4;
	materials[4] = material5;

	//------Lights
	Light* light1 = new Light();
	light1->position.set(20, 20, 10);
	light1->diffuse_color.set(1, 1, 1);
	light1->specular_color.set(0.2f, 0.2f, 0.6f);

	Light* light2 = new Light();
	light2->position.set(20, 20, 10);
	light2->diffuse_color.set(0.3f, 0.3f, 0.6f);
	light2->specular_color.set(0.2f, 0.2f, 0.6f);

	lights[0] = light1;
	lights[1] = light2;
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);

	//Get the viewprojection
	camera->aspect = window_width / window_height;
	Matrix44 viewprojection = camera->getViewProjectionMatrix();

	//Determine shader
	if (llum==1){
		shader = phong_shader;
	}

	if (llum == 0) {
		shader = shader2;
	}

	model_matrix.setIdentity();

	//Mesh for each material
	for (int j = 0; j < num_materials; j++) {

		glDisable(GL_BLEND);

		for (int i = 0; i < num_luces; i++) {

			//enable the shader
			shader->enable();
			shader->setMatrix44("model", model_matrix); //upload info to the shader
			shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

			shader->setTexture("color_texture", texture, 0); //set texture in slot 

			if (llum == 1) {
				shader->setTexture("normal_texture", texture_normal, 1);
				
				//Select material
				shader->setVector3("Ka", materials[j]->ambient);
				shader->setVector3("Kd", materials[j]->diffuse);
				shader->setVector3("Ks", materials[j]->specular);
				shader->setFloat("alpha", materials[j]->shininess);

				shader->setVector3("Ia", ambient_light);


				shader->setVector3("Id", lights[i]->diffuse_color);
				shader->setVector3("Is", lights[i]->specular_color);
				shader->setVector3("lp", lights[i]->position);

			}

			shader->setVector3("eye", camera->eye);
			shader->setFloat("blend", 1.0);

			//------------------------
			//render the data
			mesh->render(GL_TRIANGLES);

			//disable shader
			shader->disable();

			//Only apply the lights in shader2
			if (llum == 1) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
			}

		}

		model_matrix.translate((j+1)*5, 0, (j+1)*-10);
	}
	
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE])
	{
		model_matrix.rotateLocal(seconds_elapsed,Vector3(0,1,0));
	}

	if (keystate[SDL_SCANCODE_P]) {//phong activar
		llum = 1;
	}
	
	if (keystate[SDL_SCANCODE_G]) {//phong activar
		llum = 0;
	}

	//Move the light
	if (keystate[SDL_SCANCODE_UP]) {
		lights[0]->position = lights[0]->position + Vector3(0, 1, 0) * seconds_elapsed * 40.0;
		lights[1]->position = lights[1]->position + Vector3(0, 1, 0) * seconds_elapsed * 40.0;
	}
	if (keystate[SDL_SCANCODE_DOWN]) {
		lights[0]->position = lights[0]->position + Vector3(0, -1, 0) * seconds_elapsed * 40.0;
		lights[1]->position = lights[1]->position + Vector3(0, -1, 0) * seconds_elapsed * 40.0;

	}
	if (keystate[SDL_SCANCODE_LEFT]) {
		lights[0]->position = lights[0]->position + Vector3(-1, 0, 0) * seconds_elapsed * 40.0;
		lights[1]->position = lights[1]->position + Vector3(-1, 0, 0) * seconds_elapsed * 40.0;
	}
	if (keystate[SDL_SCANCODE_RIGHT]) {
		lights[0]->position = lights[0]->position + Vector3(1, 0, 0) * seconds_elapsed * 40.0;
		lights[1]->position = lights[1]->position + Vector3(1, 0, 0) * seconds_elapsed * 40.0;

	}

	//Move the eye
	if (keystate[SDL_SCANCODE_A]) {
		camera->eye = camera->eye + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
	}
	if (keystate[SDL_SCANCODE_D]) {
		camera->eye = camera->eye + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
	}
	if (keystate[SDL_SCANCODE_W]) {
		camera->eye = camera->eye + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
	}
	if (keystate[SDL_SCANCODE_S]) {
		camera->eye = camera->eye + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
	}


}

//keyboard press event 
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_r: Shader::ReloadAll(); break;
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{

	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
