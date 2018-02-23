#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();
	float baseHeight = -a_fHeight / 2;

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);

	//const int cPointNumber = a_nSubdivisions;
	//vector3 conePoints[cPointNumber];

	std::vector <vector3> conePoints;
	vector3 tip(0.0f, a_fHeight / 2, 0.0f);//0 top
	vector3 base(0.0f, baseHeight, 0.0f);//1 bottom center

	bool firstPoint = true;
	float theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++) {
		theta = ((i* (2 * PI)) / a_nSubdivisions);
		conePoints.push_back(vector3((cos(theta)*a_fRadius), baseHeight, (sin(theta)*a_fRadius)));

		//after the first point, I can instantly make the needed triangles with the previous point, I see no reason to wait for all the points to be in the vector and loop through a second time.
		if (firstPoint == true) {
			firstPoint = false;
		}

		else if (firstPoint == false) {
			AddTri(conePoints[i - 1], conePoints[i], tip);
			AddTri(conePoints[i], conePoints[i - 1], base);
		}

		else {
			std::cout<<"something went wrong";
		}
		

	}

	//reconnect to the start
	AddTri(conePoints[a_nSubdivisions - 1], conePoints[0], tip);
	AddTri(conePoints[0], conePoints[a_nSubdivisions - 1], base);

	/*conePoints.push_back(vector3 (0.0f, a_fHeight / 2, 0.0f)); //0 top
	conePoints.push_back(vector3 (0.0f, baseHeight, 0.0f));//1 bottom center
	conePoints.push_back (vector3 (0.0f, baseHeight, a_fRadius));//2 bottom front*/



	//AddTri(conePoints[1], conePoints[2], conePoints[0]);

	/*
	//for the moment, let's assume subdivisions is 3
	vector3 tip(0.0f, a_fHeight/2, 0.0f); //0 top
	vector3 point1(a_fRadius, baseHeight, -a_fRadius); //1 bottom right
	vector3 point2(-a_fRadius, baseHeight, -a_fRadius);//2 bottom left
	vector3 point3(0.0f, baseHeight, a_fRadius);//3 bottom front
	vector3 base(0.0f, baseHeight, 0.0f);//3 bottom center


	//screw me apparently addTri is a thing.
	AddVertexPosition(point3);
	AddVertexPosition(point1);
	AddVertexPosition(tip);

	AddVertexPosition(point2);
	AddVertexPosition(point3);
	AddVertexPosition(tip);

	AddVertexPosition(point1);
	AddVertexPosition(point2);
	AddVertexPosition(tip);

	AddVertexPosition(point2);
	AddVertexPosition(point1);
	AddVertexPosition(point3);
	*/

	// -------------------------------
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code

	float topHeight = a_fHeight / 2;
	float baseHeight = -a_fHeight / 2;
	std::vector <vector3> cylTopPoints;
	std::vector <vector3> cylBasePoints;
	vector3 topCenter(0.0f, topHeight, 0.0f);//0 top
	vector3 base(0.0f, baseHeight, 0.0f);//1 bottom center

	bool firstPoint = true;
	float theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++) {
		theta = ((i* (2 * PI)) / a_nSubdivisions);
		cylTopPoints.push_back(vector3((cos(theta)*a_fRadius), topHeight, (sin(theta)*a_fRadius)));
		cylBasePoints.push_back(vector3((cos(theta)*a_fRadius), baseHeight, (sin(theta)*a_fRadius)));

		//after the first point, I can instantly make the needed triangles with the previous point, I see no reason to wait for all the points to be in the vector and loop through a second time.
		if (firstPoint == true) {
			firstPoint = false;
		}

		else if (firstPoint == false) {
			//make triangle on top and bottom
			AddTri(cylTopPoints[i], cylTopPoints[i - 1], topCenter);
			AddTri(cylBasePoints[i - 1], cylBasePoints[i], base);

			AddQuad(cylBasePoints[i - 1],  cylTopPoints [i-1], cylBasePoints[i], cylTopPoints[i]);
			
		}

		else {
			std::cout << "something went wrong";
		}

		

	}
	//reconnect to the start
	AddTri(cylTopPoints[0], cylTopPoints[a_nSubdivisions - 1], topCenter);
	AddTri(cylBasePoints[a_nSubdivisions - 1], cylBasePoints[0], base);

	AddQuad(cylBasePoints[a_nSubdivisions - 1], cylTopPoints[a_nSubdivisions - 1], cylBasePoints[0], cylTopPoints[0] );

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code

	float topHeight = a_fHeight / 2;
	float baseHeight = -a_fHeight / 2;

	//only need points in 4 rings.  In terms of coding time, it's quickest (and likely crudest) to put each of them in its own vector, as while drawing, one number in the for loop will give me 4 points.
	std::vector <vector3> tubeInnerTopPoints;
	std::vector <vector3> tubeOuterTopPoints;
	std::vector <vector3> tubeInnerBasePoints;
	std::vector <vector3> tubeOuterBasePoints;


	bool firstPoint = true;
	float theta = 0;
	for (int i = 0; i < a_nSubdivisions; i++) {
		theta = ((i* (2 * PI)) / a_nSubdivisions);
		//make rings for the inner points on the top and bottom
		tubeInnerTopPoints.push_back(vector3((cos(theta)*a_fInnerRadius), topHeight, (sin(theta)*a_fInnerRadius)));
		tubeInnerBasePoints.push_back(vector3((cos(theta)*a_fInnerRadius), baseHeight, (sin(theta)*a_fInnerRadius)));

		//make rings for the outer points on the top and bottom
		tubeOuterTopPoints.push_back(vector3((cos(theta)*a_fOuterRadius), topHeight, (sin(theta)*a_fOuterRadius)));
		tubeOuterBasePoints.push_back(vector3((cos(theta)*a_fOuterRadius), baseHeight, (sin(theta)*a_fOuterRadius)));

		//after the first point, I can instantly make the needed triangles with the previous point, I see no reason to wait for all the points to be in the vector and loop through a second time.
		if (firstPoint == true) {
			firstPoint = false;
		}

		else if (firstPoint == false) {
			//make triangle on top and bottom
			//top ring
			AddQuad(tubeOuterTopPoints[i], tubeOuterTopPoints[i-1], tubeInnerTopPoints[i], tubeInnerTopPoints[i-1]);

			//bottom ring
			AddQuad(tubeOuterBasePoints[i-1], tubeOuterBasePoints[i], tubeInnerBasePoints[i-1], tubeInnerBasePoints[i]);

			//outside
			AddQuad( tubeOuterBasePoints[i], tubeOuterBasePoints[i - 1],  tubeOuterTopPoints[i], tubeOuterTopPoints[i - 1]) ;

			//inside
			AddQuad(tubeInnerBasePoints[i - 1], tubeInnerBasePoints[i], tubeInnerTopPoints[i - 1], tubeInnerTopPoints[i]);
		}

		else {
			std::cout << "something went wrong";
		}



	}
	//reconnect top ring the start
	AddQuad(tubeInnerTopPoints[a_nSubdivisions - 1], tubeInnerTopPoints[0],  tubeOuterTopPoints[a_nSubdivisions - 1], tubeOuterTopPoints[0] );
	//reconnect bottom ring to start
	AddQuad(tubeInnerBasePoints[0], tubeInnerBasePoints[a_nSubdivisions - 1], tubeOuterBasePoints[0], tubeOuterBasePoints[a_nSubdivisions - 1]);

	//reconnect outter face to front
	AddQuad(tubeOuterBasePoints[0], tubeOuterBasePoints[a_nSubdivisions - 1],  tubeOuterTopPoints[0], tubeOuterTopPoints[a_nSubdivisions - 1]);

	//reconnect inner face to front
	AddQuad(tubeInnerBasePoints[a_nSubdivisions - 1], tubeInnerBasePoints[0], tubeInnerTopPoints[a_nSubdivisions - 1], tubeInnerTopPoints[0]);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{

	/*//Creating a vertex (sphereLongi) containing a set of vertexes on a given longitude of the sphere.  Each of those vertexes holds a vertex3 for a specific space.  
	Thus the first layer works as the longitude and the second as the lattitude of any given point.  figure out how many vertexes of how many vertex3s there are with the subdivision that's given.
	Top point and bottom point are static, so technically, the sphere will have 2 more vertical "sections" than there are subdivisions.  Allocate y value for each point based on
	number of subdivisions(+ 2 as the arcs don't need to have the first and last point along a given line) and then calculate the x y and z positions based on that height*/

	/*PROBLEMS: the height division for each j vecor3 is kind of wonky, as it would be redundant to include them (as they're defined elsewhere and I only need one of each), I'm not including the
	top and bottom points for each i(set of points across a given longitude) as they already exist.  However, my current method of dividing them leads to the top and bottom sticking out, and 
	unevenly at that.*/

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	
	//std::vector <vector3> cylTopPoints;
	//std::vector <vector3> cylBasePoints;

	//make sure to set how many vectors it will have so it doesn't become angry when it's time to reallocate.  It throws an error if I don't do this.
	std::vector <std::vector<vector3>> sphereLongi(a_nSubdivisions * a_nSubdivisions);
	vector3 topCenter(0.0f, a_fRadius, 0.0f);//0 top
	vector3 base(0.0f, - a_fRadius, 0.0f);//1 bottom center

	bool firstPoint = true;
	float theta = 0.0f;
	for (int i = 0; i < a_nSubdivisions; i++) {
		theta = ((i* (2 * PI)) / a_nSubdivisions);
		sphereLongi.push_back(std::vector<vector3>());
		
		float currentRadius = 0;
		//int arcPlace = 1;
		

		for (int j = 0; j < a_nSubdivisions; j++)
		{

			//for even height
			float heightTheta = (PI / (a_nSubdivisions + 2.0f) * (j + 1.0f));
			//float currentHeight = ( (-(a_fRadius)) *(sin(heightTheta))  + a_fRadius);

			//old, broken attempts to get even height divisions
			float currentHeight = ((a_nSubdivisions + 1.0f) * ((a_fRadius * 2.0f) / (a_nSubdivisions + 2.0f)) - (j * ((a_fRadius * 2.0f)) / (a_nSubdivisions + 2.0f)));
			//float reverseHeight = ((j + 1.0f * ((a_fRadius * 2.0f)) / (a_nSubdivisions + 2.0f)));
			//if (currentHeight <= ((a_nSubdivisions + 2.0f) * ((a_fRadius * 2.0f)/( (a_nSubdivisions + 2.0f))) / 2) ) {
				
			//}

			float currentRadius = (glm::sqrt( (currentHeight * ((a_fRadius*2) - currentHeight))) );

			
			sphereLongi[i].push_back(vector3((cos(theta)*currentRadius), (currentHeight  - a_fRadius), (sin(theta)*currentRadius)));

			
			//currentHeight = currentHeight - ((a_fRadius* 2.0f) / (a_nSubdivisions + 2.0f));
		}
		
		

		//after the first point, I can instantly make the needed triangles with the previous point, I see no reason to wait for all the points to be in the vector and loop through a second time.
		if (firstPoint == true) {
			firstPoint = false;
		}

		else if (firstPoint == false) {

			//draw squares on the sides
			bool innerFirstPoint = true;

			for (int j = 0; j < a_nSubdivisions; j++) {
				
				//I just realized that I should probably just check if j == 0 here, and same for all the firstPoint checks in the rest of the code.  I don't have time to change all of it unfortunately. 
				if (innerFirstPoint == true) {
					innerFirstPoint = false;
				}

				else if (innerFirstPoint == false && firstPoint == false) {
					//make square for this part.  essentially, i is x value, j is y value (j goes top to bottom)
					AddQuad(sphereLongi[i - 1][j - 1], sphereLongi[i][j - 1], sphereLongi[i - 1][j], sphereLongi[i][j] );

				}
				else {
					std::cout << "something went wrong on the inner check";
				}
			}
			//make triangle on top
			AddTri(sphereLongi[i][0], sphereLongi[i-1][0], topCenter);
			//make triangle on bottom
			AddTri(sphereLongi[i - 1][a_nSubdivisions - 1], sphereLongi[i][a_nSubdivisions - 1],  base);

			
			
		}

		else {
			std::cout << "something went wrong";
		}


		

		



	}

	
	//reconnect top and bottom triangles to the start
	AddTri(sphereLongi[0][0], sphereLongi[a_nSubdivisions - 1][0], topCenter);
	AddTri(sphereLongi[a_nSubdivisions - 1][a_nSubdivisions - 1], sphereLongi[0][a_nSubdivisions - 1], base);

	//reconnect faces to start

	//AddQuad(cylBasePoints[a_nSubdivisions - 1], cylTopPoints[a_nSubdivisions - 1], cylBasePoints[0], cylTopPoints[0]);
	// -------------------------------

	for (int j = 0; j < a_nSubdivisions; j++) {
		if (j != 0) {
			AddQuad(sphereLongi[0][j], sphereLongi[a_nSubdivisions - 1][j],  sphereLongi[0][j-1], sphereLongi[a_nSubdivisions - 1][j-1]);
		}
	}


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}