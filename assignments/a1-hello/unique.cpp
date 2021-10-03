#include "atkui/framework.h"
#include <math.h> //for the use of pi

using namespace glm;

class Particle {
  public:
    float _x;
    float _y;
    float _theta;
    vec3 _col;

    Particle(float x, float y, float theta, vec3 col) {
      _x = x;
      _y = y;
      _theta = theta;
      _col = col;
    }

    Particle() {} //default constructor
};

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    theta = 0.0;
    thetaRate = 0.2;
    r = 200.0;

    for (int i=0; i< 150; i++) {
      float theta = M_PI / 75 * i;
      vec3 col = vec3((i/150.0f),0,(1-i/150.0f));
      float a = width()*0.5;
      pArray[i] = Particle(a + r * cos(4 * theta) * cos(theta),
                           a + r * cos(4 * theta) * sin(theta),
                           theta,
                           col);
    }
  }

  virtual void scene() {
    theta += thetaRate * dt();
    float px = r * cos(4*theta) * cos(theta) + 0.5 * width();
    float py = r * cos(4*theta) * sin(theta) + 0.5 * height();
    setColor(vec3(1, 0, 0));
    drawSphere(vec3(px, py, 0), 10);

    for (int i =0; i<150; i++) {
      setColor(pArray[i]._col);
      pArray[i]._theta += thetaRate * dt();
      float tTheta = pArray[i]._theta; //change theta so that it rotates
      pArray[i]._x = r * cos(4* tTheta) * cos(tTheta) + 0.5 * width();
      pArray[i]._y = r * cos(4* tTheta) * sin(tTheta) + 0.5 * height();
      drawSphere(vec3(pArray[i]._x,pArray[i]._y,0),5);
    }
  } 

  private:
    float theta;
    float thetaRate;
    float r;
    Particle pArray[150];
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

