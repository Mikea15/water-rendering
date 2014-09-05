#ifndef _GCAMERA_H
#define _GCAMERA_H

#include "main.h"

class GCamera
{
public:
	GCamera();

	//************************************
	// Method:    PositionCamera
	// FullName:  GCamera::PositionCamera
	// Access:    public 
	// Returns:   void
	// Qualifier: Changes the position, view, and up vector for the camera
	// Parameter: glm::vec3 position
	// Parameter: glm::vec3 view
	// Parameter: glm::vec3 up
	//************************************
	void PositionCamera(glm::vec3 position, glm::vec3 view, glm::vec3 up);

	//************************************
	// Method:    RotateView
	// FullName:  GCamera::RotateView
	// Access:    public 
	// Returns:   void
	// Qualifier: Rotates the camera's view around the axis by theta
	// Parameter: float theta
	// Parameter: glm::vec3 axis
	//************************************
	void RotateView(float theta, glm::vec3 axis);

	//************************************
	// Method:    SetViewByMouse
	// FullName:  GCamera::SetViewByMouse
	// Access:    public 
	// Returns:   void
	// Qualifier: Moves the camera's view based on mouse movements (first-person view)
	//************************************
	void SetViewByMouse();

	//************************************
	// Method:    StrafeCamera
	// FullName:  GCamera::StrafeCamera
	// Access:    public 
	// Returns:   void
	// Qualifier: Strafes the camera left (-) or right (+) based on the speed
	// Parameter: float speed
	//************************************
	void StrafeCamera(float speed);

	//************************************
	// Method:    MoveCamera
	// FullName:  GCamera::MoveCamera
	// Access:    public 
	// Returns:   void
	// Qualifier: Moves the camera left (+) or right (-) based on the speed
	// Parameter: float speed
	//************************************
	void MoveCamera(float speed);

	//************************************
	// Method:    CheckForMovement
	// FullName:  GCamera::CheckForMovement
	// Access:    public 
	// Returns:   void
	// Qualifier: Checks for keyboard input for movement
	//************************************
	void CheckForMovement();

	//************************************
	// Method:    Update
	// FullName:  GCamera::Update
	// Access:    public 
	// Returns:   void
	// Qualifier: Updates the camera's vectors
	//************************************
	void Update();

	//************************************
	// Method:    Look
	// FullName:  GCamera::Look
	// Access:    public 
	// Returns:   void
	// Qualifier: Uses gluLookAt to set the camera and sends the information to the shader
	//************************************
	void Look();

	//************************************
	// Method:    Position
	// FullName:  GCamera::Position
	// Access:    public 
	// Returns:   glm::vec3
	// Qualifier: Returns the position vector
	//************************************
	glm::vec3 Position()
	{
		return m_Position;
	}

	//************************************
	// Method:    View
	// FullName:  GCamera::View
	// Access:    public 
	// Returns:   glm::vec3
	// Qualifier: Returns the view vector
	//************************************
	glm::vec3 View()
	{
		return m_View;
	}

	//************************************
	// Method:    Up
	// FullName:  GCamera::Up
	// Access:    public 
	// Returns:   glm::vec3
	// Qualifier: Returns the up vector
	//************************************
	glm::vec3 Up()
	{
		return m_Up;
	}

	//************************************
	// Method:    Strafe
	// FullName:  GCamera::Strafe
	// Access:    public 
	// Returns:   glm::vec3
	// Qualifier: Returns the strafe vector
	//************************************
	glm::vec3 Strafe()
	{
		return m_Strafe;
	}

private:
	glm::vec3 m_Position;
	glm::vec3 m_View;
	glm::vec3 m_Up;
	glm::vec3 m_Strafe;
};

#endif