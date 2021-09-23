#include <iostream>
#include "atkui/framework.h"
using namespace glm;
class Cube{
  public:
    float _x;
    float _y;
    vec3 _col;

    Cube(float x, float y, vec3 col) {
      _x = x;
      _y = y;
      _col = col;
    }
    
    Cube() {} //default constructor
};

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }

  vec3 calcCol(int i, int j){
    vec3 c0 = c_nw * (1-i/10.0f) + c_ne * (i/10.0f);
    vec3 c1 = c_sw * (1-i/10.0f) + c_se * (i/10.0f);
    vec3 c = c0 * (1-j/10.0f) + c1 * (j/10.0f);
    return c;
  }

  virtual void setup() {
    float offset = width()/20.0f;
    for(int i=0; i< N; i++) {
      for(int j =0; j< N; j++) {
        cubes[i][j]  = Cube(width()*i/10.0f+offset, height()*j/10.0f+offset, calcCol(i,j));
      }
    }
  }

  virtual void scene() {
    for(int i=0; i< N; i++) {
      for(int j =0; j< N; j++) {
        Cube curr = cubes[i][j];
        setColor(curr._col);
        float size = width()/10.0f;
        drawCube(vec3(curr._x,curr._y,0),vec3(size,size,size));
      }
    }
  }

  int N = 10;
  Cube cubes[10][10];
  vec3 c_nw = vec3(1,1,0);
  vec3 c_ne = vec3(0,1,1);
  vec3 c_sw = vec3(1,0,0);
  vec3 c_se = vec3(1,0,1);
};



int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
