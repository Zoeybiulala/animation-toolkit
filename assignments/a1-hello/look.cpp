#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }
  

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;

    x1 = width() * 0.3; //the left eye's x position
    y = height() * 0.5; 
    x2 = width() * 0.7; //the right eye's x position
    r = 120; //eye's radius
  }

  virtual void scene() {
    //the target setup
    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1,0,0));
    drawSphere(target, 10);

    // draw the left eye
    setColor(vec3(1,1,1));
    drawSphere(vec3(x1,y,-1000), r);

    // draw the left pupil
    setColor(vec3(0,0,0));
    //the x position of the pupil move in time 
    px1 = x1 + routeleng * sin(elapsedTime()); //TODO: update this to follow the mouse
    drawSphere(vec3(px1,y,0),30);

    //draw the right eye
    setColor(vec3(1,1,1));
    drawSphere(vec3(x2,y,-1000), r);

    //the x position of the pupil move in time 
    float px2 = x2 + routeleng * sin(elapsedTime());
    drawSphere(vec3(px2,y,0),30);
  }

  void mouseMove(int x, int y) {
    _mouseX = x;
    _mouseY = height() - y; //?
  }

  void mouseDown(int button, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      
    }
  }

 private:
  int _mouseX;
  int _mouseY;
  float x1,y, x2,r,px1,px2;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
