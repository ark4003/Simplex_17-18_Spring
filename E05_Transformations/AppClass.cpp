#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	//Make a vector of cubes, loop through to scale them to correct size, translate them all to proper positions (maybe make a method for that?) loop through to make them move in update
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCube(1.0f, C_BLACK);

	//LIST OF BLOCK POSITIONS (or rather, translations to those positions), done from top to bottom left to right
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 8.0f, 3.0f)));//0 BOTTOM LEFT FIRST ROW
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 8.0f, 3.0f)));//1 
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-3.0f, 7.0f, 3.0f)));//2 BOTTOM LEFT ROW 2
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(1.0f, 7.0f, 3.0f)));//3 
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 6.0f, 3.0f)));//4 BOTTOM LEFT ROW 3
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-3.0f, 6.0f, 3.0f)));//5
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-2.0f, 6.0f, 3.0f)));//6
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-1.0f, 6.0f, 3.0f)));//7
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(0.0f, 6.0f, 3.0f)));//8
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(1.0f, 6.0f, 3.0f)));//9
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 6.0f, 3.0f)));//10
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-5.0f, 5.0f, 3.0f)));//BOTTOM LEFT ROW 4
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-2.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-1.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-0.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(3.0f, 5.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-6.0f, 4.0f, 3.0f)));//BOTTOM LEFT ROW 5
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-5.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-3.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-2.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-1.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(0.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(1.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(3.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(4.0f, 4.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-6.0f, 3.0f, 3.0f)));//BOTTOM LEFT ROW 6
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-3.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-2.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-1.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(0.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(1.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(4.0f, 3.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-6.0f, 2.0f, 3.0f)));//BOTTOM LEFT ROW 7
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-4.0f, 2.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(2.0f, 2.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(4.0f, 2.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-3.0f, 1.0f, 3.0f)));//BOTTOM LEFT ROW 8
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(-2.0f, 1.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(0.0f, 1.0f, 3.0f)));//
	translateList.push_back(glm::translate(IDENTITY_M4, vector3(1.0f, 1.0f, 3.0f)));//

	
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(0.5f,0.5f,0.5f));
	static float value = 0.0f;

	
	//translates to the right
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 0.0f , 0.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	
	//translate to the desired position, then the desired number of units right, then draw, repeat for each position
	for (int i = 0; i < translateList.size(); i++) {
		matrix4 m4Model = (m4Scale  * translateList[i]) * m4Translate;

		m_pMesh->Render(m4Projection, m4View, m4Model);

	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}