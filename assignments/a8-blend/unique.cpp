#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <list> //for iterator
#include <iterator> //for iterator

using namespace glm;
using namespace atk;
using namespace atkui;

class Particle {
   public:
   vec3 pos;
   float size = 20.0f;
   float _t;
   //constructor
   Particle(vec3 p){
      pos = p;
   }
   Particle(){}
};

class Unique : public atkui::Framework
{
public:
   Unique() : atkui::Framework(atkui::Perspective) {}
   

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _motion);

      _heading = 0;
      _offset = vec3(0);
      _offset[1] = _motion.getKey(0).rootPos[1];
      _time = 0;

      _reoriented = reorient(_motion, _offset, _heading);
   }

   //heading is the angle around y axis
   Motion reorient(const Motion& motion, const vec3& pos, float heading)
   {
      Motion result;
      result.setFramerate(motion.getFramerate());

      // todo: your code here
      Pose curr;
      for(int i=0; i<motion.getNumKeys(); i++){
         curr = motion.getKey(i);
         curr.jointRots[0] = eulerAngleRO(XYZ, vec3(0, heading, 0)); //root rotation
         vec3 offset = curr.rootPos * eulerAngleRO(XYZ,vec3(0,-heading,0));
         curr.rootPos = pos + offset;//root positiaon
         result.appendKey(curr);
      }
      
      return result;
   }

   void update()
   {  
      _time += dt() * 0.5;
      _reoriented.update(_skeleton, _time);
      // Joint* leftFoot = _skeleton.getByName("Beta:LeftFoot")
      // Particle temp = Particle()
      lst.push_back(Particle(_skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation()));
      if(lst.size()==200){
         lst.pop_front();
      }
      lstt.push_back(Particle(_skeleton.getByName("Beta:RightFoot")->getGlobalTranslation()));
      if(lstt.size()==200){
         lstt.pop_front();
      }
   }

   void scene()
   {  
      update();

      setColor(vec3(1,0,0));
      std::list<Particle>::reverse_iterator it;
      for(it = lst.rbegin();it !=lst.rend();it++){ 
         Particle cur = *it;
         drawSphere(cur.pos,10*agl::random());
      }

      setColor(vec3(1,1,0));
      for(it = lstt.rbegin();it !=lstt.rend();it++){ 
         Particle cur = *it;
         drawSphere(cur.pos,10*agl::random());
      }
      
      
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_LEFT)
      {
         _heading += M_PI/8;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
      }
      else if (key == GLFW_KEY_RIGHT)
      {
         _heading -= M_PI/8;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
      }

      if (key == 'W')
      {
         _offset[2] += 25;
         _reoriented = reorient(_motion, _offset, _heading);
         std::cout << _offset << std::endl;
         _time = 0;
      }
      else if (key == 'S')
      {
         _offset[2] -= 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
      else if (key == 'A')
      {
         _offset[0] += 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
      else if (key == 'D')
      {
         _offset[0] -= 25;
         _reoriented = reorient(_motion, _offset, _heading);
         _time = 0;
         std::cout << _offset << std::endl;
      }
   }

   Skeleton _skeleton;
   Motion _motion;
   Motion _reoriented;
   vec3 _offset;
   float _heading;
   float _time;
   std::list<Particle> lst;
   std::list<Particle> lstt;
};

int main(int argc, char** argv)
{
   Unique viewer;
   viewer.run();
   return 0;
}

