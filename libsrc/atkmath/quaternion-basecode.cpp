#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath {

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
{
	// TODO
	return Quaternion(1,0,0,0);
}

void Quaternion::toAxisAngle (Vector3& axis, double& angleRad) const
{	
	angleRad = acos(mW)*2;
	axis[0] = mX/sin(angleRad/2);
	axis[1] = mY/sin(angleRad/2);
	axis[2] = mZ/sin(angleRad/2);
   axis.normalize();
}

void Quaternion::fromAxisAngle (const Vector3& axis, double angleRad)
{	
	float t = (float) angleRad/2;
	Vector3 norm = axis.normalized();
	mX = norm[0] * sin(t);
	mY = norm[1] * sin(t);
	mZ = norm[2] * sin(t);
	mW = cos(t);
}

Matrix3 Quaternion::toMatrix () const
{	
	float a11,a12,a13,a21,a22,a23,a31,a32,a33;
	a11 = 1-2*(mY*mY+mZ*mZ);
	a12 = 2*(mX*mY-mW*mZ);
	a13 = 2*(mX*mZ+mW*mY);
	a21 = 2*(mX*mY+mW*mZ);
	a22 = 1-2*(mX*mX+mZ*mZ);
	a23 = 2*(mY*mZ-mW*mX);
	a31 = 2*(mX*mZ-mW*mY);
	a32 = 2*(mY*mZ+mW*mX);
	a33 = 1-2*(mX*mX+mY*mY);
	Matrix3 temp = Matrix3(a11,a12,a13,a21,a22,a23,a31,a32,a33);
	return temp;
}

void Quaternion::fromMatrix(const Matrix3& rot)
{
	  float wSquare = 0.25 * (rot[0][0] + rot[1][1] + rot[2][2] + 1);
      float xSquare = 0.25 * (1 + rot[0][0] - rot[1][1] - rot[2][2]);
      float ySquare = 0.25 * (1 -rot[0][0] + rot[1][1] - rot[2][2]);
      float zSquare = 0.25 * (1 -rot[0][0] - rot[1][1] + rot[2][2]);
      float max = std::max(std::max(wSquare,xSquare),std::max(ySquare,zSquare));
      float wx = 0.25 * (rot[2][1]-rot[1][2]);
      float wy = 0.25 * (rot[0][2]-rot[2][0]);
      float wz = 0.25 * (rot[1][0]-rot[0][1]);
      float xy = 0.25 * (rot[1][0]+rot[0][1]);
      float xz = 0.25 * (rot[0][2]+rot[2][0]);
      float yz = 0.25 * (rot[1][2]+rot[2][1]);

      if(max == wSquare) {
         mW = sqrt(wSquare);
         mX = wx/mW;
         mY = wy/mW;
         mZ = wz/mW;
      } else if(max == xSquare) {
         mX = sqrt(xSquare);
         mW = wx/mX;
         mY = xy/mX;
         mZ = xz/mX;
      } else if(max == ySquare){ 
         mY = sqrt(ySquare);
         mW = wy/mY;
         mX = xy/mY;
         mZ = yz/mY;
      } else{
         mZ = sqrt(zSquare);
         mW = wz/mZ;
         mX = xz/mZ;
         mY = yz/mZ;
      }
}

}