#include "atkui/framework.h"

using namespace glm;

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    theta = 0.0;
    thetaRate = 0.2;
    r = 100.0;
  }

  virtual void scene() {
    theta += thetaRate * dt();
    float px = r * cos(theta) + 0.5 * width();
    float py = r * sin(theta) + 0.5 * height();
    setColor(vec3(1, 0, 0));
    drawSphere(vec3(px, py, 0), 100);
  }

  private:
    float theta;
    float thetaRate;
    float r;
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

