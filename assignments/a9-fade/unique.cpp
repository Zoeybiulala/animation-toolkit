#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    _drawer.showAxes = true;
    pos = vec3(0);
  }

  virtual void scene()
  {
    update();
    _drawer.draw(_skeleton, *this);

    // draw heading
    vec3 p = _skeleton.getRoot()->getGlobalTranslation();
    p[1] = 10; // set height close above the floor

    setColor(vec3(0, 1, 1));
    push();
    translate(p);
    rotate(_heading, vec3(0, 1, 0));
    translate(vec3(0,0,25));
    scale(vec3(10, 10, 50));
    drawCylinder(vec3(0), 1.0f);
    pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());

    // TODO: Your code 
    
    Pose curr = _skeleton.getPose();
    //pin the skeleton at first key
    curr.rootPos = vec3(0,curr.rootPos.y,0) + pos;
    pos += vec3(2 * sin(_heading),0,2*cos(_heading));
    //rotate with heading
    curr.jointRots[0] *= glm::angleAxis(_heading, vec3(0, 1, 0));
    _skeleton.setPose(curr);

    // TODO: Override the default camera to follow the character
    globalLookPos = _skeleton.getByName("Beta:Head")->getGlobalTranslation();
    globalPos = globalLookPos + vec3(-250*sin(_heading),100,-250*cos(_heading));
    lookAt(globalPos, globalLookPos, vec3(0, 1, 0));

    // update heading when key is down
    if (keyIsDown('D')) _heading -= 0.05;
    if (keyIsDown('A')) _heading += 0.05;
    if (keyIsDown('1')) {
       setColor(vec3(1,0,1));
      Joint* pParent = _skeleton.getByName("Beta:RightHandPinky1")->getParent();
      Transform xform = pParent->getLocal2Global();
      drawSphere(xform.transformPoint(vec3(0,0,5)),20);

      pParent = _skeleton.getByName("Beta:LeftHandPinky1")->getParent();
      xform = pParent->getLocal2Global();
      drawSphere(xform.transformPoint(vec3(0,0,-5)),20);
    }

    if (keyIsDown('2')) {
       setColor(vec3(1,1,0));
      Joint* pParent = _skeleton.getByName("Beta:RightToeBase")->getParent();
      Transform xform = pParent->getLocal2Global();
      drawCylinder(xform.transformPoint(vec3(0,0,5)),20);

      pParent = _skeleton.getByName("Beta:LeftToeBase")->getParent();
      xform = pParent->getLocal2Global();
      drawCylinder(xform.transformPoint(vec3(0,0,-5)),20);
    }
  }

protected:
  float _heading;

  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;

  vec3 globalPos;
  vec3 globalLookPos;
  vec3 pos;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
