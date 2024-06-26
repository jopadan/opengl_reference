#include <GL/glew.h>

//included in SDL.h?
//#include <GL/gl.h>

#include <SDL2/SDL.h>

#include <stdio.h>

#define WIDTH 640
#define HEIGHT 480


SDL_Window* window;
SDL_GLContext glcontext;

void cleanup();
void setup_context();
int handle_events();


void check_errors(int n, const char* str);
int compile_shader_str(GLuint shader, const char* shader_str);
int link_program(GLuint program);
GLuint load_shader_pair(const char* vert_shader_src, const char* frag_shader_src);

void GLAPIENTRY debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


static const char vs_shader_str[] =
"#version 330 core                     \n"
"                                      \n"
"layout (location = 0) in vec4 vertex; \n"
"void main(void)                       \n"
"{\n"
"	gl_Position = vertex;\n"
"}";

static const char fs_shader_str[] =
"#version 330 core                     \n"
"                                      \n"
"uniform vec4 color;                   \n"
"out vec4 frag_color;                  \n"
"void main(void)                       \n"
"{                                     \n"
"	frag_color = color;                \n"
"}";

float line_width;
int aliased;
int blending = 1;
float granularity;
int hint_nicest;

int main(int argc, char** argv)
{
	setup_context();

	line_width = 1;
	aliased = 1;

	float range[2];
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range);
	printf("aliased range: %f %f\n", range[0], range[1]);

	glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, range);
	printf("smooth range: %f %f\n", range[0], range[1]);

	glGetFloatv(GL_SMOOTH_LINE_WIDTH_GRANULARITY, range);
	printf("smooth granularity: %f\n", range[0]);
	granularity = range[0];

	glGetFloatv(GL_MAX_TEXTURE_SIZE, range);
	printf("MAX_TEX_SIZE = %f\n", range[0]);

	glGetFloatv(GL_MAX_3D_TEXTURE_SIZE, range);
	printf("MAX_3D_TEX_SIZE = %f\n", range[0]);

	glGetFloatv(GL_MAX_ARRAY_TEXTURE_LAYERS, range);
	printf("MAX_ARRAY_TEX_LAYERS = %f\n", range[0]);

	glGetFloatv(GL_MAX_DEBUG_MESSAGE_LENGTH, range);
	printf("MAX_DEBUG_MESSAGE_LENGTH = %f\n", range[0]);

	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debug_callback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

	// testing debug output
	//glEnable(GL_LINE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float lines[] =
	{
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,

		0.0, 0.5, 0.0,
		0.5, 0.5, 0.0,

		-0.5, -0.75, 0.0,
		0.5, 0.0, 0.0,

		-0.75, 0.9, 0.0,
		-0.50, 0.0, 0.0,
	};

	//no error checking done for any of this except shader compilation
	GLuint program = load_shader_pair(vs_shader_str, fs_shader_str);
	if (!program) {
		printf("failed to compile/link shaders\n");
		exit(0);
	}

	glUseProgram(program);

	float Red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	int loc = glGetUniformLocation(program, "color");
	glUniform4fv(loc, 1, Red);

	//no default vao in core profile ...
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint line_buf;
	glGenBuffers(1, &line_buf);
	glBindBuffer(GL_ARRAY_BUFFER, line_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glClearColor(0, 0, 0, 1.0);

	//glDeleteVertexArrays(1, &vao);
	//glDeleteVertexArrays(1, 0);
	//check_errors(1, "hmmm");


	unsigned int old_time = 0, new_time=0, counter = 0;
	while (1) {
		if (handle_events())
			break;

		counter++;
		new_time = SDL_GetTicks();
		if (new_time - old_time > 3000) {
			printf("%f FPS\n", counter*1000.f/(new_time-old_time));
			fflush(stdout); //stupid windows doesn't flush with \n >:-/
			old_time = new_time;
			counter = 0;
		}

		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_LINES, 0, 8);

		//glDrawArrays(GL_LINES, 2, 2);
		//glDrawArrays(GL_LINES, 4, 2);

		SDL_GL_SwapWindow(window);
	}

	glDeleteBuffers(1, &line_buf);
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

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	window = SDL_CreateWindow("Test Aliasing Lines", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
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
	if (GLEW_ARB_debug_output) {
		puts("we should have debug output");
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

			switch (sc) {
			case SDL_SCANCODE_ESCAPE:
				return 1;
				break;
			case SDL_SCANCODE_UP:
				line_width += granularity;
				glLineWidth(line_width);
				printf("line_width = %f\n", line_width);
				break;
			case SDL_SCANCODE_DOWN:
				line_width -= granularity;
				if (line_width < 1.0) {
					line_width = 1.0f;
				}
				glLineWidth(line_width);
				printf("line_width = %f\n", line_width);
				break;
			case SDL_SCANCODE_SPACE:
				aliased = !aliased;
				if (!aliased) {
					glEnable(GL_LINE_SMOOTH);
					puts("smooth");
				} else {
					glDisable(GL_LINE_SMOOTH);
					puts("aliased");
				}
				break;
			case SDL_SCANCODE_B:
				blending = !blending;

				if (blending) {
					glEnable(GL_BLEND);
					puts("blending");
				} else {
					glDisable(GL_BLEND);
					puts("no blending");
				}
				break;
			case SDL_SCANCODE_H:
				hint_nicest = !hint_nicest;
				if (hint_nicest) {
					puts("nicest");
					glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				} else {
					puts("fastest");
					glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
				}
				break;
			}
			
		}
	}
	return 0;
}



