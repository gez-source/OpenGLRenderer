#include "JointTransform.h"

Matrix4 JointTransform::getLocalTransform()
{
	Matrix4 m = Matrix4::GetIdentity();
	m.Translate(position);
	m = m * rotation.RotationMatrix();
	return m;
}

JointTransform JointTransform::interpolate(JointTransform frameA, JointTransform frameB, float progression)
{
	Vector3 pos = Vector3::Lerp(frameA.position, frameB.position, progression);

	Quaternion rotation = Quaternion::Slerp(frameA.rotation, frameB.rotation, progression);

	Matrix4 m = Matrix4::GetIdentity();
	m.Translate(pos);
	m = m * rotation.RotationMatrix();

	return JointTransform(pos, rotation);
}