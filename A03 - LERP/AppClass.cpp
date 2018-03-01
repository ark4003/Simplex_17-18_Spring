#include "AppClass.h"

/*TODO:  
List of Orbits Vector of Vector 3s
List of variables for Stops Vecotor of Unsigned Ints
*/
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Andrew Knowland - ark4003@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 0.90F; //initial size of orbits


	

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	
																						   
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides

	//collect the groups of stops (declared in header, putting here commented out to improve code readability)
	//std::vector <std::vector<vector3>> stopsList;
	
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager

		//collects all the points in a given orbit
		std::vector <vector3> stopPoints;
		

		//used for finding position of each point
		float theta = 0; 


		//j is the current point.  because i is the number of sides, and the number of sides equals the number of points, j will go up to the number of points in the current shape
		for (int j = 0; j < i; j++) 
		{

			//angle of Theta.  Equation modified from code from my 3D Shapes assignment (AO2)
			theta = ((j* (2 * PI))/i);


			//modified code taken from my 3D Shapes assignment (AO2).  Take all the stops and put them in a StopPoints.  
			stopPoints.push_back(vector3( (cos(theta)*fSize),  (sin(theta)*fSize),  0) );

		}

		//take this set of points and put it in the stopsList
		stopsList.push_back(stopPoints);

		
		fSize += 0.4f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);



	//Get a timer (Taken from code I used for LERP Exercise, E06)
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer




	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos;

		//Calculations for what point I'm on and how far I am on it taken from my LERP Exercise submission (E06) and modified to fit this code
		//which point you are on.  Increases every time the timer increments by 1, returns to 0 every time the timer exceeds the number of points.
		int currentLocation = fmod(fTimer, stopsList[i].size());

		//your current destination.  if your destination would be greater than your current number of points, return it to zero so that you return to the start. 
		int nextLocation = currentLocation + 1;
		if (currentLocation == (stopsList[i].size() - 1)) {
			nextLocation = 0;
		}

		//how far you've traveled from point to point.  goes from 0 to 1 over the course of a second (meaning each time you get to a new point), returning to 0 every second.
		float travelPercentage = fmod(fTimer, 1.0f);

		//move from current location to next location, once per second
		v3CurrentPos = glm::lerp(stopsList[i][currentLocation], stopsList[i][nextLocation], travelPercentage);

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}