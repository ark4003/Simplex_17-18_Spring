#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCone(1.0f, 1.0f, 7, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCone(0.5f, 0.5f, 7,  C_WHITE);
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

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	//Field Of View (y is there just because apparently)
	float fovy = 45.0f;
	//ratio between width and height of the window.  1 makes it project the rectangular window as though it were a square, stretching it.
	float ratio = ( m_pSystem->GetWindowWidth()/ static_cast<float>(m_pSystem->GetWindowHeight()) );
	float fNear = 0.01f;
	float fFar = 1000.0f;
	m4Projection = glm::perspective(fovy, ratio, fNear, fFar);

	//measurements for screen
	float left = -10.0f;
	float right = 10.0f;
	float top = 10.0f;
	float bottom = -10.0f;
	m4Projection = glm::ortho(left, right, bottom, top, fNear, fFar);

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	vector3 v3Position (0,0, 15);
	vector3 v3Target(0, 0, 14);
	vector3 v3Up(0, 1, 0);

	m4View = glm::lookAt(v3Position, v3Target, v3Up);

	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m4View, ToMatrix4(m_qArcBall));
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m4View, glm::translate(vector3( 3.0f, 0.0f, 0.0f)));
		
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
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}