#include "atkui/framework.h"

using namespace glm;

class Particle {
  public:
    float _x;
    float _y;
    float _speed;
    vec3 _col;

    Particle(float x, float y, float speed,vec3 col) {
      _x = x;
      _y = y;
      _speed = speed;
      _col = col;
    }

    Particle() {} //default constructor
};

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    baseColor = vec3(0,1,0);
    direction = agl::randomUnitVector(); //maybe just set it to a specific direction
    for(int i=0; i<250; i++) {
      ps[i] = Particle(agl::random() * 500.0f, agl::random() * 500.0f, 
              agl::random() * 100.0f, baseColor * agl::random(-0.2,0.2));
      //may add v initialization as well
    }
  }

  virtual void scene() {
    for (int i =0; i<250;i++) {
      setColor(ps[i]._col);
      float vel = agl::random() * 5;
      if(ps[i]._x > 500) {
        ps[i]._x = 0;
      } else {
        ps[i]._x += direction[0] * ps[i]._speed * dt();
      }
      if(ps[i]._y < 0) {
        ps[i]._y = 500;
      } else {
        ps[i]._y += direction[1] * ps[i]._speed * dt();
      }
      drawSphere(vec3(ps[i]._x,ps[i]._y,0),15);
    }
  }


private:
  Particle ps[250]; //100 particles 
  vec3 direction; //the velocity of the particles
  vec3 baseColor;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
