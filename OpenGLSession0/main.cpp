#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resources/Shaders/shaderClass.h"
#include "Resources/Shaders/VAO.h"
#include "Resources/Shaders/VBO.h"
#include "Resources/Shaders/EBO.h"
#include "Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;
using namespace std;
struct Vertex {
	float x, y, z;
	float r, g, b;
	float pr, pg, pb;
};
class Cube {
public:
	vector<Vertex> mVertecies;
	glm::mat4 matrix;
	std::string name;

	Cube(float scale) : a(scale) {
		Vertex v0{ -a, -a, a , 1.0f, 0.0f, 0.0f };
		Vertex v1{ a, -a, a , 0.0f, 1.0f, 0.0f };
		Vertex v2{ a, a, a , 0.0f, 0.0f, 1.0f };
		Vertex v3{ -a, a, a , 0.0f, 1.0f, 0.0f };
		Vertex v4{ -a, -a, -a , 1.0f, 1.0f, 0.0f };
		Vertex v5{ a, -a, -a , 0.0f, 1.0f, 0.0f };
		Vertex v6{ a, a, -a , 0.0f, 1.0f, 1.0f };
		Vertex v7{ -a, a, -a , 0.0f, 1.0f, 0.0f };


		// Front face

		mVertecies.push_back(v0);
		mVertecies.push_back(v1);
		mVertecies.push_back(v3);
		mVertecies.push_back(v3);
		mVertecies.push_back(v1);
		mVertecies.push_back(v2);



		// Back face
		mVertecies.push_back(v4);
		mVertecies.push_back(v5);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v5);
		mVertecies.push_back(v6);

		// Right face
		mVertecies.push_back(v1);
		mVertecies.push_back(v5);
		mVertecies.push_back(v2);
		mVertecies.push_back(v2);
		mVertecies.push_back(v5);
		mVertecies.push_back(v6);

		// Left face
		mVertecies.push_back(v4);
		mVertecies.push_back(v0);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v0);
		mVertecies.push_back(v3);

		// Top face
		mVertecies.push_back(v3);
		mVertecies.push_back(v2);
		mVertecies.push_back(v7);
		mVertecies.push_back(v7);
		mVertecies.push_back(v2);
		mVertecies.push_back(v6);

		// Bottom face
		mVertecies.push_back(v4);
		mVertecies.push_back(v5);
		mVertecies.push_back(v0);
		mVertecies.push_back(v0);
		mVertecies.push_back(v5);
		mVertecies.push_back(v1);

		
		
		flattenVertices();
	}
	std::vector<GLfloat> getFlattenedVertices() const {
		std::vector<GLfloat> flattenedVertices;
		for (const Vertex& vertex : mVertecies) {
			flattenedVertices.push_back(vertex.x);
			flattenedVertices.push_back(vertex.y);
			flattenedVertices.push_back(vertex.z);
			flattenedVertices.push_back(vertex.r);
			flattenedVertices.push_back(vertex.g);
			flattenedVertices.push_back(vertex.b);
		}
		return flattenedVertices;
	}

private:
	void flattenVertices() {
		std::vector<GLfloat> flattenedVertices;
		for (const Vertex& vertex : mVertecies) {
			flattenedVertices.push_back(vertex.x);
			flattenedVertices.push_back(vertex.y);
			flattenedVertices.push_back(vertex.z);
			flattenedVertices.push_back(vertex.r);
			flattenedVertices.push_back(vertex.g);
			flattenedVertices.push_back(vertex.b);
		}
	}
	float a{ 1.0f };
};


