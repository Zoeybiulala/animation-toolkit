#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton& skeleton, 
    int jointid, const vec3& goalPos, float epsilon) {
  if (jointid == -1) return true;

  Joint* ankle = skeleton.getByID(jointid);
  if (!ankle->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* knee = ankle->getParent();
  if (!knee->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* hip = knee->getParent();

  // TODO: Your code here
  //from twolink
  Joint* endEff = skeleton.getByID(jointid);
  Joint* jointPar = endEff->getParent();
  Joint* jointGran = jointPar->getParent();

  float l1 = length(endEff->getLocalTranslation());
  float l2 = length(jointPar->getLocalTranslation());
  float r = length(goalPos - jointGran->getGlobalTranslation());

  float cos_phi = clamp((r * r - l1 * l1 - l2 * l2) / (-2.0f * l1 * l2),-1.0f,1.0f);
  float theta2z = acos(cos_phi)-pi<float>(); //in radian

  vec3 limbDir = normalize(jointPar->getLocalTranslation());
  vec3 axis = cross(limbDir, vec3(0,0,-1));
  if (limbDir[1] < 0) axis = cross(limbDir, vec3(0,0,1));

  quat Rz_theta2z = angleAxis(theta2z, axis);
  Transform F21(Rz_theta2z, jointPar->getLocalTranslation());
  jointPar->setLocal2Parent(F21);
  jointGran->setLocalRotation(IdentityQ);
  skeleton.fk();
  
  vec3 pos = endEff->getGlobalTranslation();
  vec3 r1 = pos - jointGran->getGlobalTranslation();
  vec3 e1 = goalPos - pos;
  if(length(e1)<epsilon){
    return false;
  }

  axis = normalize(cross(r1,e1));
  if (length(cross(r1,e1)) < epsilon) {
      return false;
  }
  float angle = atan2((double)length(cross(r1,e1)),(double)(dot(r1,r1)+dot(r1,e1)));

  axis = jointGran->getLocal2Global().inverse().transformVector(axis);
  quat rot = angleAxis(angle, axis);
  jointGran->setLocalRotation(rot * jointGran->getLocalRotation());
  

  skeleton.fk();
  
  return true;
}

bool IKController::solveIKCCD(Skeleton& skeleton, int jointid, 
    const vec3& goalPos, const std::vector<Joint*>& chain, 
    float threshold, int maxIters, float nudgeFactor) {
  // There are no joints in the IK chain for manipulation
  if (chain.size() == 0) return true;

  //TODO: Your code here
  Joint* endEff = skeleton.getByID(jointid);
  vec3 pos = endEff->getGlobalTranslation();
  int i = 0; 

  while(distance(goalPos,pos) > threshold && i< maxIters){
    for (int i = 1; i < chain.size(); i++) { //exclude the end effector
      Joint * curr = chain[i];
      vec3 currPos = curr->getGlobalTranslation();

      vec3 e = goalPos - pos;
      if(length(e) < 0.0000001){
        continue;
      }
      vec3 r = pos - currPos;
      vec3 axis = normalize(cross(r,e));//global
      if (length(cross(r,e)) < 0.000001) {
        continue;
      }
      float nudge = nudgeFactor * atan2((double)length(cross(r,e)),(double)(dot(r,r)+dot(r,e)));
      axis = curr->getLocal2Global().inverse().transformVector(axis);
      quat rot = angleAxis(nudge, axis);
      curr->setLocalRotation(rot * curr->getLocalRotation());//only curr also don't work

      skeleton.fk();
      pos = endEff->getGlobalTranslation();
    }
    i++;
  }

  return false;
}
