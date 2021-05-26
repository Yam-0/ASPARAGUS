#include "include/shader.h"

const char *ASP_Shader_Load(char *filename)
{
	FILE *sfh = fopen(filename, "r");
	if (sfh == NULL)
	{
		printf("Could not read file: %s\n", filename);
		ferror("error");
		fclose(sfh);
		return NULL;
	}

	printf("Read file: %s\n", filename);

	int nlc = 0;
	char c;
	for (c = getc(sfh); c != EOF; c = getc(sfh))
		if (c == '\n')
			nlc++;

	rewind(sfh);
	long int size = 0;
	fseek(sfh, -nlc, SEEK_END);
	size = ftell(sfh);
	rewind(sfh);

	char *shader = (char *)malloc(size);
	if (!shader)
	{
		printf("Memory error in ASP_LoadShader.\n");
		fclose(sfh);
		return NULL;
	}

	size_t readSize = fread(shader, 1, size, sfh);
	readSize = (readSize >= 0) ? readSize : 0;
	shader[readSize] = '\0';
	if (readSize != size)
	{
		printf("Read error in ASP_LoadShader.\n");
		printf("Read size %zu, malloc size %i.\n", readSize, size);
		fclose(sfh);
		return NULL;
	}

	printf("\nLoaded Shader \"%s\":\n%s\nEND\n", filename, shader);
	fclose(sfh);

	return shader;
}

GLuint ASP_Shader_Compile(GLuint shader, char *code)
{
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_TRUE)
	{
		printf("Compiled shader!\n");
		return shader;
	}
	else
	{
		printf("Failed to compile shader!\n");
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);
		return shader;
	}
}

struct ASP_Shader ASP_Shader_Create(char *filepath_vs, char *filepath_fs, size_t n, struct ASP_VertexAttribute attr[])
{
	struct ASP_Shader shader;
	char *vs, *fs;

	//Load and compile vertex shader
	vs = ASP_Shader_Load(filepath_vs);
	shader.vs_handle = glCreateShader(GL_VERTEX_SHADER);
	ASP_Shader_Compile(shader.vs_handle, vs);
	//free(vs);

	//Load and compile fragment shader
	fs = ASP_Shader_Load(filepath_fs);
	shader.fs_handle = glCreateShader(GL_FRAGMENT_SHADER);
	ASP_Shader_Compile(shader.fs_handle, fs);
	//free(fs);

	//Create and link shader program
	shader.shader_handle = glCreateProgram();
	glAttachShader(shader.shader_handle, shader.vs_handle);
	glAttachShader(shader.shader_handle, shader.fs_handle);

	for (size_t i = 0; i < n; i++)
	{
		glBindAttribLocation(shader.shader_handle, attr[i].index, attr[i].name);
	}

	glLinkProgram(shader.shader_handle);

	//Check link status
	GLint linked;
	glGetProgramiv(shader.shader_handle, GL_LINK_STATUS, &linked);

	if (linked)
	{
		printf("Linked shader!\n");
		return shader;
	}
	else
	{
		printf("Failed to Link shader!\n");
		char buffer[512];
		glGetProgramInfoLog(shader.shader_handle, 512, NULL, buffer);
		printf("Error: %s", buffer);
		return shader;
	}

	return shader;
}

void ASP_Shader_Destroy(struct ASP_Shader shader)
{
	glDeleteProgram(shader.shader_handle);
	glDeleteShader(shader.vs_handle);
	glDeleteShader(shader.fs_handle);
	return;
}
void ASP_Shader_Bind(struct ASP_Shader shader)
{
	glUseProgram(shader.shader_handle);
	return;
}
void ASP_Mat4f_uniform(struct ASP_Shader shader, char *name, mat4 m)
{
	glMatrixMode(GL_MODELVIEW);
	GLint location = glGetUniformLocation(shader.shader_handle, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, m[0]);
	return;
}
void ASP_Mat4f_camera(struct ASP_Shader shader, struct ASP_Camera *camera)
{
	ASP_Mat4f_uniform(shader, "p", camera->proj);
	ASP_Mat4f_uniform(shader, "v", camera->view);
	return;
}
