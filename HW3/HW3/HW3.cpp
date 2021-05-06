#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
#include <math.h>
#define _USE_MATH_DEFINES

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define NUM_BUSTER_AIRPLANE 100
#define NUM_CRASHED_CAR 50
#define NUM_COCKTAIL_AS_FUEL 30
#define COCKTAIL_HEIGHT 450
#define NUM_CAKE 100

GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define LOC_VERTEX 0

int win_width = 0, win_height = 0; 
float centerx, centery;
float and_centerx = 60.0f, and_centery = 60.0f;
float arm_centerx = and_centerx, arm_centery = and_centery;
float airp_x = 0.0f, airp_y = WINDOW_HEIGHT - 60.0f, min_airp_x = win_width;
float sword_fall_x = 0.0f, sword_fall_y = 0.0f;
float airp_pos_x[NUM_BUSTER_AIRPLANE];
float airp_pos_y[NUM_BUSTER_AIRPLANE];
float sword_pos_x[NUM_BUSTER_AIRPLANE];
float sword_pos_y[NUM_BUSTER_AIRPLANE];
float car_x = WINDOW_WIDTH, car_y = 0.0f;
float car_pos_x[NUM_CRASHED_CAR];
float car_pos_y[NUM_CRASHED_CAR];
float fuel_pos_x[NUM_COCKTAIL_AS_FUEL];
float fuel_pos_y[NUM_COCKTAIL_AS_FUEL];
float cake_pos_x[NUM_CAKE];
float cake_pos_y[NUM_CAKE];
float max_car_y, max_fuel_y, max_cake_y;
float score = 5.0f; // the power of android

bool jump_flag = FALSE;
bool jump_up = FALSE;
bool attk_flag = FALSE; // attack
bool return_flag = FALSE; // when arm return
bool retrieve_flag = FALSE; // retrieve arm
bool buster_flag = FALSE; // buster call flag
bool accident_flag = FALSE; // when car meets android
bool car_flag = FALSE; // car appear
bool fuel_flag = FALSE; // pour cocktail to android
bool cake_flag = FALSE; // pop cake!

GLuint texture;

// 2D 물체 정의 부분은 objects.h 파일로 분리
// 새로운 물체 추가 시 prepare_scene() 함수에서 해당 물체에 대한 prepare_***() 함수를 수행함.
// (필수는 아니나 올바른 코딩을 위하여) cleanup() 함수에서 해당 resource를 free 시킴.
#include "objects.h"

unsigned int timestamp = 0;
unsigned int arm_timestamp = 0;
void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	arm_timestamp = (arm_timestamp + 1) % UINT_MAX;
		glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void accident();

float jump_clock = timestamp % 200 - 100.0f;
float jump_acc = 0.1f;
float accel_arm = 0.1f;
float gravity = 0.01f;
float car_gravity = 0.01f;
float grav_acc = 0.1f;
float car_accel = 0.1f;
float cocktail_angle = 0.1f;
float cocktail_acc = 5.0f;
float fuel_gravity = 0.01f;
float fuel_accel = 0.1f;
float grav_acc_fuel = 0.1f;
float head_angle = 0.1f;
float head_acc = 5.0f;
float head_x,head_y;
float cake_gravity = 0.01f;
float cake_accel = 0.1f;
float grav_acc_cake = 0.1f;
float rand_size;

