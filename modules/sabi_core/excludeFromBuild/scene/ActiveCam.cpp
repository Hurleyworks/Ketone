#include "ActiveCam.h"

// This source file was auto-generated by ClassMate++
// Created: 9 May 2019 6:32:48 am
// Copyright (c) 2019, HurleyWorks


// dtor
ActiveCam::~ActiveCam ()
{	
}

void ActiveCam::lookAt(const Vector3f & eyePoint, const Vector3f & target, const Vector3f & up)
{
	this->target = target;
	Eigen::Vector3f f = (target - eyePoint).normalized();

	eye = eyePoint;
	//worldUp = up.normalized();
	viewDirection = f;

	/// Check for degeneracies.If the upDir and targetDir are parallel
	// or opposite, then compute a new, arbitrary up direction that is
	// not parallel or opposite to the targetDir.
	Vector3f upDir = up.normalized();

	if (upDir.cross(f).squaredNorm() == 0)
	{
		upDir = f.cross(Vector3f::UnitX());
		if (upDir.squaredNorm() == 0)
			upDir = f.cross(Vector3f::UnitZ());

		upDir *= -1.0f; // to match Cinder
	}

	Matrix3f camAxes;
	camAxes.col(2) = -f; 
	camAxes.col(0) = worldUp.cross(camAxes.col(2)).normalized();
	camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();
	orientation = Quaternionf(camAxes);
	
	forward = -(orientation * Vector3f::UnitZ());

	viewMatrixCached = false;
}

void ActiveCam::lookAt(const Vector3f & eyePoint, const Vector3f & target)
{
	this->target = target;
	eye = eyePoint;
	Eigen::Vector3f f = (target - eyePoint).normalized();
	viewDirection = f;

	/// Check for degeneracies.If the upDir and targetDir are parallel
	// or opposite, then compute a new, arbitrary up direction that is
	// not parallel or opposite to the targetDir.
	Vector3f upDir = worldUp;

	if (upDir.cross(f).squaredNorm() == 0)
	{
		upDir = f.cross(Vector3f::UnitX());
		if(upDir.squaredNorm() == 0)
			upDir = f.cross(Vector3f::UnitZ());

		upDir *= -1.0f; // to match Cinder
	}

	Matrix3f camAxes;
	camAxes.col(2) = -f;
	camAxes.col(0) = upDir.cross(camAxes.col(2)).normalized();
	camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();
	orientation = Quaternionf(camAxes);
	
	forward = -(orientation * Vector3f::UnitZ());

	viewMatrixCached = false;
}

void ActiveCam::setTarget(const Vector3f & target)
{
    if (!this->target.isApprox (target))
	{
        this->target = target;
		Vector3f newDirection = target - eye;
		setDirection(newDirection.normalized());
	}
}

void ActiveCam::setDirection(const Vector3f & newDirection)
{
	// TODO implement it computing the rotation between newDirection and current dir ?
	Vector3f up = getUp();

	Matrix3f camAxes;

	camAxes.col(2) = (-newDirection).normalized();
	camAxes.col(0) = up.cross(camAxes.col(2)).normalized();
	camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();
	orientation = Quaternionf(camAxes);

	viewMatrixCached = false;
}

void ActiveCam::rotateAroundTarget(const Quaternionf & q)
{
	// update the transform matrix
	if(!viewMatrixCached)
		calcViewMatrix();

	Vector3f t = viewMatrix * target;

	viewMatrix = Translation3f(t) * q * Translation3f(-t) * viewMatrix;

	Quaternionf qa(viewMatrix.linear());
	qa = qa.conjugate();
	orientation = qa;

	eye = -(qa * viewMatrix.translation());

	forward = -(orientation * Vector3f::UnitZ());
	viewDirection = (target - eye).normalized();

	viewMatrixCached = false;
}

void ActiveCam::zoom(float d)
{
	float dist = (eye - target).norm();
	if (dist > d)
	{
		eye = eye + viewDirection * d;
		forward = -(orientation * Vector3f::UnitZ());
		viewMatrixCached = false;
	}
}

void ActiveCam::calcMatrices() const
{
	if (!viewMatrixCached) calcViewMatrix();
	if (!projectionCached) calcProjection();
}

void ActiveCam::getFrustum(float * left, float * top, float * right, float * bottom, float *nearVal, float *farVal) const
{
	calcMatrices();

	*left = frustumLeft;
	*top = frustumTop;
	*right = frustumRight;
	*bottom = frustumButtom;
	*nearVal = nearClip;
	*farVal = farClip;
}

void ActiveCam::track(const Vector2f & newPoint2D)
{
	Vector3f newPoint3D;
	bool newPointOk = mapToSphere(newPoint2D, newPoint3D);

	if (lastPointOK && newPointOk)
	{
		Vector3f axis = lastPoint3D.cross(newPoint3D).normalized();
		float cos_angle = lastPoint3D.dot(newPoint3D);
		if (std::abs(cos_angle) < 1.0)
		{
			float angle = 2. * acos(cos_angle);
			
			rotateAroundTarget(Quaternionf(AngleAxisf(angle, axis)));
		}
	}

	lastPoint3D = newPoint3D;
	lastPointOK = newPointOk;
}

