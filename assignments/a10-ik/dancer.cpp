#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _drawer.color = vec3(1, 0, 0);
      _motion.update(_skeleton,0);

      _lhandPos = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();
      _rhandPos = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation();

      _lFootPos = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();
      _rFootPos = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
      _lRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
      _rRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();
      _hipPos = _skeleton.getRoot()->getGlobalTranslation() - vec3(5.0f,5.0f,0); //so the knee will bend
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());
      IKController ik;
      //follow the hand target
      Joint* LH = _skeleton.getByName("Beta:LeftHand");
      Joint* RH = _skeleton.getByName("Beta:RightHand");
      ik.solveIKAnalytic(_skeleton,LH->getID(),_lhandTarget);
      ik.solveIKAnalytic(_skeleton,RH->getID(),_rhandTarget);
      
      //animating the hips
      Joint* hip = _skeleton.getRoot();
      hip->setLocalTranslation(_hipPos + vec3(7.0f * cos(5.0f * elapsedTime()), 7.0f * abs(sin(5.0f * elapsedTime())), 0));
      _skeleton.fk();

      //pin the feet
      Joint* LF = _skeleton.getByName("Beta:LeftFoot");
      Joint* RF = _skeleton.getByName("Beta:RightFoot");
      ik.solveIKAnalytic(_skeleton, LF->getID(), _lFootPos);
      ik.solveIKAnalytic(_skeleton, RF->getID(), _rFootPos);
      LF->setLocalRotation(LF->getLocalRotation()*inverse(LF->getGlobalRotation()) *_lRot);
      RF->setLocalRotation(RF->getLocalRotation()*inverse(RF->getGlobalRotation()) *_rRot);
      _skeleton.fk();
      
      
      
   }

   void scene()
   {
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0, 0, 1));
      _lhandTarget = _lhandPos + vec3(10, 80 + 40.0f * sin(5.0f* elapsedTime()), 20);
      _rhandTarget = _rhandPos + vec3(-10, 80 + 40.0f * sin(5.0f*elapsedTime()+2.0f),20);
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;

   // original hand postions
   vec3 _lhandPos;
   vec3 _rhandPos;

   //hip
   vec3 _hipPos;

   // feet
   vec3 _lFootPos;
   vec3 _rFootPos;
   quat _lRot;
   quat _rRot;
};

int main(int argc, char **argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}
