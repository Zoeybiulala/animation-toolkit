#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;
using glm::quat;

class Tentacle : public atkui::Framework {
public:
   Tentacle() : atkui::Framework(atkui::Perspective) {}
   virtual ~Tentacle() {}

   virtual void setup() {
      lookAt(vec3(200), vec3(0));

      Joint* root  = new Joint("root");
      root->setLocalTranslation(vec3(0,0,0));
      _tentacle.addJoint(root);

      Joint* joint1  = new Joint("joint1");
      joint1->setLocalTranslation(vec3(50,0,0));
      _tentacle.addJoint(joint1, root);

      Joint* joint2  = new Joint("joint2");
      joint2->setLocalTranslation(vec3(50,0,0));
      _tentacle.addJoint(joint2, joint1);

      Joint* joint3  = new Joint("joint3");
      joint3->setLocalTranslation(vec3(50,0,0));
      _tentacle.addJoint(joint3, joint2);

      Joint* joint4  = new Joint("joint4");
      joint4->setLocalTranslation(vec3(50,0,0));
      _tentacle.addJoint(joint4, joint3);

      Joint* joint5  = new Joint("joint5");
      joint5->setLocalTranslation(vec3(50,0,0));
      _tentacle.addJoint(joint5, joint4);

      _tentacle.fk(); // compute local2global transforms
   }

   virtual void scene()
   {
      // todo animate joints
      // hint: use glm::angleAxis(...) to compute quaternions for each joint

      _tentacle.fk(); // computes local2global transforms
      setColor(vec3(1,1,0));

      // todo: loop over all joints and draw
      // std::cout<<_tentacle.getNumJoints()<<std::endl;
      for(int i = 0; i< _tentacle.getNumJoints();i++){
         Joint* currJoint = _tentacle.getByID(i);
         quat local_rot = glm::angleAxis(cos(elapsedTime()*i),vec3(0,0,1));
         currJoint->setLocalRotation(local_rot);

         if(currJoint->getParent() == NULL){ continue; }
         Joint* p = currJoint->getParent();
         vec3 p1 = p->getGlobalTranslation();
         vec3 p2 = currJoint->getGlobalTranslation();
         drawEllipsoid(p1,p2,5);
      }
   }

protected:
   Skeleton _tentacle;
};

int main(int argc, char** argv)
{
   Tentacle viewer;
   viewer.run();
} 

