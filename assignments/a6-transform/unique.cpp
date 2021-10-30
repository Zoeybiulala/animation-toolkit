#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;
using glm::quat;

class Unique : public atkui::Framework {
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   virtual void setup() {
      

      BVHReader reader;
      reader.load("../motions/Beta/jump.bvh", _person, _motion1);
      reader.load("../motions/Beta/samba_dancing.bvh", _person, _motion2);
      reader.load("../motions/Beta/gangnam_style.bvh", _person, _motion3);

      numJoints = _person.getNumJoints();
      _person.fk();
   }

   virtual void scene() {
      _person.fk(); 
     
      setColor(vec3(1,1,0));
      if(elapsedTime()<5) {
         _motion1.update(_person, elapsedTime(), true);
      } else if(elapsedTime()<10){
         _motion3.update(_person, elapsedTime(), true);
      } else {
         _motion2.update(_person,elapsedTime(),true);
      }
      
      
      

      // todo: loop over all joints and draw
      for(int i = 0; i< numJoints;i++){
         Joint* currJoint = _person.getByID(i);

         if(currJoint->getParent() == NULL){ continue; }
         Joint* p = currJoint->getParent();
         vec3 p1 = p->getGlobalTranslation();
         vec3 p2 = currJoint->getGlobalTranslation();
         drawEllipsoid(p1,p2,5);
      }

     
   }

 

protected:
   Skeleton _person;
   Motion _motion1;
   Motion _motion2;
   Motion _motion3;
   int numJoints;

};

int main(){
   Unique unique;
   unique.run();
}