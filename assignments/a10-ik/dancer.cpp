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

      _lhandPos = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();
      _rhandPos = _skeleton.getByName("Beta:RightHand")->getGlobalTranslation();
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());
      IKController ik;
      //follow the hand target
      Joint* LH = _skeleton.getByName("Beta:LeftHand");
      Joint* RH = _skeleton.getByName("Beta:RightHand");
      ik.solveIKAnalytic(_skeleton, LH->getID(), _lhandTarget, 0.001f);
      ik.solveIKAnalytic(_skeleton, RH->getID(), _rhandTarget, 0.001f);

      //animating the hips
      Joint* hip = _skeleton.getRoot();
      vec3 hipPos = hip->getGlobalTranslation();
      hip->setLocalTranslation(hipPos + vec3())
      
      
   }

   void scene()
   {
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0, 0, 1));
      _lhandTarget = _lhandPos + vec3(-40, 80 + 40.0f * sin(elapsedTime()), 20);
      _rhandTarget = _rhandPos + vec3(40, 80 + 40.0f * sin(elapsedTime()+2.0f), 20);
      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);
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
};

int main(int argc, char **argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}
