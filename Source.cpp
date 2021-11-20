#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float scale_number(float x, float oMin, float oMax, float nMin, float nMax);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_RED_BITS, );
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// check for fragment shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check linking errors
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float triangleVertices1[] = {
	0.5f, 0.9f, 0.0f, // top left  
	0.9f, 0.9f, 0.0f, // top right 
	0.9f, 0.5f, 0.0f,  // bottom right
	0.5f, 0.5f, 0.0f // bottom left
	};

	unsigned int indices[] = { // note that we start from 0 !
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// vertex buffer object and vertex array object
	unsigned int VBO1, VAO1;
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO1);
	// bind the vertex array object first, then bind and set vertex buffer(s), and then configure vertex attribute(s)
	glBindVertexArray(VAO1);
	// configure VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices1), triangleVertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	// unbind GL_ARRAY_BUFFER with following
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	float triangleVertices2[] = {
	-0.9f, 0.9f, 0.0f, // left  
	-0.9f, -0.9f, 0.0f, // right 
	 0.9f, -0.9f, 0.0f,  // top
	};

	float triangleVertices3[] = {
		0.1f, 0.4f, 0.0f, // left  
		0.5f, 0.0f, 0.0f, // right 
		0.1f, 0.0f, 0.0f  // top   
	};

	// another VBO, VAO
	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	// second
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// third
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices3), triangleVertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	// -----------------------------------------------------------

	float graph[] = {
		0.0f, 60.0f, 0.0f,
		1.0f, 57.1f, 0.0f,
		2.0f, 55.9f, 0.0f,
		3.0f, 54.2f, 0.0f,
		4.0f, 53.0f, 0.0f,
		5.0f, 51.5f, 0.0f,
		6.0f, 50.0f, 0.0f,
		7.0f, 48.8f, 0.0f,
		8.0f, 47.0f, 0.0f,
		9.0f, 45.1f, 0.0f,
		10.0f, 43.8f, 0.0f,
		11.0f, 42.1f, 0.0f,
		12.0f, 40.0f, 0.0f,
		13.0f, 38.3f, 0.0f,
		14.0f, 36.2f, 0.0f,
		15.0f, 34.6f, 0.0f,
		16.0f, 32.6f, 0.0f,
		17.0f, 30.1f, 0.0f,
		18.0f, 28.0f, 0.0f,
		19.0f, 30.2f, 0.0f,
		20.0f, 32.2f, 0.0f,
		21.0f, 34.0f, 0.0f,
	};

	unsigned int vec[63];
	for (unsigned int i = 0, k=0; i < 63; i+=3, k++) {
		vec[i] = k;
		vec[i + 1] = k + 1;
		vec[i + 2] = k;
	}
	
	for (int item = 0; item < sizeof(graph) / sizeof(float); item++) {
		graph[item] = scale_number(graph[item], 0.0, 60.0, -1.0, 1.0);
	}

	unsigned int VBOLine, VAOLine, EBOLine;
	glGenBuffers(1, &VBOLine);
	glGenBuffers(1, &EBOLine);
	glGenVertexArrays(1, &VAOLine);

	glBindVertexArray(VAOLine);
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferData(GL_ARRAY_BUFFER, sizeof(graph), graph, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLine);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vec), &vec, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// uniform color lolcation
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");


	// color combination


	// uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		
		// setting up dynamic color for fragment shader
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		/*
		// draw our first triangle
		glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		*/

		//// bind element buffer
		//glBindVertexArray(VAO1);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		//triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//triangle

		// setting up dynamic color for fragment shader
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform4f(vertexColorLocation, redValue, 0.0, 0.0f, 1.0f);


		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		//line
		/*glBindVertexArray(VAOLine);
		glDrawArrays(GL_TRIANGLES, 0, 66);
		glBindVertexArray(0);*/
		// element line
		/*glBindVertexArray(VAOLine);
		glDrawElements(GL_TRIANGLES, 63, GL_UNSIGNED_INT, 0);*/

		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/release, mouse moved, etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they-ve outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


float scale_number(float x, float oMin, float oMax, float nMin, float nMax)
{
	//range check
	if (oMin == oMax) {
		//std::cout<< "Warning: Zero input range";
		return -1;
	}

	if (nMin == nMax) {
		//std::cout<<"Warning: Zero output range";
		return -1;
	}

	//check reversed input range
	bool reverseInput = false;
	float oldMin = std::min(oMin, oMax);
	float oldMax = std::max(oMin, oMax);
	if (oldMin == oMin)
		reverseInput = true;

	//check reversed output range
	bool reverseOutput = false;
	float newMin = std::min(nMin, nMax);
	float newMax = std::max(nMin, nMax);
	if (newMin == nMin)
		reverseOutput = true;

	float portion = (x - oldMin) * (newMax - newMin) / (oldMax - oldMin);
	if (reverseInput)
		portion = (oldMax - x) * (newMax - newMin) / (oldMax - oldMin);

	float result = portion + newMin;
	if (reverseOutput)
		result = newMax - portion;

	return result;
}
	