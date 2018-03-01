#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCone(2.0f, 5.0f, 3, C_WHITE);
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

	//automated gimbol lock test.  Unrelated to metnod I ended up using (also, unrelated to the code that was left running at the start.  Leaving this here for me to refer to later.
	/*
	m_m4Model = glm::rotate(IDENTITY_M4, m_v3Rotation.x, vector3(1.0f, 0.0f, 0.0f));
	m_m4Model = glm::rotate(m_m4Model, m_v3Rotation.y, vector3(0.0f, 1.0f, 0.0f));
	m_m4Model = glm::rotate(m_m4Model, m_v3Rotation.z, vector3(0.0f, 0.0f, 1.0f));
	*/

	//nonfunctional original automatic quaternion rotation.  Warps the shape, kept for reference when using this exercise as notes.
	//m_qOrientation = m_qOrientation * glm::angleAxis(1.0f, vector3(1.0f));


	/*//angleAxis has the magnitudes of rotation across x y and z, combining to give what axes are being rotated across with quaternions*/
	m_qOrientation = m_qOrientation * glm::angleAxis(rotationAxis.x, vector3(1.0f, 0, 0));
	m_qOrientation = m_qOrientation * glm::angleAxis(rotationAxis.y, vector3(0, 1.0f, 0));
	m_qOrientation = m_qOrientation * glm::angleAxis(rotationAxis.z, vector3(0, 0, 1.0f));
	
	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));
	
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