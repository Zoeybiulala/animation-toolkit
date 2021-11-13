#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   //returns a new motion such that the lower body matches the motion in "lower", 
   //the upper body is the result of blending the upper body motion with
   //the original's upper body motion, e.g. new upper = upper * (1-alpha) + lower*alpha
   Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
   {
       Motion result;
      result.setFramerate(lower.getFramerate());
      //BFS for the subtree
      Joint* curr = _skeleton.getByName("Beta:Spine1");
      std::deque<int> q;
      std::vector<int> ids;
      q.push_back(curr->getID());
      while(!q.empty()) { 
          curr = _skeleton.getByID(q.front());
          q.pop_front();
          ids.push_back(curr->getID());
          for (int i = 0; i < curr->getNumChildren(); i++) {
              q.push_back(curr->getChildAt(i)->getID());
          }
      }
      for (int i = 0; i < lower.getNumKeys(); i++) {
          Pose lowerPose = lower.getKey(i);
          Pose upperPose = upper.getKey(i+110);

          for (int j = 0; j < ids.size(); j++) {
              lowerPose.jointRots[ids[j]] =
               glm::slerp(upperPose.jointRots[ids[j]], lowerPose.jointRots[ids[j]], alpha);
          }
          result.appendKey(lowerPose);
      }
      return result;
   }

   void scene()
   {  
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: "+std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha+0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha-0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char** argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}

