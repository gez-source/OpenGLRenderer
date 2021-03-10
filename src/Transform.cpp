#include "Transform.h"
#include "MathHelpers.h"
#include <iostream>
//#include <mmsystem.h>

//namespace ath
//{


//Nullable<Transform> Transform::rootTransform = Nullable<Transform>(); //TEST:HACK:

Transform::Transform() :
	ParentTransform(Nullable<Transform*>()),
	localMatrix(Matrix4::GetIdentity()),
	worldMatrix(Matrix4::GetIdentity()),
	QOrientationLocal(Quaternion::GetIdentity()),
	QOrientationWorld(Quaternion::GetIdentity()),
	upVector(Vector3::Up),
	forwardVector(Vector3::Forward),
	viewRotation(Matrix4::GetIdentity()),
	qRotWorld(Quaternion::GetIdentity())
{

	Children = new std::vector<Transform>();
	//rootTransform = Nullable<Transform>(); //TEST:HACK:
}

Transform::~Transform()
{
	//delete Children;
}

Vector3 Transform::GetLocalPosition()
{
	return localMatrix.GetTranslation();
}

Vector3 Transform::GetLocalScale()
{
	return localMatrix.GetScale();
}

Vector3 Transform::GetPosition()
{
	return Matrix4::TransformVector(localMatrix, GetLocalPosition());
}

Vector3 Transform::GetForwardVector()
{
	Quaternion::DeconstructQuaternion(QOrientationLocal, forwardVector, upVector);

	return forwardVector;
}

Vector3 Transform::GetUpVector()
{
	Quaternion::DeconstructQuaternion(QOrientationLocal, forwardVector, upVector);

	return upVector;
}

float Transform::GetLocalYaw()
{
	return localYaw;
}

float Transform::GetLocalPitch()
{
	return localPitch;
}

float Transform::GetLocalRoll()
{
	return localRoll;
}

float Transform::GetYaw()
{
	return yaw;
}
float Transform::GetPitch()
{
	return pitch;
}
float Transform::GetRoll()
{
	return roll;
}

Vector3 Transform::GetRotation()
{
    Vector3 result = Vector3(GetYaw(), GetPitch(), GetRoll());

    return result;
}

Vector3 Transform::GetWorldPosition()
{
	return GetPosition();
}

void Transform::SetWorldPosition(Vector3 position)
{
	SetPosition(position);
}

void Transform::SetLocalPosition(Vector3 value)
{
	Translate(value);
}

void Transform::SetLocalScale(Vector3 value)
{
	Scale(value);
}

void Transform::SetPosition(Vector3 value)
{
	// TODO: TEST: World position to local position translation
	Vector3 local = Matrix4::TransformVector(worldMatrix, value);

	SetLocalPosition(local);
}

void Transform::SetForwardVector(Vector3 value)
{
	forwardVector = value;

	updatelocalVectorRotation();
}

void Transform::SetUpVector(Vector3 value)
{
	upVector = value;

	updatelocalVectorRotation();
}

void Transform::SetLocalYaw(float yawValue)
{
	localYaw = yawValue;
	updatelocalQuaternionRotation();
}

void Transform::SetLocalPitch(float pitchValue)
{
	localPitch = pitchValue;
	updatelocalQuaternionRotation();
}

void Transform::SetLocalRoll(float rollValue)
{
	localRoll = rollValue;
	updatelocalQuaternionRotation();
}

void Transform::SetYaw(float yawValue)
{
	yaw = yawValue;
	updateworldQuaternionRotation();
}

void Transform::SetPitch(float pitchValue)
{
	pitch = pitchValue;
	updateworldQuaternionRotation();
}

void Transform::SetRoll(float rollValue)
{
	roll = rollValue;
	updateworldQuaternionRotation();
}


void Transform::updatelocalVectorRotation()
{
	QOrientationLocal = Quaternion::LookRotation(forwardVector, upVector);
}
void Transform::updatelocalQuaternionRotation()
{
	QOrientationLocal = Quaternion::Euler(GetLocalYaw(), GetLocalPitch(), GetLocalRoll());
}
void Transform::updateworldQuaternionRotation()
{
	QOrientationWorld = Quaternion::Euler(GetYaw(), GetPitch(), GetRoll());
}

