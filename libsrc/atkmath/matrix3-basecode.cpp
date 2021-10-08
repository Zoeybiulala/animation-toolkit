#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

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
      // TODO
   }

   void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
   {
      // TODO
      *this = Identity;
   }

   
}