void display(void) {

	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	int cake_clock = timestamp % 360;
	float rotation_angle_cake = atanf(100.0f * TO_RADIAN * cosf(cake_clock * TO_RADIAN));


	if (!cake_flag) {
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(win_width / 2.0f, 10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(score, score, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cake();
		if (win_width / 2.0f - score * 8.0f <= centerx && centerx <= win_width / 2.0f + score * 8.0f && 10.0f <= centery && centery <= score * 10.0f) cake_flag = TRUE;
	}
	else {
		for (int i = 0; i < NUM_CAKE; i++) {
			if (score * 10.0f + cake_accel * max_cake_y - cake_gravity > 0.0f) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.0f + cake_accel * cake_pos_x[i], score* 10.0f + cake_accel * cake_pos_y[i] - cake_gravity, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(rand_size, rand_size, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_body_android();
				draw_head_android();
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(win_width / 2.0f + cake_accel * cake_pos_x[i], score * 10.0f + cake_accel * cake_pos_y[i] - cake_gravity + rand_size*2.0f, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(rand_size, rand_size, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_arm_android();
			}
			else {
				cake_flag = FALSE;
				cake_accel = 0.1f;
				cake_gravity = 0.1f;
				grav_acc_cake = 0.1f;
				score = 5.0f;

			}
		}
		cake_accel += 15.0f;
		cake_gravity += 0.98f * grav_acc_cake;
		grav_acc_cake += 0.1f;
	}

	if (jump_flag) {
		if (jump_up) {
			and_centery -= jump_clock / 13.0f;
			if (and_centery > 250.0f) jump_up = FALSE;
		}
		else { // jump_down
			if (and_centery >= 245.0f) {
				and_centery += jump_clock / 70.0f;
			}// 체공시간 고려 
			else {
				and_centery += jump_clock / 30.0f - gravity;
				gravity += 0.098f * jump_acc;
				jump_acc += 0.01;
			}
			if (and_centery < 60.0f) jump_flag = FALSE;
		}
	}

	int fuel_clock = timestamp % 360;
	float rotation_angle_fuel = atanf(100.0f * TO_RADIAN * cosf(fuel_clock * TO_RADIAN));

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(and_centerx, and_centery, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	if (attk_flag && !fuel_flag) {
		draw_body_android();
		draw_head_android();
		if (retrieve_flag) {
			attk_flag = FALSE;
		}
	}
	else if(!fuel_flag){
		draw_body_android();
		draw_arm_android();
		draw_head_android();
	}
	else { // pour fuel to androide 
		draw_body_android();
		if(!attk_flag)draw_arm_android();
		if (retrieve_flag) {
			attk_flag = FALSE;
		}
		if (head_x >= -3.0f) head_x -= 0.15f;
		if (head_y < 6.0f) head_y += 0.3f;

		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(head_x, head_y, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, head_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_head_android();
		if (head_angle >= 135.0f) {
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(and_centerx + 60.0f, COCKTAIL_HEIGHT, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
			ModelMatrix = glm::rotate(ModelMatrix, cocktail_angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_cocktail();
			if (cocktail_angle <= 63.0f) {
				cocktail_angle += 0.15f * cocktail_acc;
				cocktail_acc -= 0.015f;
			}
			else {
				for (int i = 0; i < NUM_COCKTAIL_AS_FUEL; i++) {
					if (COCKTAIL_HEIGHT + fuel_accel * fuel_pos_y[i] - fuel_gravity > and_centery + 23.0f) {
						ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(and_centerx + 35.0f + fuel_accel * fuel_pos_x[i], COCKTAIL_HEIGHT+ fuel_accel * fuel_pos_y[i] - fuel_gravity, 1.0f));
						ModelMatrix = glm::rotate(ModelMatrix, rand() % 180 * rotation_angle_fuel, glm::vec3(0.0f, 0.0f, 1.0f));
						ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
						glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
						draw_cocktail();
						score += 0.001f;
					}
				}
				fuel_accel += 3.0f;
				fuel_gravity += 0.98f * grav_acc_fuel;
				grav_acc_fuel += 0.1f;
			}

			if (COCKTAIL_HEIGHT + fuel_accel * max_fuel_y - fuel_gravity < 0) {
				fuel_flag = FALSE;
				cocktail_angle = 0.1f;
				cocktail_acc = 5.0f;
				fuel_accel = 0;
				fuel_gravity = 0.1f;
				grav_acc_fuel = 0.1f;
				head_angle = 0.1f;
				head_acc = 0.1f;
				head_x = 0;
				head_y = 0;
			}
		}
		else {
			head_angle += 1.0f * head_acc;
			head_acc += 0.3f;
		}
	}

	// draw flying arms
	if (arm_centerx >= win_width) return_flag = TRUE;
	if (return_flag && (fabs(arm_centerx - and_centerx) <= 10.0f)) retrieve_flag = TRUE;

	int arm_clock = arm_timestamp % 360;
	float rotation_angle_arm = atanf(100.0f * TO_RADIAN * sinf(arm_clock * TO_RADIAN));

	if (!return_flag) {
		arm_centerx += (arm_clock / 15.0f) + accel_arm;
		arm_centery += (arm_clock / (10.0f + and_centerx/5.0f ))*rotation_angle_arm;
	}
	else {
		arm_centerx += (arm_clock / 15.0f) - accel_arm;
		if(and_centerx>=win_width/2-200.0f)	arm_centery += (arm_clock / (10.0f + 30.0f*and_centerx)) * rotation_angle_arm;
		else arm_centery += arm_clock / (30.0f + and_centerx / 3.0f) * rotation_angle_arm;
	}
	accel_arm += 0.1f;

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(arm_centerx, arm_centery, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10.0f, 10.0f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, rotation_angle_arm, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	if (attk_flag) {
		draw_arm_android();
	}

	// buster call
	int buster_timestamp = timestamp % 720;
	float rotation_angle_sword = atanf(180.0f*TO_RADIAN * cosf(buster_timestamp * TO_RADIAN));

	int scale_timestamp = timestamp % 180;
	float airplane_scale_factor =  atanf(60.0f*TO_RADIAN* sinf(scale_timestamp* TO_RADIAN)) + 1.5f;
	
	if (buster_flag) {
		if (airp_x - 3 * min_airp_x < 5 * win_width) {
			airp_x += 6.0f;
		}
		else {
			buster_flag = FALSE;
			airp_x = 0;
			min_airp_x = win_width;
			sword_fall_x = 0.0f;
			sword_fall_y = 0.0f;
			score+= 0.5f;
		}

		for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airp_x - 3 * airp_pos_x[i], airp_y - airp_pos_y[i], 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, TO_RADIAN * 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(airplane_scale_factor, airplane_scale_factor, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_airplane();
		}

		if (0 <= buster_timestamp < 180) {
			sword_fall_x += 1.0f;
			sword_fall_y += 1.0f;
			for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airp_x - 3 * airp_pos_x[i] - sword_fall_x, airp_y - airp_pos_y[i] - sword_fall_y, 0.0f));
				ModelMatrix = glm::rotate(ModelMatrix, buster_timestamp*rotation_angle_sword, glm::vec3(0.0f, 0.0f, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_sword();
			}
			if (buster_timestamp == 179) {
				sword_fall_x = 0.0f;
				sword_fall_y = 0.0f;
				score+=0.5f;
			}
		}
		if (180 <= buster_timestamp < 360) {
			sword_fall_x += 1.0f;
			sword_fall_y += 1.0f;
			for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airp_x - 3 * airp_pos_x[i] - sword_fall_x, airp_y - airp_pos_y[i] - sword_fall_y, 0.0f));
				ModelMatrix = glm::rotate(ModelMatrix, buster_timestamp * rotation_angle_sword, glm::vec3(0.0f, 0.0f, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_sword();
			}
			if (buster_timestamp == 359) {
				sword_fall_x = 0.0f;
				sword_fall_y = 0.0f;
				score += 0.5f;
			}
		}
		if (360 <= buster_timestamp < 540) {
			sword_fall_x += 1.0f;
			sword_fall_y += 1.0f;
			for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airp_x - 3 * airp_pos_x[i] - sword_fall_x, airp_y - airp_pos_y[i] - sword_fall_y, 0.0f));
				ModelMatrix = glm::rotate(ModelMatrix, buster_timestamp * rotation_angle_sword, glm::vec3(0.0f, 0.0f, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_sword();
			}
			if (buster_timestamp == 539) {
				sword_fall_x = 0.0f;
				sword_fall_y = 0.0f;
				score += 0.5f;
			}
		}
		if (540 <= buster_timestamp < 720) {
			sword_fall_x += 1.0f;
			sword_fall_y += 1.0f;
			for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airp_x - 3 * airp_pos_x[i] - sword_fall_x, airp_y - airp_pos_y[i] - sword_fall_y, 0.0f));
				ModelMatrix = glm::rotate(ModelMatrix, buster_timestamp * rotation_angle_sword, glm::vec3(0.0f, 0.0f, 1.0f));
				ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_sword();
			}
		}
	}

	//draw_car2

	int accident_timestamp = timestamp % 60;
	float rotation_angle_car = atanf(60.0f * TO_RADIAN * cosf(accident_timestamp * TO_RADIAN));

	if (car_flag) {
		if (!accident_flag && fabs(car_x - and_centerx) <= 30.0f && fabs(and_centery - car_y) <= 100.0f) {
			accident_flag = TRUE;
			accident();
			score += 0.5f;
		}

		if (!accident_flag) {
			car_x -= 5.0f;
			car_y = 60.0f;
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car_x, car_y, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.0f, 5.0f, 1.0f));
			ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_car2();
		}
		else { // when car meets android
			for (int i = 0; i < NUM_CRASHED_CAR; i++) {
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car_x + car_accel * car_pos_x[i], car_y + car_accel * car_pos_y[i] - car_gravity, 1.0f));
				ModelMatrix = glm::rotate(ModelMatrix, rand() % 180 * rotation_angle_car, glm::vec3(0.0f, 0.0f, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_car2();
			}
			car_accel += 10.0f;
			car_gravity += 0.98f * grav_acc;
			grav_acc += 0.1f;
			if (car_y + car_accel * max_car_y - car_gravity < 0.0f) {
				car_x = win_width;
				car_flag = FALSE;
				accident_flag = FALSE;
				car_accel = 0;
				car_gravity = 0.1f;
				grav_acc = 0.1f;
			}
		}

		if (car_x < -80.0f) {
			car_flag = FALSE; // fail to make accident
			car_x = win_width;
		}
	}

	glDeleteTextures(1,&texture);
	glFlush();	
} 

