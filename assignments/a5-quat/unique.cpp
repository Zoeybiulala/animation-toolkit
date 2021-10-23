#include "atkmath/constants.h"
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkui/framework.h"
#include <math.h> //for the use of pi

using namespace glm;
using namespace atkmath;
using glm::vec3;

class Particle {
  public:
    float _x;
    float _y;
    float _z;
    vec3 _col;

    Particle(float x, float y, float z ,vec3 col) {
      _x = x;
      _y = y;
      _z = z;
      _col = col;
    }

    Particle() {} //default constructor
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }
   virtual void setup() {
      _mouseX = width() * 0.5;
      _mouseY = height() * 0.5;
      axis = vec3(_mouseX, _mouseY, 0);
      
      baseColor = vec3(0,1,0);
      for(int i=0; i<500; i++) {
         ps[i] = Particle(agl::random() * 500.0f, agl::random() * 500.0f, agl::random() * 500.0f,baseColor * agl::random(-0.2,0.2));
   
      }
      thetaRate = 0.1;
      theta = 0.0;
   }

   virtual void scene() {
      axis = vec3(_mouseX, _mouseY, 0);
      theta += thetaRate + dt();
      for (int i =0; i<500;i++) {
         setColor(ps[i]._col);
         vec3 pos = vec3(ps[i]._x,ps[i]._y,ps[i]._z);
         push();

         translate(pos);
         rotate(theta, axis);
         drawSphere(vec3(ps[i]._x,ps[i]._y,ps[i]._z),15);
         pop();
      }
      
   }

   void mouseMotion(int pX, int pY, int dx, int dy)
   {
      _mouseX = pX;
      _mouseY = height() - pY;
   }


 private:
   Particle ps[1000];
   int _mouseX;
   int _mouseY;
   vec3 axis;
   vec3 baseColor,direction;
   float theta;
   float thetaRate;
   
};

int main(int argc, char** argv) {
   Unique viewer;
   viewer.run();
   return 0;
}

