#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <QKeyEvent>

#define STD_DELTA_TIME 0.01666

#define MAX_POS glm::ivec3(1e6, 1e6, 1e6)
#define MAX_POSF glm::vec3(1e6, 1e6, 1e6)

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

struct SlideDir
{
	SlideDir(bool front = false, bool back = false, bool left = false, bool right = false) :
	front(front), back(back), left(left), right(right)
	{

	}
	bool front, back, left, right;
};

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 worldUp, 
		float slideSpeed = 8.0f);

	void Update(float yaw, float pitch, float deltaTime);
	void Reset();
	void Slide(float deltaTime);//摄像机的滑行

	glm::vec3 GetPosition() const; 
	glm::vec3 GetDirection() const; 

	void SetAspectRatio(int w, int h);
	void SetPosition(glm::vec3 pos);
	void SetPickCellPosition(glm::vec3 pickCellPosition);

	glm::mat4 CalcOriginCameraSpaceV(glm::vec3 offset) const;
	glm::mat4 CalcCameraSpaceV(glm::vec3 offset) const;

	void KeyPressEvent(QKeyEvent *evt);
	void KeyReleaseEvent(QKeyEvent *evt);

	bool IsBlockInFrustum(glm::ivec3 center) const;

	glm::mat4 O;
	glm::mat4 V;
	glm::mat4 reverseV;
	glm::mat4 originV;
	glm::mat4 P;
	glm::mat4 VP;

private:
	glm::vec3 pos;
	glm::vec3 originPos;
	glm::vec3 view;
	glm::vec3 reverseView;
	glm::vec3 up;
	glm::vec3 reverseUp;
	glm::vec3 right;
	glm::vec3 reverseRight;
	glm::vec3 worldUp;

	float fov;
	float aspectRatio;
	float nearDist;
	float farDist;

	float initialYaw;
	float initialPitch;

	float curYaw;
	float curPitch;

	float yawOffset;
	float pitchOffset;

	SlideDir slideDir;
	float slideSpeed;

	float slideSpeedMultiplier;

	float frustum[6][4];

	glm::vec3 pickCellPosition;//摄像机所选定的cell的位置

	void CalcInitYawPitch();
	void UpdateCameraVectors(float yaw, float pitch);
	void UpdateReverseCameraVectors(float yaw, float pitch);
	void UpdateFrustum();

	bool IsVertexInFrustum(glm::vec3 v) const;
	bool IsVertexInFrustum(glm::ivec3 v) const;

	void ClampPitch(float &pitch);
};

#endif //_CAMERA_H