#include "camera.h"
#include "tool.h"

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, 
	float slideSpeed)
	: pos(pos), view(glm::normalize(target - pos)), worldUp(worldUp), 
	slideSpeed(slideSpeed)
{
	originPos = pos;
	fov = 45.0f;
	nearDist = 0.1f;
	farDist = 1000.0f;
	
	CalcInitYawPitch();

	//printf("%.2f %.2f\n", initialYaw, initialPitch);

	UpdateCameraVectors(initialYaw, initialPitch);
	UpdateReverseCameraVectors(initialYaw, initialPitch);

	originV = glm::lookAt(pos, pos + view, up);
	curYaw = 0;
	curPitch = 0;
	yawOffset = 0;
	pitchOffset = 0;
	slideSpeedMultiplier = 1;
}

void Camera::Update(float yaw, float pitch, float deltaTime)
{
	Slide(deltaTime);

	UpdateCameraVectors(initialYaw - yaw + yawOffset, initialPitch - pitch + pitchOffset);
	UpdateReverseCameraVectors(initialYaw + yaw - yawOffset, initialPitch + pitch - pitchOffset);

	curYaw = yaw;
	curPitch = pitch;

	O = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, nearDist, farDist);
	V = glm::lookAt(pos, pos + view, up);
	reverseV = glm::lookAt(pos, pos + reverseView, reverseUp);
	P = glm::perspective(fov, aspectRatio, nearDist, farDist);

	VP = P * V;

	UpdateFrustum();
}

void Camera::Reset()
{
	pos = originPos;
	yawOffset = curYaw;
	pitchOffset = curPitch;
}

void Camera::SetAspectRatio(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}
	aspectRatio = (float)w / (float)h;
}

void Camera::SetPosition(glm::vec3 pos)
{
	this->pos = pos;
}

void Camera::SetPickCellPosition(glm::vec3 pickCellPosition)
{
	this->pickCellPosition = pickCellPosition;
}

glm::vec3 Camera::GetPosition() const
{
	return pos;
}

glm::vec3 Camera::GetDirection() const
{
	return view;
}

void Camera::Slide(float deltaTime)//ÉãÏñ»úµÄ»¬ÐÐ
{
	if (slideDir.front)
	{
		pos += view * slideSpeed * slideSpeedMultiplier * deltaTime;
	}
	if (slideDir.back)
	{
		pos -= view * slideSpeed * slideSpeedMultiplier * deltaTime;
	}
	if (slideDir.left)
	{
		pos -= right * slideSpeed * slideSpeedMultiplier * deltaTime;
	}
	if (slideDir.right)
	{
		pos += right * slideSpeed * slideSpeedMultiplier * deltaTime;
	}
}

glm::mat4 Camera::CalcOriginCameraSpaceV(glm::vec3 offset) const
{
	glm::mat4 originCameraSpaceV = glm::transpose(glm::mat4(originV[0][0], originV[0][1],  originV[0][2], offset.x, 
		originV[1][0], originV[1][1],  originV[1][2], offset.y,
		originV[2][0], originV[2][1],  originV[2][2], offset.z,
		0, 0, 0, 1.0f));

	return originCameraSpaceV;
}

glm::mat4 Camera::CalcCameraSpaceV(glm::vec3 offset) const
{
	glm::mat4 cameraSpaceV = glm::transpose(glm::mat4(reverseV[0][0], reverseV[0][1],  reverseV[0][2], offset.x, 
		reverseV[1][0], reverseV[1][1],  reverseV[1][2], offset.y,
		reverseV[2][0], reverseV[2][1],  reverseV[2][2], offset.z,
		0, 0, 0, 1.0f));

	return cameraSpaceV;
}

void Camera::KeyPressEvent(QKeyEvent *evt)
{
	switch(evt->key())
	{
	case Qt::Key_W:
		slideDir.front = true;
		break;
	case Qt::Key_S:
		slideDir.back = true;
		break;
	case Qt::Key_A:
		slideDir.left = true;
		break;
	case Qt::Key_D:
		slideDir.right = true;
		break;
	case Qt::Key_R:
		Reset();
		break;
	case Qt::Key_F:
		pos = pickCellPosition + glm::vec3(3.0f, 3.0f, 3.0f);
		view = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
		CalcInitYawPitch();
		yawOffset = curYaw;
		pitchOffset = curPitch;
		break;
	case Qt::Key_Shift:
		slideSpeedMultiplier = 3.0f;
		break;
	}
}

void Camera::KeyReleaseEvent(QKeyEvent *evt)
{
	switch(evt->key())
	{
	case Qt::Key_W:
		slideDir.front = false;
		break;
	case Qt::Key_S:
		slideDir.back = false;
		break;
	case Qt::Key_A:
		slideDir.left = false;
		break;
	case Qt::Key_D:
		slideDir.right = false;
		break;
	case Qt::Key_Shift:
		slideSpeedMultiplier = 1.0f;
		break;
	}
}

