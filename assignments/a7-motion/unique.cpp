#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "dancer.h"
using namespace atk;
using glm::vec3;
using glm::quat;

class Unique : public atkui::Framework {
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~Unique() {}

   virtual void setup() {
      

      BVHReader reader;
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _motion);

      for(int i=0;i<3;i++) {
         for(int j =0; j<4;j++) {
            if(j %2 == 0){
               pos = vec3(i*200+agl::random()*20,0,j*200+agl::random()*20);
            } else {
               pos = vec3(-i*200+agl::random()*20,0,-j*200+agl::random()*20);
            }
            color = vec3(agl::random(),agl::random(),agl::random());
            size = agl::random()+0.25;
            dancers.push_back(Dancer(pos, color, size));
         }
      }
   }

   virtual void scene() {
      
      for(int i=0;i<12;i++) {
         if (!_paused) _motion.update(_skeleton, elapsedTime()+i*2);
         dancers[i].draw(_skeleton, *this);
      }
     
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

 

protected:
   int numJoints;
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Dancer _dancer;
   std::vector<Dancer> dancers;

   vec3 pos;
   vec3 color;
   float size;

};

int main(){
   Unique unique;
   unique.run();
}
