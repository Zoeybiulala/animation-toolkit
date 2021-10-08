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
    float _speed;
    vec3 _col;

    Particle(float x, float y, float z ,float speed,vec3 col) {
      _x = x;
      _y = y;
      _z = z;
      _speed = speed;
      _col = col;
    }

    Particle() {} //default constructor
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  void pushEulerRotation(const Vector3& euler) {
  Vector3 anglesRad = euler * Deg2Rad;
   //ZYX
    rotate(anglesRad[2], vec3(0, 0, 1));
    rotate(anglesRad[1], vec3(0, 1, 0));
    rotate(anglesRad[0], vec3(1, 0, 0));
  }

  virtual void setup() {
    renderer.beginShader("phong");
     aRate = .1f;
     tRate = 0.2f;
     bRate = 0.3f;
    baseColor = vec3(0,1,0);
    direction = agl::randomUnitVector(); //maybe just set it to a specific direction
    for(int i=0; i<500; i++) {
      ps[i] = Particle(agl::random() * 1000.0f, agl::random() * 1000.0f, agl::random() * -1000.0f,
              agl::random() * 100.0f, baseColor * agl::random(-0.2,0.2));
   
    }

    Vector3 ZYX = rot.toEulerAnglesZYX() * Rad2Deg;
  }

   virtual void scene() {
      alpha += aRate + dt();
      beta += bRate + dt();
      theta += tRate + dt();
    for (int i =0; i<500;i++) {
      setColor(ps[i]._col);
      if(ps[i]._x > 1000) {
        ps[i]._x = 0;
      } else {
        ps[i]._x += direction[0] * ps[i]._speed * dt();
      }
      if(ps[i]._y < -500) {
        ps[i]._y = 500;
      } else {
        ps[i]._y += direction[1] * ps[i]._speed * dt();
      }
      push();
      translate(vec3(250, 250, 0));
      euler = Vector3(alpha,theta,beta);
      pushEulerRotation(euler);
      drawSphere(vec3(ps[i]._x,ps[i]._y,ps[i]._z),15);
      pop();
    }
  }
 
  private:
   Particle ps[500]; //500 particles 
   vec3 direction; //the velocity of the particles
   vec3 baseColor;
   float alpha,theta,beta =0.0f;
   float aRate,tRate,bRate;
   Matrix3 rot;
   Vector3 euler = Vector3();
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}