bool Camera::IsBlockInFrustum(glm::ivec3 v) const
{
	for (int i=-1; i<=1; i+=2)
	{
		for (int j=-1; j<=1; j+=2)
		{
			for (int k=-1; k<=1; k+=2)
			{
				glm::vec3 corner(v.x + i, v.y + j, v.z + k);
				if (IsVertexInFrustum(corner))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Camera::CalcInitYawPitch()
{
	//calculate pitch/yaw according to the view
	initialYaw = glm::degrees(atanf(view.x / view.z)) + 180;
	initialPitch = glm::degrees(acosf(view.y));
}

void Camera::UpdateCameraVectors(float yaw, float pitch)
{
	ClampPitch(pitch);

	glm::vec3 rawView;

	rawView.x = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	rawView.y = cos(glm::radians(pitch));
	rawView.z = cos(glm::radians(yaw)) * sin(glm::radians(pitch));

	view = glm::normalize(rawView);
	right = glm::normalize(glm::cross(view, worldUp));
	up = glm::normalize(glm::cross(right, view));
}

void Camera::UpdateReverseCameraVectors(float yaw, float pitch)
{
	ClampPitch(pitch);

	glm::vec3 rawView;

	rawView.x = sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	rawView.y = cos(glm::radians(pitch));
	rawView.z = cos(glm::radians(yaw)) * sin(glm::radians(pitch));

	reverseView = glm::normalize(rawView);
	reverseRight = glm::normalize(glm::cross(reverseView, worldUp));
	reverseUp = glm::normalize(glm::cross(reverseRight, reverseView));
}

void Camera::UpdateFrustum()
{
	float t;

	frustum[0][0] = VP[0][3] + VP[0][0];
	frustum[0][1] = VP[1][3] + VP[1][0];
	frustum[0][2] = VP[2][3] + VP[2][0];
	frustum[0][3] = VP[3][3] + VP[3][0];

	t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
	frustum[0][0] /= t;
	frustum[0][1] /= t;
	frustum[0][2] /= t;
	frustum[0][3] /= t;

	frustum[1][0] = VP[0][3] - VP[0][0];
	frustum[1][1] = VP[1][3] - VP[1][0];
	frustum[1][2] = VP[2][3] - VP[2][0];
	frustum[1][3] = VP[3][3] - VP[3][0];

	t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
	frustum[1][0] /= t;
	frustum[1][1] /= t;
	frustum[1][2] /= t;
	frustum[1][3] /= t;

	frustum[2][0] = VP[0][3] + VP[0][1];
	frustum[2][1] = VP[1][3] + VP[1][1];
	frustum[2][2] = VP[2][3] + VP[2][1];
	frustum[2][3] = VP[3][3] + VP[3][1];

	t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
	frustum[2][0] /= t;
	frustum[2][1] /= t;
	frustum[2][2] /= t;
	frustum[2][3] /= t;

	frustum[3][0] = VP[0][3] - VP[0][1];
	frustum[3][1] = VP[1][3] - VP[1][1];
	frustum[3][2] = VP[2][3] - VP[2][1];
	frustum[3][3] = VP[3][3] - VP[3][1];

	t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
	frustum[3][0] /= t;
	frustum[3][1] /= t;
	frustum[3][2] /= t;
	frustum[3][3] /= t;

	frustum[4][0] = VP[0][3] + VP[0][2];
	frustum[4][1] = VP[1][3] + VP[1][2];
	frustum[4][2] = VP[2][3] + VP[2][2];
	frustum[4][3] = VP[3][3] + VP[3][2];

	t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
	frustum[4][0] /= t;
	frustum[4][1] /= t;
	frustum[4][2] /= t;
	frustum[4][3] /= t;

	frustum[5][0] = VP[0][3] - VP[0][2];
	frustum[5][1] = VP[1][3] - VP[1][2];
	frustum[5][2] = VP[2][3] - VP[2][2];
	frustum[5][3] = VP[3][3] - VP[3][2];

	t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
	frustum[5][0] /= t;
	frustum[5][1] /= t;
	frustum[5][2] /= t;
	frustum[5][3] /= t;
}

bool Camera::IsVertexInFrustum(glm::vec3 v) const
{
	for (int i=0; i<6; i++)
	{
		if (frustum[i][0] * v.x + frustum[i][1] * v.y + frustum[i][2] * v.z + frustum[i][3] <= 0)
		{
			return false;
		}
	}
	return true;
}

bool Camera::IsVertexInFrustum(glm::ivec3 v) const
{
	for (int i=0; i<6; i++)
	{
		if (frustum[i][0] * v.x + frustum[i][1] * v.y + frustum[i][2] * v.z + frustum[i][3] <= 0)
		{
			return false;
		}
	}
	return true;
}

void Camera::ClampPitch(float &pitch)
{
	if(pitch > 179.0f)
		pitch = 179.0f;
	if(pitch < 1.0f)
		pitch = 1.0f;
}