void accident() {
	srand(timestamp % 360);
	for (int i = 0; i < NUM_CRASHED_CAR; i++) {
		car_pos_x[i] = (rand() % 720 - 360.0f) / 360.0f;
		car_pos_y[i] = (rand() % 720 - 360.0f) / 360.0f;
		if (car_pos_y[i] > max_car_y) max_car_y = car_pos_y[i];
	}
}

void pop_cake() {
	srand(timestamp % 360);
	rand_size = rand() % 10 * 1.0f + 1.0f;
	for (int i = 0; i < NUM_CAKE; i++) {
		cake_pos_x[i] = (rand() % 720 - 360.0f) / 360.0f;
		cake_pos_y[i] = (rand() % 720 - 360.0f) / 360.0f;
		if (cake_pos_y[i] > max_cake_y) max_cake_y = cake_pos_y[i];
	}
}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 32: // SPACE bar
		if (!jump_flag) { // jump!
			jump_flag = TRUE;
			jump_up = TRUE;
			gravity = 0.01f;
		}
		break;
	case 65: //A
	case 97: //a
		if (!attk_flag) { // attack!
			attk_flag = TRUE;
			return_flag = FALSE;
			retrieve_flag = FALSE;
			arm_timestamp = 0;
			accel_arm = 0.1f;
			arm_centerx = and_centerx;
			arm_centery = and_centery;
		}
		break;
	case 66: //B
	case 98: //b
		if (!buster_flag) { // buster call!
			buster_flag = TRUE;
			srand(timestamp);
			for (int i = 0; i < NUM_BUSTER_AIRPLANE; i++) {
				airp_pos_x[i] = rand() % 1440 - 50.0f;
				airp_pos_y[i] = rand() % 120;
				if (airp_pos_x[i] < min_airp_x) min_airp_x = airp_pos_x[i];
			}
		}
		break;
	case 67: //C
	case 99: //c
		if (!car_flag) { // car start to drive
			car_flag = TRUE;
		}
		break;
	case 70: //F
	case 102: //f
		if (!fuel_flag) { // give the android fuel
			fuel_flag = TRUE;
			srand(timestamp % 360);
			for (int i = 0; i < NUM_COCKTAIL_AS_FUEL; i++) {
				fuel_pos_x[i] = (rand() % 4 - 4) / 8.0f;
				fuel_pos_y[i] = (rand() % 360 - 720) / 360.0f;
				if (fuel_pos_y[i] > max_fuel_y) max_fuel_y = fuel_pos_y[i];
			}
			score += 0.5f;
		}
		break;
	}

}

