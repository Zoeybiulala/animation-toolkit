#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup() {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 pos = vec3(0);
      vec3 color = vec3(1,0,0);
      float size = 1.0f;
      for(int i=0;i<3;i++) {
         for(int j =0; j<4;j++) {
            pos = vec3(i*200+agl::random()*20,0,j*200+agl::random()*20);
            color = vec3(agl::random(),agl::random(),agl::random());
            size = agl::random()+0.25;
            devils.push_back(Devil(pos, color, size));
         }
      }
      
   }

   virtual void scene() {
      if (!_paused) _motion.update(_skeleton, elapsedTime());
      for(Devil devil: devils) {
         devil.draw(_skeleton, *this);
      }
      
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Devil _devil;
   std::vector<Devil> devils;
};

int main(int argc, char** argv) {
   Thriller viewer;
   viewer.run();
}

