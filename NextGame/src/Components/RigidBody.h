#pragma once
#include <cassert>

#include "Transform.h"
#include "Math/Matrix.h"


struct RigidBody
{
	// constant quantities mass and body space inertial tensor
	double Mass;
	Mat3X3 InertialTensorBody;           // IBody(t) 
	Mat3X3 InertialTensorBodyInverse;    // IBody^-1(t) 

	// state variables

	Vec3d LinearMomentum;                // P(t)
	Vec3d AngularMomentum;               // L(t)

	// Derived Quantity
	Vec3d Velocity;                      // v(t) = P(t) / M
	Mat3X3 InertialTensorInverse;        // I^-1(t) = R(t) * IBody(t) R^T(t)
	Vec3d AngularVelocity;               // w(t) = I^-1(t) * L(t)
	Vec3d Force;                         // F(t)
	Vec3d Torque;                        // T(t)

	// Total Force and Torque exerted

	bool IsStatic;
};


inline void SetInertialTensorBody(RigidBody& rb, const Vec3d scale)
{
	//TODO: default to cube (Add more shapes later)
	rb.InertialTensorBody[0] = { scale.Y * scale.Y + scale.Z * scale.Z, 0, 0 };
	rb.InertialTensorBody[1] = { 0, scale.X * scale.X + scale.Z * scale.Z, 0 };
	rb.InertialTensorBody[2] = { 0, 0, scale.X * scale.X + scale.Y + scale.Y };
	rb.InertialTensorBody = (rb.Mass / 12) * rb.InertialTensorBody;
	rb.InertialTensorBodyInverse = rb.InertialTensorBody.Inverse();
}


/**
 * \brief Setup an Test RigidBody (cube)
 */
inline void SetUpRigidBody(RigidBody& rb, const Transform& t)
{
	rb.Mass = 5;
	rb.LinearMomentum = {};
	rb.AngularMomentum = {};
	rb.Force = {0, 0, 0};
	rb.Torque = {50, 25, 0};
	rb.Velocity = rb.LinearMomentum / rb.Mass;
	SetInertialTensorBody(rb, t.Scaling);
	const Mat3X3 Rotation = GetRotationMatrix(t);
	rb.InertialTensorInverse = Rotation * rb.InertialTensorBodyInverse * Transpose(Rotation);
	rb.AngularVelocity = rb.InertialTensorInverse * rb.AngularMomentum;
}




inline void IntegrationStep(RigidBody& rb, Transform& t,  float deltaTime)
{

	// Step 1: ODE solve using basic Euler method

	// position = dt * velocity
	Vec3d newPos  = t.Position + rb.Velocity * deltaTime;
	// Rotation Matrix delta = (Star(Angular Velocity) * Rotation Matrix)
	Mat3X3 AngularMatrix{};
	AngularMatrix[0] = { 0, -rb.AngularVelocity.Z, rb.AngularVelocity.Y };
	AngularMatrix[1] = { rb.AngularVelocity.Z, 0, -rb.AngularVelocity.X  };
	AngularMatrix[2] = { -rb.AngularVelocity.Y, rb.AngularVelocity.X, 0  };

	Mat3X3 Rotation = GetRotationMatrix(t);
	Mat3X3 RotationDelta = AngularMatrix * Rotation;
	Rotation = Rotation + RotationDelta * deltaTime;

	// Update Transform
	SetTransform(t, Rotation, newPos);

	// linearMomentum = dt * force
	rb.LinearMomentum = rb.LinearMomentum + rb.Force * deltaTime;
	// AngularMomentum = dt * torque
	rb.AngularMomentum = rb.AngularMomentum + rb.Torque * deltaTime;

	// Step 2: Update dt parameters

	// velocity = linearMomentum / Mass
	rb.Velocity = rb.LinearMomentum / rb.Mass;
	// inertialTensorInverse = R * inertialTensorBodyInverse * R^T
	rb.InertialTensorInverse = Rotation * rb.InertialTensorBodyInverse * Transpose(Rotation);
	// AngularVelocity = inertialTensorInverse * AngularMomentum
	rb.AngularVelocity = rb.InertialTensorInverse * rb.AngularMomentum;

	// Compute Force and Torque (derivative of Linear Momentum and Angular Momentum)
	// the forces applied are user defined
	const Vec3d gravity = { 0, -9.81, 0 };
	rb.Force = gravity;
	rb.Torque = {};
}