#pragma once

typedef struct {
	glm::vec3 pos, prp, vrp, vup; // in this example code, make vup always equal to the v direction.
	float fov_y, aspect_ratio, near_clip, far_clip, zoom_factor;
} CAMERA;

typedef struct {
	int x, y, w, h;
} VIEWPORT;


#define NUMBER_OF_CAMERAS 2

CAMERA camera[NUMBER_OF_CAMERAS];
VIEWPORT viewport[NUMBER_OF_CAMERAS];

// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
glm::mat4 ViewProjectionMatrix[NUMBER_OF_CAMERAS], ViewMatrix[NUMBER_OF_CAMERAS], ProjectionMatrix[NUMBER_OF_CAMERAS];
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;

void print_mat4(const char* string, glm::mat4 M) {
	fprintf(stdout, "\n***** %s ******\n", string);
	for (int i = 0; i < 4; i++)
		fprintf(stdout, "*** COL[%d] (%f, %f, %f, %f)\n", i, M[i].x, M[i].y, M[i].z, M[i].w);
	fprintf(stdout, "**************\n\n");
}