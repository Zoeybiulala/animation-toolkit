#include "atkui/framework.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
    
  }
  //should this be private?
  //position
  double x = 0;
  double y = 0;
  double z = 0;
  //velocity
  double v = 500;
  //random direction
  vec3 unitvec = agl::randomUnitVector();

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));


    // draw a sphere at center of the world
    float radius = 50.0;
    drawSphere(vec3(x,y,z), radius);
  }

  void keyUp(int key, int mods){

    if(key == GLFW_KEY_SPACE) {
      std::cout << "toggle spac key" << std::endl;
      //change the position
      x += dt() * v * unitvec[0];
      y += dt() * v * unitvec[1];
      z += dt() * v * unitvec[2];
      std::cout << dt() << std::endl;
    } else if (key == GLFW_KEY_R) {
      std::cout << "toggle R key" << std::endl;
      //reset position
      x = 0;
      y = 0;
      z = 0;
      unitvec = agl::randomUnitVector(); //reset to another random direction
    } 
  }
  
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
