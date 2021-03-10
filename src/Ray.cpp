#include "Ray.h"

Ray::Ray()
{
	Origin = Vector3::Zero;
	Direction = Vector3::Forward;
	Length = FLT_MAX;
}

Ray::Ray(Vector3 origin, Vector3 direction)
{
	Origin = origin;
	Direction = direction;
	Length = FLT_MAX;
}

Ray::Ray(Vector3 origin, Vector3 direction, float length)
{
	Origin = origin;
	Direction = direction;
	Length = length;
}

Ray Ray::TransformRay(Ray ray, Matrix4 transformMatrix)
{
	Ray r;
	r.Origin = Matrix4::TransformVector(transformMatrix, ray.Origin);
	r.Direction = Matrix4::TransformVector(transformMatrix, ray.Direction);
	//r.Origin = ray.Origin.TransformVector(transformMatrix);
	//r.Direction = ray.Direction.TransformNormal(transformMatrix);
	return r;
}
