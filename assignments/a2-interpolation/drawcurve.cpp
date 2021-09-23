#include <iostream>
#include "atkui/framework.h"
#include<math.h>
using namespace glm;

class DrawCubic : public atkui::Framework
{
 public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }

  vec3 calcVal(float t){ //Bezier's 
    vec3 p_t = (float)(pow((1-t),3)) * B0
                + 3 * t * (float)(pow((1-t),2)) * B1 
                + 3 * (float)(pow(t,2) * (1-t)) * B2 
                + (float)(pow(t,3)) * B3;
    return p_t;
  }

  vec3 LERP(vec3 b_i, vec3 b_ii, float t){
    return b_i * (1 - t) + b_ii * t;
  }

  vec3 calcVal_Cast(float t){
    //LEVEL 1
    vec3 b11 = LERP(B0,B1,t);
    vec3 b12 = LERP(B1,B2,t);
    vec3 b13 = LERP(B2,B3,t);
    //LEVEL 2
    vec3 b21 = LERP(b11, b12, t);
    vec3 b22 = LERP(b12, b13, t);
    //LEVEL 3
    vec3 b31 = LERP(b21, b22, t);
    return b31;
  }

  virtual void scene()
  {
    setColor(vec3(0,0,1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    setColor(vec3(1,1,0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);

    if(key1 == 1) {
      setColor(vec3(1,1,1));
      for (int i=0; i<100; i++){
        drawLine(calcVal(i/100.0f),calcVal(i/100.0f+0.01));
      }
    }

    if(key2 == 1) {
      setColor(vec3(0,0,1));
      for (int i = 0; i<100; i++) {
        drawLine(calcVal_Cast(i/100.0f),calcVal_Cast(i/100.0f+0.01));
      }
    }
    
  }

  void keyUp(int key, int mod) {
    if (key == GLFW_KEY_1) {
      key1 = 1;
      key2 = 0;
    }
    else if (key == GLFW_KEY_2) {
      key2 = 1;
      key1 = 0;
    }
  }

 private:
  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  int key1 = 0;
  int key2 = 0;
};

int main(int argc, char **argv) {
  DrawCubic viewer;
  viewer.run();
}
