#ifndef DANCER_H_
#define DANCER_H_

#include "atkui/framework.h"

using namespace atk;
using namespace glm;

class Dancer
{
public:
   Dancer() {}
   Dancer(const vec3& p, const vec3& c, float s) {
      _pos = p;
      _size = vec3(s);
      _color = c;
   }
   virtual ~Dancer() {}

   void draw(const Skeleton& skeleton, atkui::Framework& f) {

      f.push();
      f.translate(_pos);
      f.scale(_size);
      f.setColor(_color);

      float jointRadius = 15;
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;
         if (joint->getParent()->getName().find("Hand") != std::string::npos) continue; // skip hands

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         f.drawEllipsoid(p1, p2, jointRadius);
         f.drawSphere(p2, jointRadius);
         f.drawSphere(p1, jointRadius);
      }

      Joint* head = skeleton.getByName("Beta:Head");
      assert(head);

      // Use joint's global position and rotation to convert 
      // from local head coordinates to global coordinates
      Joint* pParent = head->getParent();
      Transform xform = pParent->getLocal2Global();

      float headLength = glm::distance(
         pParent->getGlobalTranslation(), 
         head->getGlobalTranslation());

      // eyes
      float pupilSize = jointRadius * 0.30;
      f.setColor(vec3(0));

      vec3 pupilPosL(jointRadius*0.3, headLength * 0.75, pupilSize); 
      f.drawSphere(xform.transformPoint(pupilPosL), pupilSize);

      vec3 pupilPosR(-jointRadius*0.3, headLength * 0.75, pupilSize); 
      f.drawSphere(xform.transformPoint(pupilPosR), pupilSize);

      //mouth
      float mouthSize = jointRadius * 0.6;
      f.setColor(vec3(1,0,0));

      vec3 mouthPos(jointRadius*0.3, headLength * 0.5,mouthSize);
      f.drawCube(xform.transformPoint(mouthPos),vec3(mouthSize,jointRadius*0.1,0));

      // hat
      f.setColor(vec3(0,1,0));
      float hs = jointRadius;
      vec3 hatPos(jointRadius*0.4, 10*headLength, -10);
      mat3 hatRot = eulerAngleRO(ZYX, glm::radians(vec3(-90, 0, -10)));

      f.push();
      f.transform(xform*Transform(hatRot, hatPos));
      f.scale(vec3(1.5*hs,1*hs,2*hs));
      f.drawCone(vec3(0), 1.0f);
      f.pop();

      // pitch fork
      Joint* hand = skeleton.getByName("Beta:RightHand");
      assert(hand);
      xform = hand->getLocal2Global();

      hs = jointRadius*0.25;
      float len = 100;
      f.setColor(vec3(0,0,0));
      vec3 spearPos(0, 0, 0);
      mat3 spearRot = eulerAngleRO(ZYX, glm::radians(vec3(0, 0, 0)));

      f.push();
      f.transform(xform*Transform(spearRot, spearPos));
      f.scale(vec3(hs,hs,len));
      f.drawCylinder(vec3(0), 1.0f);
      f.pop();

      f.pop();
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   vec3 _color = vec3(1.0,0.0,0.0);
   vec3 _size = vec3(1.0);
   vec3 _pos = vec3(0);

};

#endif
