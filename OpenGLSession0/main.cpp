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



struct Point {
	float x, y, z;
	float r, g, b;
};
void CreateCoordinateSystem(std::vector<Vertex>& vertices,float start, float iterations) {
	// X-axis
	for (int i = start; i < iterations; ++i) {
		float n = 0.05f;
		float x = i * n;
		float y = 0.0;
		float z = 0.0f;
		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		vertex.r = 1.0f;
		vertex.g = 0.0f;
		vertex.b = 0.0f;  // Adjust for coloring effect
		vertices.push_back(vertex);

		
	}

	// Y-axis
	for (int i = start; i < iterations; ++i) {
		float n = 0.05f;
		float x = 0.0;
		float y = i * n;
		float z = 0.0f;
		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		vertex.r = 0.0f;
		vertex.g = 1.0f;
		vertex.b = 0.0f;  // Adjust for coloring effect
		vertices.push_back(vertex);

		
	}
	// Z - Axis
	for (int i = start; i < iterations; ++i) {
		float n = 0.05f;
		float x = 0.0;
		float y = 0.0f;
		float z = i * n;
		Vertex vertex;
		vertex.x = x;
		vertex.y = y;
		vertex.z = z;
		vertex.r = 0.0f;
		vertex.g = 0.0f;
		vertex.b = 1.0f;  // Adjust for coloring effect
		vertices.push_back(vertex);


	}
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

	glm::vec4 x =  inverseA * y;
	return x;
}
glm::vec3 LeastSquareMethod() {

	glm::mat3x3 ATA(271, 3, 3,   
		          -5, 268, 48,  
		           -1, 48, 8);
	
	//compute A^T * y
	glm::vec3 ATy(-5,368,48); // default -5,368,48

	//compute B^-1 or (A * A^T)^-1
	glm::mat3 ATAInverse = glm::inverse(ATA);

	// compute x
	glm::vec3 x = ATAInverse * ATy;
	
	std::cout << " x: " << x.x << " y: " << x.y << " z: " << x.z << std::endl;
	return x;

}
glm::vec4 CubicInterpolation(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {
	glm::mat<4, 4, float> A(
		powf(x0, 3), powf(x0, 2), x0, 1,
		powf(x1, 3), powf(x1, 2), x1, 1,
		powf(x2, 3), powf(x2, 2), x2, 1,
		powf(x3, 3), powf(x3, 2), x3, 1
	);

	glm::vec4 y(y0, y1, y2, y3);

	glm::mat4 inverseA = glm::inverse(A);

	glm::vec4 x = inverseA * y;
	return x;
}
void CreateGraphFromFunction(std::vector<Vertex>& verticesgraph,float c , int iterations, const char* filename, int start) {
	glm::vec3 leastSquare = LeastSquareMethod();
	for (int i = start; i < iterations; ++i) {
		float t = static_cast<float>(i);
		float n = 0.05f;
		float x = i * n;
		float y = ((leastSquare.x * 100) * x * x) + (leastSquare.y) *  x + 1 * leastSquare.z / 100;	

		float z = 0.0f;
	
		float df = x;
		

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
		//writeToFile(filename, vertex.x, vertex.y, vertex.z,vertex.r, vertex.g, vertex.b);

	}

	


}
void CreateGraphFromPoints(std::vector<Vertex>& verticesgraph, float c, int iterations, const char* filename, int start) {
	glm::vec4 CubicFunc = CubicInterpolation(-2,-3,1,-1,3,3,4,8);
	for (int i = start; i < iterations; ++i) {
		float t = static_cast<float>(i);
		float n = 0.05f;
		float x = i * n;
		float y = ((CubicFunc.x * 100) * x * x * x) + (CubicFunc.y) * x * x + 1 * CubicFunc.z * x + CubicFunc.w;

		float z = 0.0f;

		float df = x;


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
		//writeToFile(filename, vertex.x, vertex.y, vertex.z,vertex.r, vertex.g, vertex.b);

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
	GLFWwindow* window = glfwCreateWindow(width, height, "Compulsory 2 math", NULL, NULL);
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
	
	
	const char* outputFileLeastSquareData = "LeastSqureVardata.txt";
	const char* outputFileCubicData = "CubicVardata.txt";

	
	glViewport(0, 0, width, height);

	std::vector<Vertex> verticesGraph;
	std::vector<Vertex> verticesCoordinate;
	//Readfile("grahTwoVardata.txt", verticesGraph);
	float a = 0.1f;  // Adjust these parameters accordingly
	float b = 0.1f;
	float c = 0.1f;
	float angularFrequency = 5.1f;
	int iterations = 1200;
	int start = -1200;

	const int numRows = 3;
	const int numCols = 4;
	//draw the first 4 points
	glm::mat<3, 4, float> A(
		-2, -5, -7, -8,   // X
		2, 4, 7, 11,   // Y
		0, 0, 0, 0    // Z
	);

	glm::mat<3, 4, float> B(
		2, 4, 6, 9,  // X
		2, 5, 7, 10, // Y
		0, 0, 0, 0  // Z
		
	);

	std::vector<Point> points;
	for (int i = 0; i < numCols; ++i) {
		Point vert;
		vert.x = (A[0][i]);
		vert.y = A[1][i];
		vert.z = A[2][i];
		vert.b = 1.0f;
		points.push_back(vert);
	}

	for (int i = 0; i < numCols; ++i) {
		Point vert;
		vert.x = (B[0][i]);
		vert.y = B[1][i];
		vert.z = B[2][i];
		vert.b = 1.0f;
		cout << " x = " << (B[0][i]) << endl;
		cout << " y = " << B[1][i] << endl;
		cout << " z = " << B[2][i] << endl;
		points.push_back(vert);
	}
	
	
	
	

	
	//CreateGraphFromPoints(verticesGraph, c, 8, outputFileLeastSquareData, -2);
	CreateGraphFromFunction(verticesGraph, c, iterations, outputFileCubicData,start);
	CreateCoordinateSystem(verticesCoordinate,start,iterations);
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
	

	
	

	
	

	// Generates Vertex Buffer Object and links it to graph vertices
	VBO VBO_Spiral(flattenedVertices.data(), flattenedVertices.size() * sizeof(GLfloat));
	VAO1.LinkAttrib(VBO_Spiral, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO_Spiral, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO VAO2;
	std::vector<GLfloat> flattenedPoints;
	for (const Point& point : points) {
		flattenedPoints.push_back(point.x);
		flattenedPoints.push_back(point.y);
		flattenedPoints.push_back(point.z);
		flattenedPoints.push_back(point.r);
		flattenedPoints.push_back(point.g);
		flattenedPoints.push_back(point.b);
		// Add color components 
	}
	VAO2.Bind();
	VBO VBO_Point(flattenedPoints.data(), flattenedPoints.size() * sizeof(GLfloat));
	VAO2.LinkAttrib(VBO_Point, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO2.LinkAttrib(VBO_Point, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	VAO VAO3;
	VAO3.Bind();
	std::vector<GLfloat> CoordinatePoints;
	for (const Vertex& vertex : verticesCoordinate) {
		CoordinatePoints.push_back(vertex.x);
		CoordinatePoints.push_back(vertex.y);
		CoordinatePoints.push_back(vertex.z);
		CoordinatePoints.push_back(vertex.r);
		CoordinatePoints.push_back(vertex.g);
		CoordinatePoints.push_back(vertex.b);
	}
	VBO VBO_CoordinateSys(CoordinatePoints.data(), CoordinatePoints.size() * sizeof(GLfloat));
	VAO3.LinkAttrib(VBO_CoordinateSys, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO3.LinkAttrib(VBO_CoordinateSys, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	
	VAO1.Unbind();
	VBO_Spiral.Unbind();
	VAO2.Unbind();
	VBO_Point.Unbind();
	VAO3.Unbind();
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	float scaleValue = 25.0f;
	
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
		camera.Matrix(45.0f, 0.1f, 500.0f, shaderProgram, "camMatrix");
		// Draw the points
		glUniform1f(uniID, scaleValue);

		VAO1.Bind();
		glDrawArrays(GL_LINE_STRIP, 0, verticesGraph.size());
		glLineWidth(5.0f);
		VAO1.Unbind();


		VAO2.Bind();         
		glDrawArrays(GL_POINTS, 0, points.size());
		glPointSize(25.0f);// Set point size here
		VAO2.Unbind();

		VAO3.Bind();
		glDrawArrays(GL_LINES, 0, verticesCoordinate.size());
		glLineWidth(5.0f);
		VAO3.Unbind();
		 //Bind the VAO so OpenGL knows to use it
		

		

		
		

		

		// Unbind VAO to prevent accidentally modifying it
		
		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	
	/*std::ofstream clearFile(outputFileCubicData);
		clearFile.close();*/
	
	
	// Delete all the objects we've created
	VAO1.Delete();
	VAO2.Delete();
	VAO3.Delete();
	VBO_Point.Delete();
	VBO_Spiral.Delete();
	VBO_CoordinateSys.Delete();
	
	shaderProgram.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}