/// <summary>
/// Scales a vector by the specified scale.
/// </summary>
void Transform::Scale(float scalar)
{
	localMatrix.Scale(Vector3::Multiply(Vector3::One, scalar));
}
void Transform::Scale(Vector3 scale)
{
	localMatrix.Scale(scale);
}

void Transform::RotateX(float angleInDegrees)
{
	localMatrix.RotateX(angleInDegrees);
}
void Transform::RotateY(float angleInDegrees)
{
	localMatrix.RotateY(angleInDegrees);
}
void Transform::RotateZ(float angleInDegrees)
{
	localMatrix.RotateZ(angleInDegrees);
}

void Transform::Rotate(float rotationXPitchInDegrees, float rotationYYawInDegrees, float rotationZRollInDegrees)
{
	Quaternion rotation = Quaternion::Euler(rotationXPitchInDegrees, rotationYYawInDegrees, rotationZRollInDegrees);
	localMatrix = rotation.RotationMatrix() * localMatrix;
}


/// <summary>
/// Translates a vector by the specified translation.
/// </summary>
/// <param name="vector"></param>
/// <param name="translation"></param>
/// <returns></returns>
void Transform::Translate(float x, float y, float z)
{
	Vector3 transl = Vector3(x, y, z);
	Translate(transl);
}

/// <summary>
/// Translates a vector by the specified translation.
/// </summary>
/// <param name="vector"></param>
/// <param name="translation"></param>
/// <returns></returns>
void Transform::Translate(Vector3 translation)
{
	localMatrix.Translate(translation);
}

Matrix4 Transform::LocalMatrix() // LocalToWorldMatrix
{
	return localMatrix;
}

void Transform::ScaleFieldOfView(float scaleFactor)
{
	if (scaleFactor > 1)
	{
		_fovScale = (1 / _fovScale) * (1 / scaleFactor);
	}
	else
	{
		_fovScale = scaleFactor;
	}
}

Vector4 Transform::TransformSRT(Vector4 vector, Vector3 scale, Vector3 rotationYPR, Vector3 translation)
{
	// Manual SRT 4x4 Matrix

	Vector4 result;


	Matrix4 matrixSRT = Matrix4::GetIdentity();

	matrixSRT.Scale(scale);
	matrixSRT *= Matrix4::RotationYawPitchRoll(rotationYPR.y, rotationYPR.x, rotationYPR.z);
	matrixSRT.Translate(translation);

	result = Matrix4::TransformVector(matrixSRT, vector);

	return result;
}

Matrix4 Transform::RecalculateWorldTransformations()
{
	Matrix4 combinedTransformationMatrix = Matrix4::CreateSQTMatrix(GetLocalScale(), QOrientationLocal, GetLocalPosition());

	if (ParentTransform.is_set())
	{
		// Recalculate prior transformations
		Transform* p = ParentTransform.get();

		while (p != nullptr)
		{
			combinedTransformationMatrix *= Matrix4::CreateSQTMatrix(p->GetLocalScale(), p->QOrientationLocal, p->GetLocalPosition());

			p->worldMatrix = combinedTransformationMatrix;

			if (p->ParentTransform.is_set())
			{
				p = p->ParentTransform.get();
			}
			else
			{
				p = nullptr;
			}
		}
	}

	worldMatrix = combinedTransformationMatrix;

	return combinedTransformationMatrix;
}

Matrix4 Transform::RecalculateLocalToWorldMatrix()
{
	Matrix4 localToWorld = localMatrix;

	return localToWorld;
}

Vector4 Transform::TransformLocalToWorldspace(Vector3 localPosition)
{
	// Transform the local position to world space
	// applying transformations from the object and its parents to derive a world position

	Vector4 inputLocal(localPosition, 1.0f);

	//Vector4 result = RecalculateLocalToWorldMatrix() * inputLocal;
	RecalculateWorldTransformations();

	Vector4 result = worldMatrix.TransformVector(inputLocal);

	result.w = 1.0f;

	return result;
}

//}