struct Point {
	float x, y, z;

};
void CreateCoordinateSystem(std::vector<Vertex>& vertices) {
	// X-axis
	vertices.push_back({ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });
	vertices.push_back({ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f });

	// Y-axis
	vertices.push_back({ 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f });
	vertices.push_back({ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f });

	// Z-axis
	vertices.push_back({ 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f });
	vertices.push_back({ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
}
void writeToFile(const char* fileName, double x, double y, double z, double r, double g, double b) {
	std::ofstream outputFile(fileName, std::ios::app);  // Open the file in append mode
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(3);
		outputFile << x << " , " << y << " , " << z << " , " << r << " , " << g << " , " << b;


	}
	else {
		std::cerr << "Unable to open the output file for writing." << std::endl;
	}
	//if (y > 0) {
	//	outputFile << " green";  // Use green for positive y
	//}
	//else if (y < 0) {
	//	outputFile << " red";    // Use red for negative y
	//}
	//else {
	//	outputFile << " white";  // Use white for y = 0
	//}
	outputFile << std::endl;
	outputFile.close();
}
void Readfile(const char* fileName, std::vector<Vertex>& verticesSpiral) {
	std::ifstream inputFile(fileName);
	if (inputFile.is_open()) {


		std::string line;
		std::getline(inputFile, line);
		Vertex vertex;
		char comma; // to capture the commas in the file

		while (inputFile >> vertex.x >> comma >> vertex.y >> comma >> vertex.z >> comma 
			>> vertex.r >> comma >> vertex.g >> comma >> vertex.b) {
			verticesSpiral.push_back(vertex);
		}

		inputFile.close();
	}
	else {
		std::cerr << "Unable to open the input file for reading." << std::endl;
	} 
	
	
	
}
glm::vec4 CubicHermiteFunction(float inx0, float inx1, float iny0,float iny1, float derivativeY0, float derivativeY1) {
	float x0, x1, y0, y1;
	x0 = inx0;
	x1 = inx1;
	y0 = iny0;
	y1 = iny1;

	float derY0, derY1;
	derY0 = derivativeY0;
	derY1 = derivativeY1;

	float func = powf(1, 1);
	glm::mat<4, 4, float> A
	    (powf(x0, 3), powf(x0, 2), x0, 1,
		powf(x1, 3), powf(x1, 2), x1, 1,
		powf(x0, 2), x0, 1, 0,
		powf(x0, 2), x0, 1, 1);
	
	glm::vec4 y(y0, y1,derivativeY0,derivativeY1);

	glm::mat4 inverseA = glm::inverse(A);

	glm::vec4 x = y * inverseA;
	return x;
}
glm::vec3 LeastSquareMethod() {
#define G 5
	glm::mat3x3 ATA(279, -5, -1,   // First column
		-5, 368, 48,  // Second column
		-1, 48, 8);
	              
	
	
	
	
	//compute A^T
	
 // amount of points

	//compute A^T * y
	glm::vec3 ATy(-5,368,48);

	//compute B^-1 or (A * A^T)^-1
	glm::mat3 ATAInverse = glm::inverse(ATA);

	
	glm::vec3 x = ATAInverse * ATy;
	/*std::cout << "ATy: " << ATy.x << ", " << ATy.y << ", " << ATy.z << std::endl;
    std::cout << "ATAInverse: " << ATAInverse[0][0] << ", " << ATAInverse[0][1] << ", " << ATAInverse[0][2] << std::endl;
    std::cout << "            " << ATAInverse[1][0] << ", " << ATAInverse[1][1] << ", " << ATAInverse[1][2] << std::endl;
    std::cout << "            " << ATAInverse[2][0] << ", " << ATAInverse[2][1] << ", " << ATAInverse[2][2] << std::endl;*/

    // compute x
	
	std::cout << " x: " << x.x << " y: " << x.y << " z: " << x.z << std::endl;
	return x;

}
void CreateGraphFromFunction(std::vector<Vertex>& verticesgraph,float c , int iterations, const char* filename, int start) {
	for (int i = start; i <= iterations; ++i) {
		float t = static_cast<float>(i);
		float n = 0.05f;
		float x = i * n;
		float y = LeastSquareMethod().x * x * x + LeastSquareMethod().y * x + LeastSquareMethod().z;
		float z = 0.0f;
	
		float df = 2*x;
		

		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		if (df < 0) {
			vertex.r = 1.0f;
			vertex.g = 0.0f;
		}
		else {
			vertex.r = 0.0f;
			vertex.g = 1.0f;
		}
		
		vertex.b = std::abs(z) / (c * iterations);  // Adjust for coloring effect
		/*vertex.pr = 0.0f;
		vertex.pg = 0.0f;
		vertex.pb = 1.0f;*/
		verticesgraph.push_back(vertex);
		writeToFile(filename, vertex.x, vertex.y, vertex.z,vertex.r, vertex.g, vertex.b);

	}

	

}
void FunctionWithTwoVariables(std::vector<Vertex>& verticesgraph, int iterations, const char* filename) {

	for (int i = -50; i <= iterations; ++i) {
		
		
		float n = 0.01f;
		float x = i * n;
		for (int j = -50; j <= iterations; ++j) {
			float y = j * n;
			//float z = sin(cos(tan(x))) * sin(cos(tan(y)));
			float z = x * y;


			Vertex vertex;
			vertex.x = x;
			vertex.y = y;
			vertex.z = z;
			vertex.r = 0.0f;
			vertex.g = 0.1 * abs(x);
			vertex.b = 0.1 * abs(y);

			verticesgraph.push_back(vertex);
			//writeToFile(filename, vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b);

		}
	}
}
int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	
	
	const char* outputFileGraphTwoVar = "grahTwoVardata.txt";
	

	
	glViewport(0, 0, width, height);

	std::vector<Vertex> verticesGraph;
	//Readfile("grahTwoVardata.txt", verticesGraph);
	float a = 0.1f;  // Adjust these parameters accordingly
	float b = 0.1f;
	float c = 0.1f;
	float angularFrequency = 5.1f;
	int iterations = 200;
	int start = -200;

	const int numRows = 3;
	const int numCols = 4;

	glm::mat<3, 4, float> A(-2, 2, 1,   
		-5, 4, 1,  
		-7, 7, 1,
		-8, 11, 1
	    );  

	glm::mat<3, 4, float> B(4, 2, 1,
		-2, -1, 1,
		1, 1, 1,
		0, 0, 0
	);

	std::vector<glm::vec3> points;
	for (int i = 0; i < numCols; ++i) {
		glm::vec3 point = glm::vec3(A[0][i], A[1][i], A[2][i]);
		points.push_back(point);
	}
	std::vector<GLfloat> flattenedPoints;
	for (const glm::vec3& point : points) {
		flattenedPoints.push_back(point.x);
		flattenedPoints.push_back(point.y);
		flattenedPoints.push_back(point.z);
		// Add color components if needed
	}
	
	

	
	//FunctionWithTwoVariables(verticesGraph, iterations, outputFileGraphTwoVar);
	CreateGraphFromFunction(verticesGraph, c, iterations, outputFileGraphTwoVar, start);
	//CreateCoordinateSystem(coordinateSystemVertices);
	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	
	
	
	

	// Flatten the vector of Vertex into GLfloat
	

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	std::vector<GLfloat> flattenedVertices;
	for (const Vertex& vertex : verticesGraph) {
		flattenedVertices.push_back(vertex.x);
		flattenedVertices.push_back(vertex.y);
		flattenedVertices.push_back(vertex.z);
		flattenedVertices.push_back(vertex.r);
		flattenedVertices.push_back(vertex.g);
		flattenedVertices.push_back(vertex.b);
	}
	

	// Unbind to prevent accidentally modifying it
	

	
	

	// Generates Vertex Buffer Object and links it to spiral vertices
	VBO VBO_Spiral(flattenedVertices.data(), flattenedVertices.size() * sizeof(GLfloat));
	VAO1.LinkAttrib(VBO_Spiral, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO_Spiral, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO VAO2;
	VAO2.Bind();
	VBO VBO_Point(flattenedPoints.data(), flattenedPoints.size() * sizeof(glm::vec3));
	VAO2.LinkAttrib(VBO_Point, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO_Point, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	
	
	
	// Unbind all to prevent accidentally modifying them
	
	VAO1.Unbind();
	VBO_Spiral.Unbind();
	VAO2.Unbind();
	VBO_Point.Unbind();
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 100.0f;
	
	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


		glUniform1f(uniID, scaleValue);
		 //Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		glDrawArrays(GL_LINE_STRIP, 0, verticesGraph.size());
		glLineWidth(5.0f);
		VAO1.Unbind();

		

		// Draw the points
		//VAO2.Bind();
		//glPointSize(25.0f);  // Set point size here
		//glDrawArrays(GL_POINTS, 0, points.size());
		//VAO2.Unbind();
		//

		

		// Unbind VAO to prevent accidentally modifying it
		
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	
	
	
	
	// Delete all the objects we've created
	VAO1.Delete();
	VAO2.Delete();
	VBO_Point.Delete();
	VBO_Spiral.Delete();
	
	shaderProgram.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}