void specialinput(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		if (and_centerx > 0.0f) {
			and_centerx -= (10.0f);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (and_centerx < win_width) {
			and_centerx += (10.0f);
		}
		break;
	}
}

int leftbuttonpressed = 0;
void mouse(int button, int state, int x, int y) {
	centerx = x, centery = win_height - y;
	if (!cake_flag && (button == GLUT_LEFT_BUTTON) && state == (GLUT_DOWN))
		leftbuttonpressed = 1;
	else if ((button == GLUT_LEFT_BUTTON) && state == (GLUT_UP))
		leftbuttonpressed = 0;
}

void reshape(int width, int height) {
	win_width = width, win_height = height;

  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(0.0, win_width / 1.0, 
		0.0, win_height /1.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	glDeleteVertexArrays(1, &VAO_cake);
	glDeleteBuffers(1, &VBO_cake);

	glDeleteVertexArrays(1, &VAO_sword);
	glDeleteBuffers(1, &VBO_sword);

	glDeleteVertexArrays(1, &VAO_head_android);
	glDeleteBuffers(1, &VBO_head_android);

	glDeleteVertexArrays(1, &VAO_body_android);
	glDeleteBuffers(1, &VBO_body_android);

	glDeleteVertexArrays(1, &VAO_arm_android);
	glDeleteBuffers(1, &VBO_arm_android);

	glDeleteVertexArrays(1, &VAO_car2);
	glDeleteBuffers(1, &VBO_car2);

	glDeleteVertexArrays(1, &VAO_cocktail);
	glDeleteBuffers(1, &VBO_cocktail);
}

void update() {
	if (leftbuttonpressed == 1) {
		pop_cake();
		centerx = 0.0f, centery = 0.0f;
		leftbuttonpressed = 0;
	}
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialinput);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
	glutIdleFunc(update);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(229 / 255.0f, 229 / 255.0f, 229 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_airplane();
	prepare_cake();
	prepare_head_android();
	prepare_body_android();
	prepare_arm_android();
	prepare_sword();
	prepare_car2();
	prepare_cocktail();
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

void greetings(char *program_name, char messages[][512], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    ANDORID wanna be the IRON-MAN.\n");
	fprintf(stdout, "      So he decided to be a IRON-DROID!!\n");
	fprintf(stdout, "         Now, he become super strong, NO-ONE can't stop him...\n\n");

	fprintf(stdout, "		CREATOR : KIM SANG HOON		\n\n");
	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 8
int main(int argc, char *argv[]) {
	char program_name[64] = "I'M IRON-DORID";
	char messages[N_MESSAGE_LINES][512] = {
		"    - press the key below regardless of case\n"
		"    - 'ESC' : If you wanna shut program\n"
		"    - 'A' : the arm of andriod fly \n"
		"    - 'B' : buster call\n"
		"    - 'C' : car appear(and hit the android!)\n"
		"    - 'F' : give fuel to android(fuel is alcohol)\n"
		"    - 'SPACE BAR' : android jump \n"
		"    - Mouse left button : pop the cake (celebrate the growth of IRON-DROID!)\n"
	};

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}