// adapted from learnopengl.com
void GLAPIENTRY debug_callback(GLenum source,
                               GLenum type,
                               unsigned int id,
                               GLenum severity,
                               GLsizei length,
                               const char *message,
                               const void *userParam)
{
	// ignore non-significant error/warning codes
	//
	// TODO per the spec: "There can potentially be overlap between the
	// namespaces of two different pairs of source and type, so messages can
	// only be uniquely distinguished from each other by the full combination
	// of source, type and ID." and "The assignment of IDs ... is implementation
	// -dependent"
	//
	// so this is probably a bad idea
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	puts("---------------");
	printf("Debug message (%d): %s\n", id, message);

	switch (source) {
	case GL_DEBUG_SOURCE_API:             puts("Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   puts("Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: puts("Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     puts("Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     puts("Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           puts("Source: Other"); break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               puts("Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: puts("Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  puts("Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:         puts("Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:         puts("Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:              puts("Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          puts("Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:           puts("Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:               puts("Type: Other"); break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         puts("Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       puts("Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:          puts("Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: puts("Severity: notification"); break;
	}
	puts("");
}





void check_errors(int n, const char* str)
{
	GLenum error;
	int err = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		switch (error)
		{
		case GL_INVALID_ENUM:
			fprintf(stderr, "invalid enum\n");
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "invalid value\n");
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "invalid operation\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			fprintf(stderr, "invalid framebuffer operation\n");
			break;
		case GL_OUT_OF_MEMORY:
			fprintf(stderr, "out of memory\n");
			break;
		default:
			fprintf(stderr, "wtf?\n");
		}
		err = 1;
	}
	if (err)
		fprintf(stderr, "%d: %s\n\n", n, (!str)? "Errors cleared" : str);
}

#define BUF_SIZE 1000

int link_program(GLuint program)
{
	glLinkProgram(program);
	int status = 0;
	char info_buf[BUF_SIZE];
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		int len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		if (len > 0) {
			int written = 0;
			glGetProgramInfoLog(program, len, &written, info_buf);
			printf("Link failed:\n===============\n%s\n", info_buf);
		}
		return 0;
	}

	return program;
}

int compile_shader_str(GLuint shader, const char* shader_str)
{
	glShaderSource(shader, 1, &shader_str, NULL);
	glCompileShader(shader);

	int result;
	char shader_info_buf[BUF_SIZE];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 0 && length <= BUF_SIZE) {
			int written = 0;
			glGetShaderInfoLog(shader, BUF_SIZE, &written, shader_info_buf);

			printf("Compile failed:\n===============\n%s\n", shader_info_buf);
		}
		return 0;
	}
	return 1;
}

GLuint load_shader_pair(const char* vert_shader_src, const char* frag_shader_src)
{
	GLuint program, vert_shader, frag_shader;

	program = glCreateProgram();
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	if (!compile_shader_str(vert_shader, vert_shader_src))
		return 0;
	if (!compile_shader_str(frag_shader, frag_shader_src))
		return 0;

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	return link_program(program);
}

