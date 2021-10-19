#include "atkui/framework.h"

using glm::vec3;

class Example : public atkui::Framework {
 public:
  Example() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
     theta = 0.0;
     thetaRate = 0.05;
     r = 250.0;
  }

  virtual void scene() {
    theta += thetaRate + dt();
    float px = r * cos(theta) + width() * 0.5;
    float py = r * sin(theta) + height() * 0.5;
    setColor(vec3(1,0,0));
    drawSphere(vec3(px,py,0),100);
  }

  private:
    float theta;
    float thetaRate;
    float r;
  
};

int main(int argc, char** argv) {
  Example example;
  example.run();
}
