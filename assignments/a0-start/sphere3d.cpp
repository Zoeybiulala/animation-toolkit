#include "atkui/framework.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
    
  }
  vec3 currentPos;
  vec3 vel;
  int keyFlag;
  virtual void setup() {
    //position
    currentPos = vec3(0,0,0);
    //velocity
    vel = agl::randomUnitVector() * 100.0f;
    keyFlag = 0;
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));
    //hitting the space bar then move
    if (keyFlag == 1) {
      currentPos += dt() * vel;
    }
    // draw a sphere at center of the world
    float radius = 50.0;
    drawSphere(currentPos, radius);
  }

  void keyUp(int key, int mods){

    if(key == GLFW_KEY_SPACE) {
      std::cout << "toggle space key" << std::endl;
      //change the position
      keyFlag = 1;
    } else if (key == GLFW_KEY_R) {
      std::cout << "toggle R key" << std::endl;
      //reset position
      currentPos = vec3(0,0,0);
      //geenrate a new direction
      vel = agl::randomUnitVector() * 100.0f;
      keyFlag = 0;
    } 
  }
  
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
