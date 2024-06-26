
#include <vector>

#include <gltools.h>
#include <rsw_math.h>


#include <SDL2/SDL.h>

#include <stdio.h>

#define WIDTH 640
#define HEIGHT 640

using namespace std;

using rsw::vec3;
using rsw::mat4;


SDL_Window* window;
SDL_GLContext glcontext;

int polygon_mode;
int use_elements;
int minus_1;
mat4 mvp_mat;
int mvp_loc;

void cleanup();
void setup_context();
int handle_events();


int main(int argc, char** argv)
{
	setup_context();


	vector<vec3> tri_strips;
	vector<vec3> colors;
	vector<GLuint> strip_elems;
	vec3 offset(10, 10, 0);

	int sq_dim = 20;
	vector<GLint> firsts;
	// can be GLsizeiptr or GLintptr or
	// really any int type that is the same size
	// as a pointer
	vector<GLintptr> first_elems;
	vector<GLsizei> counts;

	const int cols = 25;
	const int rows = 25;

	for (int j=0; j<rows; j++) {
		for (int i=0; i<cols; i++) {
			firsts.push_back(tri_strips.size());
			first_elems.push_back(tri_strips.size()*sizeof(GLuint));
			counts.push_back(4);

			tri_strips.push_back(vec3(i*(sq_dim+5),        j*(sq_dim+5),        0));
			tri_strips.push_back(vec3(i*(sq_dim+5),        j*(sq_dim+5)+sq_dim, 0));
			tri_strips.push_back(vec3(i*(sq_dim+5)+sq_dim, j*(sq_dim+5),        0));
			tri_strips.push_back(vec3(i*(sq_dim+5)+sq_dim, j*(sq_dim+5)+sq_dim, 0));

			colors.push_back(vec3(1,0,0));
			colors.push_back(vec3(0,1,0));
			colors.push_back(vec3(0,0,1));
			colors.push_back(vec3(0,0,0));

			strip_elems.push_back((j*cols+i)*4);
			strip_elems.push_back((j*cols+i)*4+1);
			strip_elems.push_back((j*cols+i)*4+2);
			strip_elems.push_back((j*cols+i)*4+3);
		}
	}
	for (int i=0; i<tri_strips.size(); i++) {
		tri_strips[i] += offset;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint square_buf;
	glGenBuffers(1, &square_buf);
	glBindBuffer(GL_ARRAY_BUFFER, square_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*tri_strips.size(), &tri_strips[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint color_buf;
	glGenBuffers(1, &color_buf);
	glBindBuffer(GL_ARRAY_BUFFER, color_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*colors.size(), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint elem_buf;
	glGenBuffers(1, &elem_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*strip_elems.size(), &strip_elems[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	const char vs_file[] = "../media/shaders/transform_vertcolors.vp";
	const char fs_file[] = "../media/shaders/smooth_color.fp";

	GLuint program = load_shader_file_pair(vs_file, fs_file);
	if (!program) {
		printf("failed to compile/link shaders\n");
		exit(0);
	}
	glUseProgram(program);

	mvp_loc = glGetUniformLocation(program, "mvp_mat");

	rsw::make_orthographic_matrix(mvp_mat, 0, WIDTH, 0, HEIGHT, 1, -1);
	glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, (float*)&mvp_mat);

	unsigned int old_time = 0, new_time=0, counter = 0;
	while (1) {
		if (handle_events())
			break;

		counter++;
		new_time = SDL_GetTicks();
		if (new_time - old_time > 3000) {
			printf("%f FPS\n", counter*1000.f/(new_time-old_time));
			old_time = new_time;
			counter = 0;
		}

		glClear(GL_COLOR_BUFFER_BIT);


		if (!use_elements) {
			glMultiDrawArrays(GL_TRIANGLE_STRIP, &firsts[0], &counts[0], 300);
		} else {
			glMultiDrawElements(GL_TRIANGLE_STRIP, &counts[0], GL_UNSIGNED_INT, (GLvoid* const*)(&first_elems[0]), 625);
		}

		SDL_GL_SwapWindow(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &square_buf);
	glDeleteProgram(program);

	cleanup();

	return 0;
}


void setup_context()
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(0);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("Multidraw", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (!window) {
		cleanup();
		exit(0);
	}

	glcontext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("Error: %s\n", glewGetErrorString(err));
		cleanup();
		exit(0);
	}

	check_errors(0, "Clearing stupid error after glewInit");

	int major, minor, profile;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);

	printf("OpenGL version %d.%d with profile %d\n", major, minor, profile);
}

void cleanup()
{
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

int handle_events()
{
	SDL_Event e;
	int sc;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return 1;
		} else if (e.type == SDL_KEYDOWN) {
			sc = e.key.keysym.scancode;

			if (sc == SDL_SCANCODE_ESCAPE) {
				return 1;
			} else if (sc == SDL_SCANCODE_P) {
				polygon_mode = (polygon_mode + 1) % 3;
				if (polygon_mode == 0)
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				else if (polygon_mode == 1)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			} else if (sc == SDL_SCANCODE_E) {
				use_elements = !use_elements;
				if (use_elements) {
					puts("Using MultiDrawElements");
				} else {
					puts("Using MultiDrawArrays");
				}
			} else if (sc == SDL_SCANCODE_M) {
				minus_1 = !minus_1;
				if (minus_1) {
					puts("minus_1");
					rsw::make_orthographic_matrix(mvp_mat, 0, WIDTH-1, 0, HEIGHT-1, 1, -1);
					glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, (float*)&mvp_mat);
				} else {
					puts("all the pixels");
					rsw::make_orthographic_matrix(mvp_mat, 0, WIDTH, 0, HEIGHT, 1, -1);
					glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, (float*)&mvp_mat);
				}
			}

		}
	}
	return 0;
}







