// light and material definitions

typedef struct _Light_Parameters {
	int light_on;
	float position[4];
	float ambient_color[4], diffuse_color[4], specular_color[4];
	float spot_direction[3];
	float spot_exponent;
	float spot_cutoff_angle;
	float light_attenuation_factors[4]; // produce this effect only if .w != 0.0f
	int blind_effect;
} Light_Parameters;

typedef struct _loc_LIGHT_Parameters {
	GLint light_on;
	GLint position;
	GLint ambient_color, diffuse_color, specular_color;
	GLint spot_direction;
	GLint spot_exponent;
	GLint spot_cutoff_angle;
	GLint light_attenuation_factors;
	GLint blind_effect;
} loc_light_Parameters;

typedef struct _Material_Parameters {
	float ambient_color[4], diffuse_color[4], specular_color[4], emissive_color[4];
	float specular_exponent;
} Material_Parameters;

typedef struct _loc_Material_Parameters {
	GLint ambient_color, diffuse_color, specular_color, emissive_color;
	GLint specular_exponent;
} loc_Material_Parameters;

GLuint h_ShaderProgram_simple, h_ShaderProgram_PS, h_ShaderProgram_GS; // handles to shader programs

// for simple shaders
GLint loc_ModelViewProjectionMatrix_simple, loc_primitive_color;

// for Phong Shading (Textured) shaders
#define NUMBER_OF_LIGHT_SUPPORTED 4 
GLint loc_global_ambient_color_PS;
loc_light_Parameters loc_light_PS[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material_PS;
GLint loc_ModelViewProjectionMatrix_PS, loc_ModelViewMatrix_PS, loc_ModelViewMatrixInvTrans_PS;
GLint loc_texture_PS, loc_flag_fog;
// for screen effect in Phong shaders
GLint loc_screen_frequency, loc_screen_width, loc_screen_effect;
// for blind effect
GLint loc_blind_effect;

// for Gouraud Shading (Textured) shaders
GLint loc_global_ambient_color_GS;
loc_light_Parameters loc_light_GS[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material_GS;
GLint loc_ModelViewProjectionMatrix_GS, loc_ModelViewMatrix_GS, loc_ModelViewMatrixInvTrans_GS;

// lights in scene
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

