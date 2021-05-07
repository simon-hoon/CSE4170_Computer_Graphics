
#include "my_geom_objects.h"
#include "my_cam.h"

// fog stuffs , and it is also used for additional effect (toggled with 'r' key)
int flag_fog;

int flag_blind_effect;
float screen_frequency, screen_width;
int screen_max_flag;
int phong_or_gouraud; // 0 is phong, 1 is gouraud;

void display_camera(int camera_index) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[camera_index].x, viewport[camera_index].y, viewport[camera_index].w, viewport[camera_index].h);

	glUseProgram(h_ShaderProgram_simple);
	ModelViewMatrix = glm::scale(ViewMatrix[camera_index], glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	glUseProgram(h_ShaderProgram_PS);
	if (!phong_or_gouraud) {
		set_material_floor();
		glUniform1i(loc_texture_PS, TEXTURE_ID_FLOOR);
		ModelViewMatrix = glm::translate(ViewMatrix[camera_index], glm::vec3(-500.0f, 0.0f, 500.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
		draw_floor();
	}
	else {
		glUseProgram(h_ShaderProgram_GS);
		set_material_floor_GS();
		ModelViewMatrix = glm::translate(ViewMatrix[camera_index], glm::vec3(-500.0f, 0.0f, 500.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1000.0f, 1000.0f, 1000.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
		ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_GS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_GS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_GS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
		draw_floor();
	}
	glUseProgram(h_ShaderProgram_PS);
	
	float tiger_jump = 80.0f * sinf(rotation_angle_tiger * 3) + 60.0f * cosf(rotation_angle_tiger * 2);
	tiger_jump = tiger_jump > 0.0f ? tiger_jump : 0.0f;

 	set_material_tiger();
	glUniform1i(loc_texture_PS, TEXTURE_ID_TIGER);
	ModelViewMatrix = glm::rotate(ViewMatrix[camera_index], -rotation_angle_tiger, glm::vec3(0.0f, 1.0f, 0.0f)); 
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(rotation_angle_tiger* 70.0f*sinf(rotation_angle_tiger), tiger_jump, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 180.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	draw_tiger();

	glUseProgram(h_ShaderProgram_simple);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	draw_axes();

	glUseProgram(h_ShaderProgram_PS);

	set_material_ben();
	glUniform1i(loc_texture_PS, TEXTURE_ID_BEN);
	ModelViewMatrix = glm::rotate(ViewMatrix[camera_index], -90.0f* sinf( TO_RADIAN * rotation_angle_ben) + cosf(rotation_angle_ben * TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(300.0f, 0.0f, 50.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(100.0f, -100.0f, -100.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	draw_ben();

	float wolf_jump = 80.0f * sinf(rotation_angle_wolf * 4) + 50.0f * sinf(rotation_angle_wolf * 2);
	wolf_jump = wolf_jump > 0.0f ? wolf_jump : 0.0f;

	set_material_wolf();
	glUniform1i(loc_texture_PS, TEXTURE_ID_WOLF);
	ModelViewMatrix = glm::rotate(ViewMatrix[camera_index], 90.0f * TO_RADIAN * rotation_angle_wolf, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-400.0f, wolf_jump, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(1000.0f * tanf(rotation_angle_wolf*TO_RADIAN), 0.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(0.0f, 0.0f, 1000.0f * sinf(rotation_angle_wolf * TO_RADIAN)));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(150.0f, 150.0f, 150.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	draw_wolf();

	// dragon
	set_material_dragon();
	glUniform1i(loc_texture_PS, TEXTURE_ID_DRAGON);
	ModelViewMatrix = glm::rotate(ViewMatrix[camera_index], rotation_angle_dragon, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(-500.0f, 130.0f * sinf(rotation_angle_dragon * 2) + 130.0f * cosf(rotation_angle_dragon * 4) + 400.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(20.0f, 20.0f, 20.0f));

	// shading light from dragon in MC
	glm::vec4 position_MC = ModelViewMatrix * glm::vec4(light[3].position[0], light[3].position[1],
		light[3].position[2], light[3].position[3]);
	glUniform4fv(loc_light_PS[3].position, 1, &position_MC[0]);
	glUniform4fv(loc_light_PS[3].ambient_color, 1, light[3].ambient_color);
	glUniform4fv(loc_light_PS[3].diffuse_color, 1, light[3].diffuse_color);
	glUniform4fv(loc_light_PS[3].specular_color, 1, light[3].specular_color);
	glm::vec3 direction_MC = glm::mat3(ModelViewMatrix) * glm::vec3(light[3].spot_direction[0], light[3].spot_direction[1],
		light[3].spot_direction[2]);
	glUniform3fv(loc_light_PS[3].spot_direction, 1, &direction_MC[0]);
	glUniform1f(loc_light_PS[3].spot_cutoff_angle, light[3].spot_cutoff_angle);
	glUniform1f(loc_light_PS[3].spot_exponent, light[3].spot_exponent);

	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	draw_dragon();

	// godzilla
	set_material_godzilla();
	if (special_tex_flag) glUniform1i(loc_texture_PS, TEXTURE_ID_SPECIAL_GODZILLA);
	else glUniform1i(loc_texture_PS, TEXTURE_ID_GODZILLA);
	ModelViewMatrix = glm::translate(ViewMatrix[camera_index], glm::vec3(400.0f, 0.0f, 60.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	draw_godzilla();

	// ironman
	set_material_ironman();
	glUniform1i(loc_texture_PS, TEXTURE_ID_IRONMAN);
	ModelViewMatrix = glm::translate(ViewMatrix[camera_index], glm::vec3(-400.0f, 0.0f, 120.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 90.0f * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(150.0f, 150.0f, 150.0f));
	ModelViewProjectionMatrix = ProjectionMatrix[camera_index] * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[camera_index][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[camera_index][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[camera_index][0]);
	glUniform1i(loc_screen_effect, 1);
	draw_ironman();
	glUniform1i(loc_screen_effect, 0);

	glUseProgram(0);
	glutSwapBuffers();
}

int leftbutton_pressed = 0;
int prevx, prevy;

#define CAM_ROT_SENSITIVITY 0.15f
#define CAM_TSPEED 0.1f

enum axes { X_AXIS, Y_AXIS, Z_AXIS };
int flag_rotation_axis;

glm::mat4 renew_cam_orientation_uaxis(float delx, float dely, float length) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_right, vec3_up, vec3_view, vec3_tmp;

	vec3_view = glm::vec3(-ViewMatrix[0][0].y, -ViewMatrix[0][1].y, -ViewMatrix[0][2].y); // -v vector		
	vec3_up = camera[0].vrp; // u vector
	vec3_right = glm::vec3(ViewMatrix[0][0].z, ViewMatrix[0][1].z, ViewMatrix[0][2].z); // n vector

	vec3_tmp = delx * vec3_right + dely * vec3_up;
	vec3_tmp = glm::cross(vec3_tmp, vec3_view); // the rotation axis

	mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].prp);
	mat4_tmp = glm::rotate(mat4_tmp, 0.5f * CAM_ROT_SENSITIVITY * length * TO_RADIAN, vec3_tmp);
	mat4_tmp = glm::translate(mat4_tmp, -camera[0].prp);

	return mat4_tmp;
}

glm::mat4 renew_cam_orientation_vaxis(float delx, float dely, float length) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_right, vec3_up, vec3_view, vec3_tmp;

	vec3_view = glm::vec3(-ViewMatrix[0][0].z, -ViewMatrix[0][1].z, -ViewMatrix[0][2].z); // -n vector		
	vec3_up = camera[0].vup; // v vector
	vec3_right = glm::vec3(ViewMatrix[0][0].x, ViewMatrix[0][1].x, ViewMatrix[0][2].x); // u vector

	vec3_tmp = delx * vec3_right + dely * vec3_up;
	vec3_tmp = glm::cross(vec3_tmp, vec3_view); // the rotation axis

	mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vrp);
	mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY * length * TO_RADIAN, vec3_tmp);
	mat4_tmp = glm::translate(mat4_tmp, -camera[0].vrp);

	return mat4_tmp;
}

glm::mat4 renew_cam_orientation_naxis(float delx, float dely, float length) {
	glm::mat4 mat4_tmp;
	glm::vec3 vec3_right, vec3_up, vec3_view, vec3_tmp;

	vec3_view = glm::vec3(-ViewMatrix[0][0].x, -ViewMatrix[0][1].x, -ViewMatrix[0][2].x); // -u vector		
	vec3_up = camera[0].prp; // n vector
	vec3_right = glm::vec3(ViewMatrix[0][0].y, ViewMatrix[0][1].y, ViewMatrix[0][2].y); // v vector

	vec3_tmp = delx * vec3_right + dely * vec3_up;
	vec3_tmp = glm::cross(vec3_tmp, vec3_view); // the rotation axis

	mat4_tmp = glm::translate(glm::mat4(1.0f), camera[0].vup);
	mat4_tmp = glm::rotate(mat4_tmp, CAM_ROT_SENSITIVITY * length * TO_RADIAN, vec3_tmp);
	mat4_tmp = glm::translate(mat4_tmp, -camera[0].vup);

	return mat4_tmp;
}

void camera_rotate(int x, int y) {
	glm::mat4 mat4_tmp;
	float delx, dely, length;

	if (leftbutton_pressed) {
		delx = (float)(x - prevx), dely = -(float)(y - prevy);
		prevx = x, prevy = y;
		length = sqrtf(delx * delx + dely * dely);
		if (length == 0.0f) return;

		switch (flag_rotation_axis) {
		case X_AXIS:
			mat4_tmp = renew_cam_orientation_uaxis(delx, dely, length);
			camera[0].vrp = glm::vec3(mat4_tmp * glm::vec4(camera[0].vrp, 1.0f));
			camera[0].prp = glm::vec3(mat4_tmp * glm::vec4(camera[0].prp, 0.0f));
			break;
		case Y_AXIS:
			mat4_tmp = renew_cam_orientation_vaxis(delx, dely, length);
			camera[0].prp = glm::vec3(mat4_tmp * glm::vec4(camera[0].prp, 1.0f));
			camera[0].vup = glm::vec3(mat4_tmp * glm::vec4(camera[0].vup, 0.0f));
			break;
		case Z_AXIS:
			mat4_tmp = renew_cam_orientation_naxis(delx, dely, length);
			camera[0].vrp = glm::vec3(mat4_tmp * glm::vec4(camera[0].vrp, 1.0f));
			camera[0].vup = glm::vec3(mat4_tmp * glm::vec4(camera[0].vup, 0.0f));
			break;
		}

		ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
		ViewMatrix[0] = glm::translate(ViewMatrix[0], -camera[0].pos);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];

		glutPostRedisplay();
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	display_camera(0);
	//display_camera(1);
	//glutSwapBuffers();
}

#define CAM_ZOOM_STEP			0.05f
#define CAM_MAX_ZOOM_IN_FACTOR	0.25f
#define CAM_MAX_ZOOM_OUT_FACTOR	2.50f

// for zoom in and out
void mousewheel(int wheel, int direction, int x, int y) {

	if (direction > 0) { // zoom in
		camera[0].zoom_factor -= CAM_ZOOM_STEP;
		if (camera[0].zoom_factor < CAM_MAX_ZOOM_IN_FACTOR)
			camera[0].zoom_factor = CAM_MAX_ZOOM_IN_FACTOR;
		ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y * TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
	}
	else { // zoom out
		camera[0].zoom_factor += CAM_ZOOM_STEP;
		if (camera[0].zoom_factor > CAM_MAX_ZOOM_OUT_FACTOR)
			camera[0].zoom_factor = CAM_MAX_ZOOM_OUT_FACTOR;
		ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y * TO_RADIAN, camera[0].aspect_ratio, camera[0].near_clip, camera[0].far_clip);
		ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
		glutPostRedisplay();
	}
}

void camera_move(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP: // Y_AXIS
		camera[0].pos += 200.0f * CAM_TSPEED * camera[0].vup;
		break;
	case GLUT_KEY_DOWN:
		camera[0].pos -= 200.0f * CAM_TSPEED * camera[0].vup;
		break;
	case GLUT_KEY_RIGHT: // X_AXIS
		camera[0].pos += 0.1f * CAM_TSPEED * glm::cross(camera[0].vup,camera[0].prp);
		break;
	case GLUT_KEY_LEFT: 
		camera[0].pos -= 0.1f * CAM_TSPEED * glm::cross(camera[0].vup, camera[0].prp);
		break;
	case GLUT_KEY_PAGE_UP: // Z_AXIS
		camera[0].pos += CAM_TSPEED * -camera[0].prp;
		break;
	case GLUT_KEY_PAGE_DOWN:
		camera[0].pos -= CAM_TSPEED * -camera[0].prp;
		break;
	}

	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	ViewMatrix[0] = glm::translate(ViewMatrix[0], -camera[0].pos);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
	glutPostRedisplay();
}

void mousepress(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		prevx = x, prevy = y;
		leftbutton_pressed = 1;
		glutPostRedisplay();
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		leftbutton_pressed = 0;
		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y) {

	glm::vec4 position_EC;
	glm::vec3 direction_EC;

	// for light 
	if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int)(key - '0');

		glUseProgram(h_ShaderProgram_PS);
		light[light_ID].light_on = 1 - light[light_ID].light_on;
		if (light[light_ID].light_on) fprintf(stdout, "Light %d on\n", light_ID);
		else fprintf(stdout, "Light %d off\n", light_ID);

		glUniform1i(loc_light_PS[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(h_ShaderProgram_GS);
		glUniform1i(loc_light_GS[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(0);

		glutPostRedisplay();
		return;
	}

	switch (key) {
#define SCEEN_MAX_FREQUENCY 500.0f
	case 'e':
		if (!screen_max_flag) {
			screen_frequency += 10.0f;
			if (screen_frequency > SCEEN_MAX_FREQUENCY)
				screen_max_flag = 1;
		}
		else {
			if (screen_frequency <= 20.0f) screen_max_flag = 0;
			screen_frequency -= 10.0f;
		}
		glUseProgram(h_ShaderProgram_PS);
		glUniform1f(loc_screen_frequency, screen_frequency);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	
	case 'r': // for special effect
		flag_fog = 1 - flag_fog;
		glUseProgram(h_ShaderProgram_PS);
		glUniform1i(loc_flag_fog, flag_fog);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 't': // for special texture for godzilla
		special_tex_flag = 1 - special_tex_flag;
		break;
	case 'q': // choose Phong or Gouraud shader 
		phong_or_gouraud = 1 - phong_or_gouraud;
		break;
	case 'w':
		flag_blind_effect = 1 - flag_blind_effect;
		if (flag_blind_effect) fprintf(stdout, "blind_effect on\n");
		else fprintf(stdout, "blind_effect off\n");
		glUseProgram(h_ShaderProgram_PS);
		glUniform1i(loc_blind_effect, flag_blind_effect);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 'x':
		flag_rotation_axis = X_AXIS;
		break;
	case 'y':
		flag_rotation_axis = Y_AXIS;
		break;
	case 'z':
		flag_rotation_axis = Z_AXIS;
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}
}

void reshape(int width, int height) {

	camera[0].aspect_ratio = (float)width / height;
	viewport[0].x = viewport[0].y = 0;
	viewport[0].w = width ; viewport[0].h = height;
	ProjectionMatrix[0] = glm::perspective(camera[0].zoom_factor * camera[0].fov_y * TO_RADIAN, camera[0].aspect_ratio,
		camera[0].near_clip, camera[0].far_clip);
	ViewProjectionMatrix[0] = ProjectionMatrix[0] * ViewMatrix[0];
	
	camera[1].aspect_ratio = camera[0].aspect_ratio; // for the time being ...
	viewport[1].x = (int)(0.75f * width); viewport[1].y = (int)(0.75f * height);
	viewport[1].w = (int)(0.25f * width); viewport[1].h = (int)(0.25 * height);
	ProjectionMatrix[1] = glm::perspective(camera[1].fov_y * TO_RADIAN, camera[1].aspect_ratio, camera[1].near_clip, camera[1].far_clip);
	ViewProjectionMatrix[1] = ProjectionMatrix[1] * ViewMatrix[1];
	
	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO); 
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteTextures(N_TEXTURES_USED, texture_names);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(camera_move);
	glutMouseFunc(mousepress);
	glutMouseWheelFunc(mousewheel);
	glutMotionFunc(camera_rotate);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutTimerFunc(100, timer_dragon, 0);
	glutTimerFunc(100, timer_wolf, 0);
	glutTimerFunc(100, timer_ben, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_PS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Phong.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_GS[3] = {
	{ GL_VERTEX_SHADER, "Shaders/Gouraud.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Gouraud.frag" },
	{ GL_NONE, NULL }
	};

	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	h_ShaderProgram_PS = LoadShaders(shader_info_PS);
	loc_ModelViewProjectionMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrixInvTrans");

	h_ShaderProgram_GS = LoadShaders(shader_info_GS);
	loc_ModelViewProjectionMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_ModelViewMatrixInvTrans");

	// setting for Phong shader
	loc_global_ambient_color_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_global_ambient_color");
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light_PS[i].light_on = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light_PS[i].position = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light_PS[i].ambient_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light_PS[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light_PS[i].specular_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light_PS[i].spot_direction = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light_PS[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light_PS[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light_PS[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].blind_effect", i);
		loc_light_PS[i].blind_effect = glGetUniformLocation(h_ShaderProgram_PS, string);
	}

	loc_material_PS.ambient_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.ambient_color");
	loc_material_PS.diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.diffuse_color");
	loc_material_PS.specular_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_color");
	loc_material_PS.emissive_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.emissive_color");
	loc_material_PS.specular_exponent = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_exponent");

	loc_texture_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_base_texture");
	loc_flag_fog = glGetUniformLocation(h_ShaderProgram_PS, "u_flag_fog");

	loc_screen_effect = glGetUniformLocation(h_ShaderProgram_PS, "screen_effect");
	loc_screen_frequency = glGetUniformLocation(h_ShaderProgram_PS, "screen_frequency");
	loc_screen_width = glGetUniformLocation(h_ShaderProgram_PS, "screen_width");
	loc_blind_effect = glGetUniformLocation(h_ShaderProgram_PS, "u_blind_effect");


	// setting for Gouraud shader
	loc_global_ambient_color_GS = glGetUniformLocation(h_ShaderProgram_GS, "u_global_ambient_color");
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light_GS[i].light_on = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light_GS[i].position = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light_GS[i].ambient_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light_GS[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light_GS[i].specular_color = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light_GS[i].spot_direction = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light_GS[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light_GS[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light_GS[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_GS, string);
		sprintf(string, "u_light[%d].blind_effect", i);
		loc_light_GS[i].blind_effect = glGetUniformLocation(h_ShaderProgram_GS, string);
	}

	loc_material_GS.ambient_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.ambient_color");
	loc_material_GS.diffuse_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.diffuse_color");
	loc_material_GS.specular_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_color");
	loc_material_GS.emissive_color = glGetUniformLocation(h_ShaderProgram_GS, "u_material.emissive_color");
	loc_material_GS.specular_exponent = glGetUniformLocation(h_ShaderProgram_GS, "u_material.specular_exponent");
}

void initialize_lights_and_material(void) { // follow OpenGL conventions for initialization

	// for Phong Shader
	glUseProgram(h_ShaderProgram_PS);

	glUniform4f(loc_global_ambient_color_PS, 0.115f, 0.115f, 0.115f, 1.0f);
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light_PS[i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light_PS[i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light_PS[i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light_PS[i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light_PS[i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light_PS[i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light_PS[i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light_PS[i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light_PS[i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light_PS[i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light_PS[i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
		if(i == 3) glUniform1i(loc_light_PS[i].blind_effect, 1); // blind effect for only 4th light
		else glUniform1i(loc_light_PS[i].blind_effect, 0);
	}

	glUniform4f(loc_material_PS.ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material_PS.diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material_PS.specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material_PS.emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material_PS.specular_exponent, 0.0f); // [0.0, 128.0]

	glUniform1i(loc_screen_effect, 0);
	glUniform1f(loc_screen_frequency, 10.0f);
	glUniform1f(loc_screen_width, 0.1f);
	glUniform1i(loc_blind_effect, 0);

	// for Gouraud Shader
	glUseProgram(h_ShaderProgram_GS);

	glUniform4f(loc_global_ambient_color_GS, 0.115f, 0.115f, 0.115f, 1.0f);
	for (int i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light_GS[i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light_GS[i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light_GS[i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light_GS[i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light_GS[i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light_GS[i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light_GS[i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light_GS[i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light_GS[i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light_GS[i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light_GS[i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}

	glUniform4f(loc_material_GS.ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material_GS.diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material_GS.specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material_GS.emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material_GS.specular_exponent, 0.0f); // [0.0, 128.0]

	glUseProgram(0);

}

void initialize_flags(void) {
	special_tex_flag = 0;
	flag_fog = 0;
	phong_or_gouraud = 0;

	glUseProgram(h_ShaderProgram_PS);
	glUniform1i(loc_flag_fog, flag_fog);
	glUseProgram(0);
}

#define PRINT_DEBUG_INFO
void initialize_OpenGL(void) {

	glEnable(GL_MULTISAMPLE);

	// initialize the 0th camera.
	camera[0].pos = glm::vec3(500.0f, 300.0f, 500.0f);
	camera[0].prp = 2.5f * camera[0].pos;
	camera[0].vrp = glm::vec3(0.0f, 0.0f, 1.0f);
	camera[0].vup = glm::vec3(0.0f, 1.0f, 0.0f);
	ViewMatrix[0] = glm::lookAt(camera[0].prp, camera[0].vrp, camera[0].vup);
	camera[0].vup = glm::vec3(ViewMatrix[0][0].y, ViewMatrix[0][1].y, ViewMatrix[0][2].y); // in this example code, make vup always equal to the v direction.

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 0", ViewMatrix[0]);
#endif

	camera[0].fov_y = 45.0f;
	camera[0].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[0].near_clip = 1.0f;
	camera[0].far_clip = 10000.0f;
	camera[0].zoom_factor = 1.0f; // will be used for zoomming in and out.

	//initialize the 1st camera.
	camera[1].pos = glm::vec3(-500.0f, 300.0f, -500.0f);
	camera[1].prp = 10.0f * camera[1].pos;

	camera[1].vrp = glm::vec3(0.0f, 0.0f, 1.0f);
	camera[1].vup = glm::vec3(0.0f, 1.0f, 0.0f);
	ViewMatrix[1] = glm::lookAt(camera[1].prp, camera[1].vrp, camera[1].vup);
	camera[1].vup = glm::vec3(ViewMatrix[1][0].y, ViewMatrix[1][1].y, ViewMatrix[1][2].y); // in this example code, make vup always equal to the v direction.

#ifdef PRINT_DEBUG_INFO 
	print_mat4("Cam 1", ViewMatrix[1]);
#endif

	camera[1].fov_y = 45.0f;
	camera[1].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[1].near_clip = 1000.0f;
	camera[1].far_clip = 10000.0f;
	camera[1].zoom_factor = 1.0f; // will be used for zoomming in and out.

  	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	initialize_lights_and_material();
	initialize_flags();

	glGenTextures(N_TEXTURES_USED, texture_names);
}


void set_up_scene_lights(void) {

	light[0].light_on = 1;
	light[0].position[0] = -3000.0f; light[0].position[1] = 1000.0f; 	// point light position in WC
	light[0].position[2] = -3000.0f; light[0].position[3] = 1.0f;

	light[0].ambient_color[0] = 0.13f; light[0].ambient_color[1] = 0.13f;
	light[0].ambient_color[2] = 0.13f; light[0].ambient_color[3] = 1.0f;

	// pink
	light[0].diffuse_color[0] = 211.0f/255.0f; light[0].diffuse_color[1] = 39.0f/255.0f;
	light[0].diffuse_color[2] = 208.0f/255.0f; light[0].diffuse_color[3] = 0.5f;

	// hot pink
	light[0].specular_color[0] = 1.0f; light[0].specular_color[1] = 20.0f/255.0f; 
	light[0].specular_color[2] = 106.0f / 255.0f ; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: use light 1
	light[1].light_on = 1;
	light[1].position[0] = 10000.0f; light[1].position[1] = 3000.0f; // spot light position in WC
	light[1].position[2] = 3000.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.13f; light[1].ambient_color[1] = 0.13f;
	light[1].ambient_color[2] = 0.13f; light[1].ambient_color[3] = 1.0f;

	// mint
	light[1].diffuse_color[0] = 30.0f/ 255.0f; light[1].diffuse_color[1] = 190.0f/255.0f;
	light[1].diffuse_color[2] = 104.0f / 255.0f; light[1].diffuse_color[3] = 0.3f;

	// green
	light[1].specular_color[0] = 19.0f / 255.0f; light[1].specular_color[1] = 190.0f / 255.0f;
	light[1].specular_color[2] = 36.0f / 255.0f; light[1].specular_color[3] = 0.8f;

	// point_light_EC: use light 2

	light[2].light_on = 0;
	light[2].position[0] = 0.0f; light[2].position[1] = 0.0f; 	// point light position in EC
	light[2].position[2] = 0.0f; light[2].position[3] = 1.0f;

	light[2].ambient_color[0] = 0.13f; light[2].ambient_color[1] = 0.13f;
	light[2].ambient_color[2] = 0.13f; light[2].ambient_color[3] = 1.0f;

	// yellow
	light[2].diffuse_color[0] = 1.0f; light[2].diffuse_color[1] = 213.0f/255.0f;
	light[2].diffuse_color[2] = 0.0f; light[2].diffuse_color[3] = 1.0f;

	// lemon 
	light[2].specular_color[0] = 248.0f/ 255.0f; light[2].specular_color[1] = 233.0f/255.0f;
	light[2].specular_color[2] = 155.0f/255.0f; light[2].specular_color[3] = 1.0f;

	light[2].spot_direction[0] = 0.0f; light[2].spot_direction[1] = -1.0f; // spot light direction in WC
	light[2].spot_direction[2] = 0.0f;
	light[2].spot_cutoff_angle = 30.0f;
	light[2].spot_exponent = 30.0f;

	// point_light_MC: use light 3 (to the dragon)

	light[3].light_on = 0;
	light[3].position[0] = 0.0f; light[3].position[1] = 10.0f; 	// point light position in MC
	light[3].position[2] = 0.0f; light[3].position[3] = 1.0f;

	light[3].ambient_color[0] = 0.13f; light[3].ambient_color[1] = 0.13f;
	light[3].ambient_color[2] = 0.13f; light[3].ambient_color[3] = 1.0f;

	// light purple
	light[3].diffuse_color[0] = 151.0f / 255.0f; light[3].diffuse_color[1] = 93.0f / 255.0f ;
	light[3].diffuse_color[2] = 238.0f/ 255.0f; light[3].diffuse_color[3] = 1.0f;

	// purple
	light[3].specular_color[0] = 84.0f / 255.0f; light[3].specular_color[1] = 14.0f/ 255.0f;
	light[3].specular_color[2] = 190.0f / 255.0f; light[3].specular_color[3] = 1.0f;

	light[3].spot_direction[0] = 0.0f; light[3].spot_direction[1] = -1.0f; // spot light direction in WC
	light[3].spot_direction[2] = -1.0f;
	light[3].spot_cutoff_angle = 40.0f;
	light[3].spot_exponent = 30.0f;

	// for Phong Shader
	glUseProgram(h_ShaderProgram_PS);

	// light 0, 1 are in WC
	glUniform1i(loc_light_PS[0].light_on, light[0].light_on);
	glUniform4fv(loc_light_PS[0].position, 1, light[0].position);
	glUniform4fv(loc_light_PS[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light_PS[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light_PS[0].specular_color, 1, light[0].specular_color);
	
	glUniform1i(loc_light_PS[1].light_on, light[1].light_on);
	glUniform4fv(loc_light_PS[1].position, 1, light[1].position);
	glUniform4fv(loc_light_PS[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light_PS[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light_PS[1].specular_color, 1, light[1].specular_color);
	
	// shading light from camera in EC
	glUniform1i(loc_light_PS[2].light_on, light[2].light_on);
	glm::vec4 position_EC = ViewMatrix[0] * glm::vec4(light[2].position[0], light[2].position[1],
												light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light_PS[2].position, 1, &position_EC[0]); 
	glUniform4fv(loc_light_PS[2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light_PS[2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light_PS[2].specular_color, 1, light[2].specular_color);
	glm::vec3 direction_EC = glm::mat3(ViewMatrix[0]) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1], 
																light[2].spot_direction[2]);
	glUniform3fv(loc_light_PS[2].spot_direction, 1, &direction_EC[0]); 
	glUniform1f(loc_light_PS[2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light_PS[2].spot_exponent, light[2].spot_exponent);
	
	// for Goraud Shader
	glUseProgram(h_ShaderProgram_GS);

	// light 0, 1 are in WC
	glUniform1i(loc_light_GS[0].light_on, light[0].light_on);
	glUniform4fv(loc_light_GS[0].position, 1, light[0].position);
	glUniform4fv(loc_light_GS[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light_GS[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light_GS[0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light_GS[1].light_on, light[1].light_on);
	glUniform4fv(loc_light_GS[1].position, 1, light[1].position);
	glUniform4fv(loc_light_GS[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light_GS[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light_GS[1].specular_color, 1, light[1].specular_color);

	// shading light from camera in EC
	glUniform1i(loc_light_GS[2].light_on, light[2].light_on);
	position_EC = ViewMatrix[0] * glm::vec4(light[2].position[0], light[2].position[1],
		light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light_GS[2].position, 1, &position_EC[0]);
	glUniform4fv(loc_light_GS[2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light_GS[2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light_GS[2].specular_color, 1, light[2].specular_color);
	direction_EC = glm::mat3(ViewMatrix[0]) * glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1],
		light[2].spot_direction[2]);
	glUniform3fv(loc_light_GS[2].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light_GS[2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light_GS[2].spot_exponent, light[2].spot_exponent);
	glUseProgram(0);
}

void initialize_screen(void) {
	screen_max_flag = 0;
	screen_frequency = 10.0f;
	screen_width = 0.125f;
}

void prepare_scene(void) {
	prepare_axes();
	prepare_floor();
	prepare_tiger();
	prepare_ben();
	prepare_wolf();
	prepare_dragon();
	prepare_godzilla();
	prepare_ironman();
	set_up_scene_lights();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "20140021 ±è»óÈÆ HW6";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', '2', '3', 't', 'q', 'r', 'w', 'x', 'y', 'z', 'up', 'down', 'right', 'left', 'page up', 'page down', 'r', 'ESC', and mouse wheel (rolling)"  };

	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}