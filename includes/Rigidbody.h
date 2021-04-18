#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector3.h"
#include "MathExt.h"

enum ForceType
{
    Force = 0,
    Impulse = 1, // Linear Impulse ?
    VelocityChange = 2,
    Acceleration = 5,
};

class Rigidbody
{
public:
    bool UseGravity;
    float mass = 1.0f;
    float Gravity = -9.81f;
    float drag = 0.05f;
    float angularDrag = 0.05f;
    float VelocityDampening = 0.0001f;
    float AngularDampening = 0.0001f;
    Vector3 LinearMomentum;
    Vector3 Acceleration;
    Vector3 velocity;
    Vector3 angularVelocity;

private:
        Vector3 _netWorldForce;
        Vector3 _netWorldTorque;
        Vector3 _netRelativeForce;
        Vector3 _netRelativeTorque;
        //Transform _transform;

public:
    void AddRelativeForce(Vector3 force, ForceType forceType = ForceType::Force);
    void AddForce(Vector3 force, ForceType forceType = ForceType::Force);
    void AddRelativeTorque(Vector3 torque, ForceType forceType = ForceType::Force);
    void AddTorque(Vector3 torque, ForceType forceType = ForceType::Force);

private:
    void ApplyWorldTranslation();
    void ApplyLocalTranslation();
    void ApplyLocalRotation();
    void ApplyLocalPhysics();
    void ApplyGravity();

public: 
    void UpdatePhysics();
};

#endif