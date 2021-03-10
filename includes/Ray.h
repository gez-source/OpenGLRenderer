#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

//namespace ath
//{
	//namespace physics 
	//{
		//using namespace math;

		class Ray
		{
		public:
			Vector3 Origin;
			Vector3 Direction;
			float Length;

			Ray();

			Ray(Vector3 origin, Vector3 direction);

			Ray(Vector3 origin, Vector3 direction, float length);

			Ray operator *(Matrix4 right) const
			{
				Ray lhs = *this;
				return TransformRay(lhs, right);
			}

			static Ray TransformRay(Ray ray, Matrix4 transformMatrix);
		};
	//}
//}

#endif
