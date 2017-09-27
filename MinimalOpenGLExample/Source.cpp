#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// Helper function to print out the compiler errors from GLSL
void PrintShaderInfoLog(GLint const Shader)
{
	int InfoLogLength = 0;
	int CharsWritten = 0;

	glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, & InfoLogLength);

	if (InfoLogLength > 0)
	{
		GLchar * InfoLog = new GLchar[InfoLogLength];
		glGetShaderInfoLog(Shader, InfoLogLength, & CharsWritten, InfoLog);
		std::cout << "Shader Info Log:" << std::endl << InfoLog << std::endl;
		delete [] InfoLog;
	}
}

int main()
{
	//////////////////////////////////
	// Create Window to render into //
	//////////////////////////////////

	GLFWwindow * window = nullptr;
	if (! glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (! window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (! gladLoadGL())
	{
		return -1;
	}

	//////////
	// Data //
	//////////

	// Vertex Shader
	char const * VertexShaderSource =
		"#version 150								\n"
		"in vec2 position;							\n"
		"void main()								\n"
		"{											\n"
		"	gl_Position = vec4(position, 0.0, 1.0);	\n"
		"}											\n";

	// Fragment Shader
	char const * FragmentShaderSource =
		"#version 150								\n"
		"out vec4 outColor;							\n"
		"void main()								\n"
		"{											\n"
		"	outColor = vec4(1.0, 1.0, 1.0, 1.0);	\n"
		"}											\n";

	// Vertex data
	GLfloat const Vertices[] = {
		0.0f,  0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	// Index data
	GLuint const Elements[] = {
		0, 1, 2
	};


	////////////////////
	// OpenGL Objects //
	////////////////////

	// Create VAO
	GLuint VAO;
	glGenVertexArrays(1, & VAO);
	glBindVertexArray(VAO);

	// Create VBO
	GLuint VBO;
	glGenBuffers(1, & VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create EBO (sometimes called IBO)
	GLuint EBO;
	glGenBuffers(1, & EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW);


	/////////////////////
	// Compile Shaders //
	/////////////////////

	// Compile vertex shader
	GLint Compiled;
	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, & VertexShaderSource, NULL);
	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, & Compiled);
	if (! Compiled)
	{
		std::cerr << "Failed to compile vertex shader!" << std::endl;
		PrintShaderInfoLog(VertexShader);
	}

	// Compile fragment shader
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, & FragmentShaderSource, NULL);
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, & Compiled);
	if (! Compiled)
	{
		std::cerr << "Failed to compile fragment shader!" << std::endl;
		PrintShaderInfoLog(FragmentShader);
	}

	// Compile shader
	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glBindFragDataLocation(ShaderProgram, 0, "outColor");
	glLinkProgram(ShaderProgram);
	glUseProgram(ShaderProgram);

	// Enable the attribute from our VBO
	GLint PositionAttribute = glGetAttribLocation(ShaderProgram, "position");
	glEnableVertexAttribArray(PositionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(PositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	///////////////
	// Main Loop //
	///////////////

	while (! glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/////////////
	// Cleanup //
	/////////////

	glDeleteProgram(ShaderProgram);
	glDeleteShader(FragmentShader);
	glDeleteShader(VertexShader);

	glDeleteBuffers(1, & EBO);
	glDeleteBuffers(1, & VBO);
	glDeleteVertexArrays(1, & VAO);

	return 0;
}
