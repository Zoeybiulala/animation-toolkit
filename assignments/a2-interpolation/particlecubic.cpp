#include "atkui/framework.h"
#include <math.h> //for fmod
using namespace glm;

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic) {
  }

  vec3 calcVal(float t){ //Bezier's 
    vec3 p_t = (float)(pow((1-t),3)) * B0
                + 3 * t * (float)(pow((1-t),2)) * B1 
                + 3 * (float)(pow(t,2) * (1-t)) * B2 
                + (float)(pow(t,3)) * B3;
    return p_t;
  }

  void setup() {
    //originally start from B0;
    pos = B0;
    t = 0;
  }

  void scene() {
    
    //cubic curve
    setColor(vec3(1,1,1));
    for (int i=0; i<100; i++){
      drawLine(calcVal(i/100.0f),calcVal(i/100.0f+0.01));
    }

    float duration = 5.0f;
    t =  elapsedTime()/duration;
    t  = fmod(t, 1);


    setColor(vec3(1, 0, 0));
    pos = calcVal(t);
    drawSphere(pos, 10);
  }

  private:
    vec3 B0 = vec3(100,  50, 0);
    vec3 B1 = vec3(150, 200, 0);
    vec3 B2 = vec3(250, 100, 0);
    vec3 B3 = vec3(300, 300, 0);
    float t;
    vec3 pos;
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

