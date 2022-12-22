// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#include "Camera.h"
#include <ctype.h>
#include "foundation/PxMat33.h"
#include <PxScene.h>
#include "PxRigidDynamic.h"
#include <iostream>
#include <glut.h>
#include "../Game.h"
using namespace physx;

namespace Snippets
{
Camera::Camera(const PxVec3& eye, const PxVec3& dir)
{
	mEye = eye;
	mDir = dir.getNormalized();
	mMouseX = 0;
	mMouseY = 0;
}

void Camera::handleMouse(int button, int state, int x, int y)
{
	PX_UNUSED(state);
	PX_UNUSED(button);
	if (game != nullptr && !game->getControlsActive())return;
	mMouseX = x;
	mMouseY = y;
	game->seeMouseInput(button, state, x, y);
}

bool Camera::handleKey(unsigned char key, int x, int y, float speed)
{
	PX_UNUSED(x); //si pulso alguna de las teclas de switch
	PX_UNUSED(y);
	if (!game->getControlsActive())return false;
	PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
	switch (toupper(key)) {

	case 'W':if (follow) { foco -= 0.5; foco = max(foco, minFoco); }else	mEye += mDir * 2.0f * speed;		break;
	case 'S':if (follow) { foco += 0.5;	foco = min(foco, maxFoco); }else	mEye -= mDir * 2.0f * speed;		break;
	case 'A':if(!follow)	mEye -= viewY * 2.0f * speed;		break;
	case 'D':if(!follow)	mEye += viewY * 2.0f * speed;		break;
	default:							return false;
	}

	return true;
}

void Camera::handleAnalogMove(float x, float y)
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();
	mEye += mDir*y;
	mEye += viewY*x;
}

void Camera::handleMotion(int x, int y)
{
	if (game != nullptr && !game->getControlsActive())return;
	int dx = mMouseX - x;
	int dy = mMouseY - y;

	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();

	PxQuat qx(PxPi * dx / 180.0f, PxVec3(0,1,0));
	mDir = qx.rotate(mDir);
	
	PxQuat qy(PxPi * dy / 180.0f, viewY);
	mDir = qy.rotate(mDir);
	mDir.normalize();
	

	mMouseX = x;
	mMouseY = y;
}

PxTransform Camera::getTransform() const
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0));

	if(viewY.normalize()<1e-6f) 
		return PxTransform(mEye);

	PxMat33 m(mDir.cross(viewY), viewY, -mDir);
	return PxTransform(mEye, PxQuat(m));
}

void Camera::update(){
	if (follow) {
		Vector3 dir = mDir; dir.y = 0; dir.normalize();
		PxVec3 relativePos = golfBall->getGlobalPose().p;
		relativePos.y += foco-foco/3;
		relativePos -= dir * foco;
		mEye = relativePos;
		//cout << "x: " << mDir.x << " y: " << mDir.y << " z: " << mDir.z << "\n";
	}
	
}

PxVec3 Camera::getEye() const
{ 
	return mEye; 
}

PxVec3 Camera::getDir() const
{ 
	return mDir; 
}

}

