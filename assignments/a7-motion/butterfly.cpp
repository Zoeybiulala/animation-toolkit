#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      Joint* body = new Joint("Body");
      body->setLocalTranslation(vec3(1,2,1)*100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0,1,0)));
      skeleton.addJoint(body);

      Joint* lwing = new Joint("LWing");
      lwing->setLocalTranslation(vec3(0.1,0,0)*100.0f);
      skeleton.addJoint(lwing, body);

      Joint* rwing = new Joint("RWing");
      rwing->setLocalTranslation(vec3(-0.1,0,0)*100.0f);
      skeleton.addJoint(rwing, body);

      skeleton.fk();
   }

   void scene() {
      Joint* body = skeleton.getByName("Body");
      body->setLocalTranslation(vec3(100,cos(elapsedTime()/2)*100+200,100));

      Joint* lwing = skeleton.getByName("LWing");
      lwing->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0,0,1)));

      Joint* rwing = skeleton.getByName("RWing");
      rwing->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0,0,1))); 
      skeleton.fk();

      // attach geometry to skeleton 
      Transform B = body->getLocal2Global(); 
      Transform LT = lwing->getLocal2Global(); 
      Transform RT = rwing->getLocal2Global(); 

      // draw body
      Transform bodyGeometry(
         glm::angleAxis(glm::pi<float>()*0.5f, vec3(1,0,0)), // rotation
         vec3(0), vec3(25, 200, 25)); // position, scale

      Transform lwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,0), 
         vec3(120,20,200));

      Transform rwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,0), 
         vec3(120,20,200));

      push();
      transform(B);
      if(axis) {
          setColor(vec3(1, 0, 0));
          drawEllipsoid(vec3(0, 0, 0), vec3(200, 0, 0), 10);
      
          setColor(vec3(0, 1, 0));
          drawEllipsoid(vec3(0, 0, 0), vec3(0, 200, 0), 10);
     
          setColor(vec3(0, 0, 1));
          drawEllipsoid(vec3(0, 0, 0), vec3(0, 0, 200), 10);
      }
      pop();

      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1); //draw body
      pop();

      setColor(vec3(0.8, 0, 0.0));
      push();
         transform(LT * lwingGeometry);
         push();
            transform(Transform(eulerAngleRO(XYZ,vec3(0,0,0)),
            vec3(0,0,0.5),vec3(1)));
            drawSphere(vec3(0),1);
         pop();
         push();
            transform(Transform(eulerAngleRO(XYZ,vec3(0,0,0)),
            vec3(0,0,-0.2),vec3(1)));
            drawSphere(vec3(0),0.75);
         pop();
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RT * rwingGeometry);
      push();
            transform(Transform(eulerAngleRO(XYZ,vec3(0,0,0)),
            vec3(0,0,0.5),vec3(1)));
            drawSphere(vec3(0),1);
         pop();
         push();
            transform(Transform(eulerAngleRO(XYZ,vec3(0,0,0)),
            vec3(0,0,-0.2),vec3(1)));
            drawSphere(vec3(0),0.75);
         pop();
      pop();
   }

   virtual void keyUp(int key, int mods) {
      if(key == GLFW_KEY_A){
         axis = !axis;
      }
   }

private:
   bool axis = false;
   Skeleton skeleton;
};


int main(int argc, char** argv) {
   Butterfly viewer;
   viewer.run();
}