void ActiveCam::calcViewMatrix() const
{
   // LOG (DBUG) << _FN_;

	mW = -viewDirection.normalized();
	mU = orientation * Vector3f::UnitX(); 
	mV = orientation * Vector3f::UnitY();  

	Quaternionf q = orientation.conjugate();
	viewMatrix.linear() = q.toRotationMatrix();

	if (!wabi::isOrthogonal<float>(viewMatrix.linear()))
	{
		Matrix3f m = viewMatrix.linear();
		if (!wabi::reOrthogonalize(m))
		{
			throw std::runtime_error("Could not fix non-orthogongal matrix");
		}

		viewMatrix.linear() = m;
	}

	viewMatrix.translation() = -(viewMatrix.linear() * eye);

	forward = -(orientation * Vector3f::UnitZ());

	viewMatrixCached = true;
	inverseModelViewCached = false;
}

void ActiveCam::calcInverseView() const
{
	if (!viewMatrixCached) calcViewMatrix();

	inverseModelViewMatrix = viewMatrix.inverse().matrix();
	inverseModelViewCached = true;
}

wabi::Ray3f ActiveCam::calcRay(float uPos, float vPos, float imagePlaneAspectRatio) const
{
	calcMatrices();

	float s = (uPos - 0.5f)  * imagePlaneAspectRatio;
	float t = vPos - 0.5f;
	float viewDistance = imagePlaneAspectRatio / wabi::Math<float>::FAbs(frustumRight - frustumLeft) * nearClip;

	return wabi::Ray3f(eye, (mU * s + mV * t - (mW * viewDistance)).normalized());
}

bool ActiveCam::mapToSphere(const Vector2f & p2, Vector3f & v3)
{ 
	float w = screenSize.x();
	float h = screenSize.y();

	if ((p2.x() >= 0) && (p2.x() <= int(w)) &&
		(p2.y() >= 0) && (p2.y() <= int(h)))
	{
		double x = (double)(p2.x() - 0.5*w) / (double)w;
		double y = (double)(0.5*h - p2.y()) / (double)h;
		double sinx = sin(wabi::Mathd::PI * x * 0.5);
		double siny = sin(wabi::Mathd::PI * y * 0.5);
		double sinx2siny2 = sinx * sinx + siny * siny;

		v3.x() = sinx;
		v3.y() = siny;
		v3.z() = sinx2siny2 < 1.0 ? sqrt(1.0 - sinx2siny2) : 0.0;

		return true;
	}
	else
		return false;
}

PerspectiveCam::PerspectiveCam()
	: ActiveCam()
{
	lookAt(DEFAULT_CAMERA_POSIIION, DEFAULT_CAMERA_TARGET, Vector3f(0, 1, 0));
	setPerspective(DEFAULT_FOV_DEGREES, DEFAULT_ASPECT, 0.1f, 1000);
}

PerspectiveCam::PerspectiveCam(int pixelWidth, int pixelHeight, float fovDegrees)
	: ActiveCam()
{
	float eyeX = pixelWidth / 2.0f;
	float eyeY = pixelHeight / 2.0f;
	float halfFov = wabi::Math<float>::PI * fovDegrees / 360.0f;
	float theTan = std::tan(halfFov);
	float dist = eyeY / theTan;
	float nearDist = dist / 10.0f;	// near / far clip plane
	float farDist = dist * 10.0f;
	float aspect = pixelWidth / (float)pixelHeight;

	setPerspective(fovDegrees, aspect, nearDist, farDist);
	lookAt(Vector3f(eyeX, eyeY, dist), Vector3f(eyeX, eyeY, 0.0f));
}

PerspectiveCam::PerspectiveCam(int pixelWidth, int pixelHeight, float fovDegrees, float nearPlane, float farPlane)
	: ActiveCam()
{
	float halfFov, theTan, aspect;

	float eyeX = pixelWidth / 2.0f;
	float eyeY = pixelHeight / 2.0f;
	halfFov = wabi::Math<float>::PI * fovDegrees / 360.0f;
	theTan = std::tan(halfFov);
	float dist = eyeY / theTan;
	aspect = pixelWidth / (float)pixelHeight;

	setPerspective(fovDegrees, aspect, nearPlane, farPlane);
	lookAt(Vector3f(eyeX, eyeY, dist), Vector3f(eyeX, eyeY, 0.0f));
}

void PerspectiveCam::setPerspective(float verticalFovDegrees, float aspectRatio, float nearPlane, float farPlane)
{
	fovDegrees = verticalFovDegrees;
	aspect = aspectRatio;
	nearClip = nearPlane;
	farClip = farPlane;

	projectionCached = false;
}

void PerspectiveCam::calcProjection() const
{
	frustumTop = nearClip * std::tan(wabi::Math<float>::PI / 180.0f * fovDegrees * 0.5f);
	frustumButtom = -frustumTop;
	frustumRight = frustumTop * aspect;
	frustumLeft = -frustumRight;

	Eigen::Matrix4f m = Eigen::Matrix4f::Zero();
	m(0, 0) = (2.0f * nearClip) / (frustumRight - frustumLeft);
	m(1, 1) = (2.0f * nearClip) / (frustumTop - frustumButtom);
	m(0, 2) = (frustumRight + frustumLeft) / (frustumRight - frustumLeft);
	m(1, 2) = (frustumTop + frustumButtom) / (frustumTop - frustumButtom);
	m(2, 2) = -(farClip + nearClip) / (farClip - nearClip);
	m(3, 2) = -1.0f;
	m(2, 3) = -(2.0f * farClip * nearClip) / (farClip - nearClip);
	
	projectionMatrix = m;

	projectionCached = true;
}



void ActiveCam::setFrame (const Frame& f)
{
    frame = f;
    viewMatrixCached = false;
}
