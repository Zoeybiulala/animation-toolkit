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
    if(mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      target = vec3(_mouseX, _mouseY, 0);
    } else {
      target = vec3(width() * 0.5, height() * 0.5, 0);
    }
    setColor(vec3(1,0,0));
    drawSphere(target, 10);

    // draw the left eye
    setColor(vec3(1,1,1));
    drawSphere(vec3(x1,y,-1000), r);

    // draw the left pupil
    setColor(vec3(0,0,0));
    //the x position of the pupil move in time 
    px1 = x1; //TODO: update this to follow the mouse
    drawSphere(vec3(px1,y,0),30);

    //draw the right eye
    setColor(vec3(1,1,1));
    drawSphere(vec3(x2,y,-1000), r);

    //draw the right pupil
    setColor(vec3(0,0,0));
    px2 = x2;
    drawSphere(vec3(px2,y,0),30);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    _mouseX = x;
    _mouseY = height() - y; //?
  }

 private:
  //current mouse position?
  int _mouseX;
  int _mouseY;
  float x1,y, x2,r,px1,px2;
  vec3 target;
  
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
