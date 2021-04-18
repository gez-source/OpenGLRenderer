#include "Rigidbody.h"

void Rigidbody::AddRelativeForce(Vector3 force, ForceType forceType)
{
    Vector3 acceleration;
    Vector3 F;

    switch (forceType)
    {
    case ForceType::Force:
        _netRelativeForce += force;
        break;
    case ForceType::Acceleration:
        // Newton's 2nd Law of motion, F = ma
        acceleration = force;
        F = acceleration * mass; // 'force' in this case is a property of acceleration.

        // Is this How you Apply an Acceleration?
        _netRelativeForce += F;

        break;
    case ForceType::Impulse:

        break;
    case ForceType::VelocityChange:
        velocity += force; // 'force' in this case is a change in velocity.
        break;
    }
}

void Rigidbody::AddForce(Vector3 force, ForceType forceType)
{
    Vector3 acceleration;
    Vector3 F;

    switch (forceType)
    {
    case ForceType::Force:
        _netWorldForce += force;
        break;
    case ForceType::Acceleration:
        // Newton's 2nd Law of motion, F = ma
        acceleration = force;
        F = acceleration * mass; // 'force' in this case is a property of acceleration.

        // Is this How you Apply an Acceleration?
        _netWorldForce += F;

        break;
    case ForceType::Impulse:
        // A one time force for a short time period (which is why it has to be big)
        break;
    case ForceType::VelocityChange:
        velocity += force; // 'force' in this case is a change in velocity.
        break;
    }


}

void Rigidbody::AddRelativeTorque(Vector3 torque, ForceType forceType)
{
    Vector3 acceleration;
    Vector3 F;

    switch (forceType)
    {
    case ForceType::Force:
        _netRelativeTorque += torque;
        break;
    case ForceType::Acceleration:
        // Newton's 2nd Law of motion, F = ma
        acceleration = torque;
        F = acceleration * mass; // 'force' in this case is a property of acceleration.

        // Is this How you Apply an Acceleration?
        _netRelativeTorque += F;

        break;
    case ForceType::Impulse:
        // A one time force for a short time period (which is why it has to be big)
        break;
    case ForceType::VelocityChange:
        angularVelocity += torque; // 'force' in this case is a change in velocity.
        break;
    }

}

void Rigidbody::AddTorque(Vector3 torque, ForceType forceType)
{
    Vector3 acceleration;
    Vector3 F;

    switch (forceType)
    {
    case ForceType::Force:
        _netWorldTorque += torque;
        break;
    case ForceType::Acceleration:
        // Newton's 2nd Law of motion, F = ma
        acceleration = torque;
        F = acceleration * mass; // 'force' in this case is a property of acceleration.

        // Is this How you Apply an Acceleration?
        _netWorldTorque += F;

        break;
    case ForceType::Impulse:
        // A one time force for a short time period (which is why it has to be big)
        break;
    case ForceType::VelocityChange:
        angularVelocity += torque; // 'force' in this case is a change in velocity.
        break;
    }
}

void Rigidbody::ApplyWorldTranslation()
{
    if (_netWorldForce == Vector3::Zero) return;

    // Get the current acceleration
    Acceleration = _netWorldForce / mass;


    velocity += _netWorldForce * VelocityDampening;

    // Apply the new velocity to the current position
    //_transform.position += velocity;
}

void Rigidbody::ApplyLocalTranslation()
{
    if (_netRelativeForce == Vector3::Zero) return;

    // Get the current acceleration
    Acceleration = _netRelativeForce / mass;


    velocity += _netRelativeForce * VelocityDampening;

    // Apply a local transformation (maintaining integrity of the Transformation Hierarchy )

    // Apply the new velocity to the current position
    //_transform.localPosition += velocity;
}

void Rigidbody::ApplyLocalRotation()
{
    if (_netRelativeTorque == Vector3::Zero) return;

    angularVelocity += _netRelativeTorque * AngularDampening;


    // angularVelocity is measured in this case as radians per second.
    Vector3 eulerAngles = Vector3(Rad2Deg * angularVelocity.x, Rad2Deg * angularVelocity.y, Rad2Deg * angularVelocity.z);

    Quaternion newRotationQ = Quaternion::Euler(eulerAngles.x, eulerAngles.y, eulerAngles.z);


    //_transform.localRotation *= newRotationQ;
}

void Rigidbody::ApplyLocalPhysics()
{
    //ApplyLocalTranslation();
    ApplyLocalRotation();

}

void Rigidbody::ApplyGravity()
{
    Vector3 up = Vector3::Up;

    // Gravity is a type of acceleration so must be multiplied by mass.
    Vector3 forceGravity = up * -(mass * Gravity);

    //Acceleration = forceGravity;

    AddForce(forceGravity);
}

void Rigidbody::UpdatePhysics()
{
    if (UseGravity)
    {
        ApplyGravity();
    }

    // Forces applied to local position.
    ApplyLocalPhysics();
}