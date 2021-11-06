#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;
using std::cout;
using std::endl;
using std::string;

class MotionViewer : public atkui::Framework
{
public:
   string f;
   MotionViewer(string file) : atkui::Framework(atkui::Perspective) {
       f = file;
   }

   void setup() {
      BVHReader reader;
      reader.load(f, skeleton, motion);
      motion.update(skeleton, 0);
      fps = motion.getFramerate();
   }

   void scene() {
      if(paused == false) {
         time += dt() * timeScale;
      }
      motion.update(skeleton, time);
      currentFrame = motion.getKeyID(time);

      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      drawText(paused? "Paused" : "Playing", 10, 15);
      drawText("Current frame: "+std::to_string(currentFrame), 10, 35);
      drawText("Time scale: "+std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods) {
      if(key == GLFW_KEY_P){
         paused = !paused;
      }
      else if(key == GLFW_KEY_0){
         time = 0.0f;
      }
      else if(key == GLFW_KEY_PERIOD){
         time += (1.0f/fps);
      }
      else if(key == GLFW_KEY_COMMA){
         time -= (1.0f/fps);
      }
      else if(key == GLFW_KEY_LEFT_BRACKET){
         timeScale -= 0.5f;
      }
      else if(key == GLFW_KEY_RIGHT_BRACKET){
         timeScale += 0.5f;
      }
   }


private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0; 
   bool paused = false;
   float time = 0;
   int fps;
};


int main(int argc, char** argv) {

   if(argc==1) {
      MotionViewer viewer("../motions/Beta/jump.bvh"); 
      viewer.run();
   } else if (argc==2) {
      MotionViewer viewer(argv[1]);
      viewer.run();
   } else {
      cout<<"invalid command line input number."<<endl;
   }
   
}
