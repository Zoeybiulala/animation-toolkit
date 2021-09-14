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

class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
   }

   virtual void scene() {
   }

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
