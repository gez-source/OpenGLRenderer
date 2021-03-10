#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Nullable.h"
#include "Matrix4.h"
#include <vector>

//namespace ath
//{
	class Transform;
	class Camera;

	typedef Vector2i(Transform::*ProjectionFunct)(Vector3);

	class Transform
	{
	public:

		//static Nullable<Transform> rootTransform; //TEST:HACK:
		Nullable<Transform*> ParentTransform;

		std::vector<Transform>* Children;

		/// <summary>
		/// The combined set of local transformations to apply to the object.
		/// </summary>
		Matrix4 localMatrix = Matrix4::GetIdentity();
		Matrix4 worldMatrix;// the new combined transform matrix

		Quaternion QOrientationLocal;
		Quaternion QOrientationWorld;

	private:

		Vector3 forwardVector;
		Vector3 upVector;
		float localYaw, localPitch, localRoll;
		float yaw, pitch, roll;
		Quaternion qRotWorld;
		Matrix4 viewRotation;

	public:
		float _fovScale = 1;

	public:
		Transform();
		~Transform();

		Vector3 GetLocalPosition();
		Vector3 GetLocalScale();
		Vector3 GetPosition();
		Vector3 GetForwardVector();
		Vector3 GetUpVector();
		float GetLocalYaw();
		float GetLocalPitch();
		float GetLocalRoll();
		float GetYaw();
		float GetPitch();
		float GetRoll();
		Vector3 GetRotation();
		Vector3 GetWorldPosition();

		void SetWorldPosition(Vector3 position);
		void SetLocalPosition(Vector3 value);
		void SetLocalScale(Vector3 value);
		void SetPosition(Vector3 value);
		void SetForwardVector(Vector3 value);
		void SetUpVector(Vector3 value);
		void SetLocalYaw(float yawValue);
		void SetLocalPitch(float pitchValue);
		void SetLocalRoll(float rollValue);
		void SetYaw(float yawValue);
		void SetPitch(float pitchValue);
		void SetRoll(float rollValue);


		void updatelocalVectorRotation();
		void updatelocalQuaternionRotation();
		void updateworldQuaternionRotation();


		Camera GetCamera();

		ProjectionFunct worldToScreen();

		/// <summary>
		/// Scales a vector by the specified scale.
		/// </summary>
		void Scale(float scalar);
		void Scale(Vector3 scale);

		void RotateX(float angleInDegrees);
		void RotateY(float angleInDegrees);
		void RotateZ(float angleInDegrees);
		void Rotate(float rotationXPitchInDegrees, float rotationYYawInDegrees, float rotationZRollInDegrees);

		/// <summary>
		/// Translates a vector by the specified translation.
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="translation"></param>
		/// <returns></returns>
		void Translate(float x, float y, float z);

		/// <summary>
		/// Translates a vector by the specified translation.
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="translation"></param>
		/// <returns></returns>
		void Translate(Vector3 translation);

		Matrix4 LocalMatrix();

		void ScaleFieldOfView(float scaleFactor);

		Vector4 TransformSRT(Vector4 vector, Vector3 scale, Vector3 rotationYPR, Vector3 translation);

		Matrix4 RecalculateWorldTransformations();

		Matrix4 RecalculateLocalToWorldMatrix();

		Vector4 TransformLocalToWorldspace(Vector3 localPosition);

	};

//}
#endif
