#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include <algorithm>    // std::max

using namespace glm;
namespace atkmath {
   Matrix3 Rz(double beta){
      return Matrix3(cos(beta),-sin(beta),0.0,sin(beta),cos(beta),0.0,0.0,0.0,1.0);
   }

   Matrix3 Ry(double theta){
      return Matrix3(cos(theta),0.0,sin(theta),0.0,1.0,0.0,-sin(theta),0.0,cos(theta));
   }

   Matrix3 Rx(double alp){
      return Matrix3(1.0,0.0,0.0,0.0,cos(alp),-sin(alp),0.0,sin(alp),cos(alp));
   }
   double alp,beta,theta;


   Vector3 Matrix3::toEulerAnglesXYZ() const
   {  
      theta = asin(m13);
      if(m13 == 1) {
         alp = 0.0;
         beta = atan2(m32,m22);
      } else if(m13 ==-1) {
         alp = 0.0;
         beta = -atan2(m32,m22);
      } else {
         alp = atan2(-m23,m33);
         beta = atan2(-m12,m11);
      }
     
      return Vector3(alp,theta,beta);
   }

   Vector3 Matrix3::toEulerAnglesXZY() const
   {  
      beta = asin(-m12);
      if(m12 == 1) {
         theta = 0.0;
         alp = -atan2(m23,m33);
      } else if(m12 == -1) {
         theta = 0.0;
         alp = atan2(m31,m21);
      } else {
         alp = atan2(m32,m22);
         theta = atan2(m13,m11);
      }
      return Vector3(alp,theta,beta);
   }

   Vector3 Matrix3::toEulerAnglesYXZ() const
   {  
      alp = asin(-m23);
      if(m23 == 1) {
         beta = 0.0;
         theta = atan2(-m12, m11);
      } else if(m23 == -1) {
         beta = 0.0;
         theta = atan2(m12,m11);
      } else {
         beta = atan2(m21,m22);
         theta = atan2(m13,m33);
      }
      return Vector3(alp,theta,beta);
   }

   Vector3 Matrix3::toEulerAnglesYZX() const
   {  
      beta = asin(m21);
      if(m21 == 1){
         alp = atan2(m32,m33);
         theta = 0.0;
      } else if(m21 == -1) {
         alp = atan2(m32,m33);
         theta = 0.0;
      } else {
         alp = atan2(-m23,m22);
         theta = atan2(-m31,m11);
      }
      return Vector3(alp,theta,beta);
   }

   Vector3 Matrix3::toEulerAnglesZXY() const
   {  
      alp = asin(m32);
      if(m32 ==1) {
         beta = 0.0;
         theta = atan2(m21, m11);
      } else if(m32 == -1) {
         beta = 0.0;
         theta = atan2(-m21,m11);
      } else {
         theta = atan2(-m31,m33);
         beta = atan2(-m12,m22);
      }
      return Vector3(alp,theta,beta);
   }

   Vector3 Matrix3::toEulerAnglesZYX() const
   {  
      theta = asin(-m31);
      if(m31 == 1) {
         beta = 0.0;
         alp = atan2(-m12,m22);
      } else if(m31 == -1) {
         beta = 0.0;
         alp = atan2(m12,m22);
      } else {
         beta = atan2(m21,m11);
         alp = atan2(m32,m33);
      }
      return Vector3(alp,theta,beta);
   }

   void Matrix3::fromEulerAnglesXYZ(const Vector3& angleRad)
   {
      *this = Rx(angleRad[0])*Ry(angleRad[1])*Rz(angleRad[2]);
   }

   void Matrix3::fromEulerAnglesXZY(const Vector3& angleRad)
   {
      *this = Rx(angleRad[0])*Rz(angleRad[2])*Ry(angleRad[1]);
   }

   void Matrix3::fromEulerAnglesYXZ(const Vector3& angleRad)
   {
      *this = Ry(angleRad[1])*Rx(angleRad[0])*Rz(angleRad[2]);
   }

   void Matrix3::fromEulerAnglesYZX(const Vector3& angleRad)
   {
      *this = Ry(angleRad[1])*Rz(angleRad[2])*Rx(angleRad[0]);
   }

   void Matrix3::fromEulerAnglesZXY(const Vector3& angleRad)
   {  
      
      *this = Rz(angleRad[2])*Rx(angleRad[0])*Ry(angleRad[1]);
   }

   void Matrix3::fromEulerAnglesZYX(const Vector3& angleRad)
   {
      *this = Rz(angleRad[2])*Ry(angleRad[1])*Rx(angleRad[0]);
   }

   void Matrix3::toAxisAngle(Vector3& axis, double& angleRad) const
   {
      float wSquare = 0.25 * (m11 + m22 + m33 + 1);
      float xSquare = 0.25 * (1 + m11 - m22 - m33);
      float ySquare = 0.25 * (1 - m11 + m22 - m33);
      float zSquare = 0.25 * (1 - m11 - m22 + m33);
      float max = std::max(std::max(wSquare,xSquare),std::max(ySquare,zSquare));
      float wx = 0.25 * (m32-m23);
      float wy = 0.25 * (m13-m31);
      float wz = 0.25 * (m21-m12);
      float xy = 0.25 * (m21+m12);
      float xz = 0.25 * (m13+m31);
      float yz = 0.25 * (m23+m32);
      float x,y,z,w  = 0.0;
      if(max == wSquare) {
         w = sqrt(wSquare);
         x = wx/w;
         y = wy/w;
         z = wz/w;
      } else if(max == xSquare) {
         x = sqrt(xSquare);
         w = wx/x;
         y = xy/x;
         z = xz/x;
      } else if(max == ySquare){ 
         y = sqrt(ySquare);
         w = wy/y;
         x = xy/y;
         z = yz/y;
      } else{
         z = sqrt(zSquare);
         w = wz/z;
         x = xz/z;
         y = yz/z;
      }

      angleRad = acos(w)/2;
      axis[0] = x/sin(angleRad/2);
      axis[1] = y/sin(angleRad/2);
      axis[2] = z/sin(angleRad/2);

   }

   void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
   {  
      float x = axis[0];
      float y = axis[1];
      float z = axis[2];
      float c = cos(angleRad);
      float s = sin(angleRad);
      *this = Matrix3(c+(1-c)*x*x, -s*z + (1-c)*x*y, y * s + x*z*(1-c),
                           -z*s + (1-c)*x*y, c+ (1-c)*y*y, -x*s+(1-c) *y*z,
                           s*y + (1-c) *x*y, -x*s+(1-c)*y*z, c+ (1-c) *z *z);
   }

   
}

