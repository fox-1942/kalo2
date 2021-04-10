#include "camera.h"

void init_camera(struct Camera* camera)
{
	camera->position.x = 3000;
	camera->position.y = -5000;
	camera->position.z = 1000;

	camera->pose.x = -15;
	camera->pose.y = 0;
	camera->pose.z = 90;
}


void don_not_head_up_against_the_wall (struct Camera* camera, Move move){

	//Skybox
	if (camera->position.x<-skybox_size || camera->position.x>skybox_size ||
		camera->position.y<-skybox_size || camera->position.y>skybox_size ||
		camera->position.z<-skybox_size || camera->position.z>skybox_size)
		init_camera (camera);
	//Sun
	if (camera->position.x<sun_size && camera->position.x>-sun_size &&
			camera->position.y<sun_size && camera->position.y>-sun_size &&
			camera->position.z<sun_size && camera->position.z>-sun_size)
			init_camera (camera);
	//Planet1
	if (camera->position.x < move.planet1.x+planet1_size && camera->position.x>move.planet1.x-planet1_size &&
			camera->position.y<move.planet1.y+planet1_size && camera->position.y>move.planet1.y-planet1_size &&
			camera->position.z<move.planet1.z+planet1_size && camera->position.z>move.planet1.z-planet1_size)
			init_camera (camera);
	//Planet2
	if (camera->position.x < move.planet1.x+1000+planet2_size && camera->position.x>move.planet1.x+1000-planet2_size &&
			camera->position.y<move.planet1.y+1000+planet2_size && camera->position.y>move.planet1.y+1000-planet2_size &&
			camera->position.z<move.planet1.z-100+planet2_size && camera->position.z>move.planet1.z-100-planet2_size)
			init_camera (camera);
	//Planet3
	if (camera->position.x < move.planet3.x+planet3_size && camera->position.x>move.planet3.x-planet3_size &&
			camera->position.y<move.planet3.y+planet3_size && camera->position.y>move.planet3.y-planet3_size &&
			camera->position.z<move.planet3.z+planet3_size && camera->position.z>move.planet3.z-planet3_size)
			init_camera (camera);
	//Planet4
	if (camera->position.x < move.planet4.x+planet4_size && camera->position.x>move.planet4.x-planet4_size &&
			camera->position.y<move.planet4.y+planet4_size && camera->position.y>move.planet4.y-planet4_size &&
			camera->position.z<move.planet4.z+planet4_size && camera->position.z>move.planet4.z-planet4_size)
			init_camera (camera);
	//Satellite
	if (camera->position.x < move.satellite.x +satellite_size+1500 && camera->position.x>move.satellite.x-satellite_size &&
			camera->position.y<move.satellite.y+satellite_size && camera->position.y>move.satellite.y-satellite_size &&
			camera->position.z<move.satellite.z+satellite_size && camera->position.z>move.satellite.z-satellite_size)
			init_camera (camera);
}


void set_view_point(const struct Camera* camera)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-(camera->pose.x + 90), 1.0, 0, 0);
	glRotatef(-(camera->pose.z - 90), 0, 0, 1.0);
	glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(struct Camera* camera, double horizontal, double vertical)
{
	camera->pose.z += horizontal/CAMERA_SPEED;
	camera->pose.x += vertical/CAMERA_SPEED;

	if (camera->pose.z < 0) {
		camera->pose.z += 360.0;
	}

	if (camera->pose.z > 360.0) {
		camera->pose.z -= 360.0;
	}

	if (camera->pose.x < 0) {
		camera->pose.x += 360.0;
	}

	if (camera->pose.x > 360.0) {
		camera->pose.x -= 360.0;
	}

}


void move_camera_up(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	camera->position.z += distance;

}



void move_camera_down(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	camera->position.z -= distance;
}

void move_camera_backward(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	double angle = degree_to_radian(camera->pose.z);

	camera->position.x -= cos(angle) * distance;
	camera->position.y -= sin(angle) * distance;
}

void move_camera_forward(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	double angle = degree_to_radian(camera->pose.z);
	camera->position.x += cos(angle) * distance;
	camera->position.y += sin(angle) * distance;
}

void step_camera_right(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	double angle = degree_to_radian(camera->pose.z + 90.0);


	camera->position.x -= cos(angle) * distance;
	camera->position.y -= sin(angle) * distance;

}

void step_camera_left(struct Camera* camera, double distance)
{
	camera->prev_position = camera->position;
	double angle = degree_to_radian(camera->pose.z - 90.0);

	camera->position.x -= cos(angle) * distance;
	camera->position.y -= sin(angle) * distance;

}
