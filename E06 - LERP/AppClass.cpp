#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Andrew Knowland - ark4003@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	/*start at m_stopsList_0 and then lerp at a rate of one point per second.  Every second, increase the place in m_stopsList you're aiming at by one.
	HINT: Modulus is our friend, use Modulus to basically loop the amount of time you're working on.  There are 11 points.
	*/

	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));

	//wanted to make sure my code can be used flexibly.  It can.  Uncomment this line to prove it is.
	//m_stopsList.push_back(vector3(0.0f, 0.0f, 0.0f));

	

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

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;
	




	//your code goes here

	//which point you are on.  Increases every time the timer increments by 1, returns to 0 every time the timer exceeds the number of points.
	int currentLocation = fmod(fTimer, m_stopsList.size());

	//your current destination.  if your destination would be greater than your current number of points, return it to zero so that you return to the start. 
	int nextLocation = currentLocation + 1;
	if (currentLocation == (m_stopsList.size() - 1)) {
		nextLocation = 0;
	}

	//how far you've traveled from point to point.  goes from 0 to 1 over the course of a second (meaning each time you get to a new point), returning to 0 every second.
	float travelPercentage = fmod(fTimer, 1.0f);

	//move from current location to next location, once per second
	v3CurrentPos = glm::lerp(m_stopsList[currentLocation], m_stopsList[nextLocation], travelPercentage);
	//-------------------
	


	
	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("Timer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	m_pMeshMngr->Print("Current Position: ");
	m_pMeshMngr->PrintLine(std::to_string(currentLocation), C_BLUE);

	m_pMeshMngr->Print("Travel Percentage: ");
	m_pMeshMngr->PrintLine(std::to_string(travelPercentage), C_GREEN);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
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
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}