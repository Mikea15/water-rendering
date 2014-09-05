#include "include/main.h"

#define SPEED 50.0f

void CalculateFrameRate()
{
	static int fps = 0;
	static float fpsTime = 0.0f;
	char frameRate[50] = { 0 };

	fpsTime += g_DeltaTime;

	if (fpsTime > 1.0f)
	{
		fpsTime = 0.0f;
		sprintf_s(frameRate, "Current FPS: %d", fps);
		SetWindowText(g_WindowHandle, frameRate);
		fps = 0;
	}
	else
	{
		++fps;
	}
}

GCamera::GCamera()
{
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_View = glm::vec3(0.0f, 1.0f, 0.5f);
	m_Up = glm::vec3(0.0f, 0.0f, 1.0f);
}

void GCamera::PositionCamera(glm::vec3 position, glm::vec3 view, glm::vec3 up)
{
	m_Position = position;
	m_View = view;
	m_Up = up;
}

void GCamera::SetViewByMouse()
{
	POINT pos;
	int middleX = SCREEN_WIDTH >> 1;
	int middleY = SCREEN_HEIGHT >> 1;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	static float currentXRot = 0.0f;

	GetCursorPos(&pos);

	if (pos.x == middleX && pos.y == middleY)
		return;

	SetCursorPos(middleX, middleY);

	angleY = (float)(middleX - pos.x) / 500.0f;
	angleZ = (float)(middleY - pos.y) / 500.0f;

	static float lastXRot = 0.0f;
	lastXRot = currentXRot;

	currentXRot += angleZ;

	if (currentXRot > 1.0f)
	{
		// Clamp to 1
		currentXRot = 1.0f;

		// If we haven't completed the rotation, finish it
		if (lastXRot != 1.0f)
		{
			glm::vec3 axis = glm::cross(m_View - m_Position, m_Up);
			axis = glm::normalize(axis);
			RotateView(1.0f - lastXRot, axis);
		}
	}
	else if (currentXRot < -1.0f)
	{
		currentXRot = -1.0f;
		if (lastXRot != -1.0f)
		{
			glm::vec3 axis = glm::cross(m_View - m_Position, m_Up);
			axis = glm::normalize(axis);
			RotateView(-1.0f - lastXRot, axis);
		}
	}
	else
	{
		glm::vec3 axis = glm::cross(m_View - m_Position, m_Up);
		axis - glm::normalize(axis);
		RotateView(angleZ, axis);
	}

	RotateView(angleY, glm::vec3(0, 1, 0));
}

void GCamera::RotateView(float theta, glm::vec3 axis)
{
	glm::vec3 rotatedView;
	glm::vec3 view = m_View - m_Position;

	// Calculate sine and cosine of angle
	float cosTheta = (float)cos(theta);
	float sinTheta = (float)sin(theta);

	// Quicker access to vector components
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	// Compute the new x position rotated point
	rotatedView.x =  (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	rotatedView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	rotatedView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	// Compute the new y position rotated point
	rotatedView.y =	 ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	rotatedView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	rotatedView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	// Compute the new z position rotated point
	rotatedView.z =  ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	rotatedView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	rotatedView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	m_View = m_Position + rotatedView;
}

void GCamera::StrafeCamera(float speed)
{
	m_Position.x += m_Strafe.x * speed;
	m_Position.y += m_Strafe.x * speed;

	m_View.x += m_Strafe.x * speed;
	m_View.y += m_Strafe.z * speed;
}

void GCamera::MoveCamera(float speed)
{
	glm::vec3 view = m_View - m_Position;
	view = glm::normalize(view);

	m_Position.x += view.x * speed;
	m_Position.y += view.y * speed;
	m_Position.z += view.z * speed;

	m_View.x += view.x * speed;
	m_View.y += view.y * speed;
	m_View.z += view.z * speed;
}

void GCamera::CheckForMovement()
{
	float speed = SPEED * g_DeltaTime;

	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
	{
		MoveCamera(speed);
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
	{
		MoveCamera(-speed);
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)
	{
		StrafeCamera(speed);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
	{
		MoveCamera(-speed);
	}
}

void GCamera::Update()
{
	glm::vec3 strafe = glm::cross(m_View - m_Position, m_Up);
	m_Strafe = glm::normalize(strafe);

	SetViewByMouse();

	CheckForMovement();

	CalculateFrameRate();
}

void GCamera::Look()
{
	gluLookAt(
		m_Position.x, m_Position.y, m_Position.z,
			m_View.x,	  m_View.y,		m_View.z,
			  m_Up.x,		m_Up.y,		  m_Up.z);
}