#include "atkui/framework.h"
#include <cmath>        // std::abs
using namespace glm;

class Screensaver : public atkui::Framework {
 public:
  Screensaver() : atkui::Framework(atkui::Orthographic) {
  }
  vec3 calcVal(vec3 B0, vec3 B1, vec3 B2, vec3 B3, float t){ //Bezier's 
    vec3 p_t = (float)(pow((1-t),3)) * B0
                + 3 * t * (float)(pow((1-t),2)) * B1 
                + 3 * (float)(pow(t,2) * (1-t)) * B2 
                + (float)(pow(t,3)) * B3;
    return p_t;
  }

  vec3 linearInpo(vec3 B0, vec3 B1, float t) {
    return (1 - t) * B0 + t * B1;
  }

  void setup() {
    //curve1
    float w = width();
    BS0 = vec3(agl::random()*w,agl::random()*w,0);
    BS1 = vec3(agl::random()*w,agl::random()*w,0);
    BS2 = vec3(agl::random()*w,agl::random()*w,0);
    BS3 = vec3(agl::random()*w,agl::random()*w,0);
    
    //curve2
    BE0 = vec3(agl::random()*w,agl::random()*w,0);
    BE1 = vec3(agl::random()*w,agl::random()*w,0);
    BE2 = vec3(agl::random()*w,agl::random()*w,0);
    BE3 = vec3(agl::random()*w,agl::random()*w,0);

    //current curve
    BC0 = BS0;
    BC1 = BS1;
    BC2 = BS2;
    BC3 = BS3;

  }

  void scene() {

    for (int i=0; i<500; i++){
      setColor(vec3(1,1,1));
      vec3 currP = calcVal(BE0,BE1,BE2,BE3,i/500.0f);
      drawLine(currP,currP+2.0f);
    }

    for (int i=0; i<500; i++){
      setColor(vec3(1,0,0));
      vec3 currP = calcVal(BS0,BS1,BS2,BS3,i/500.0f);
      drawLine(currP,currP+2.0f);
    }

    duration = 20.0f;
    t = elapsedTime()/duration;
    t = glm::clamp(t, 0.0f, 1.0f); // clamp to range [0,1]
    
    for (int i=0; i<5; i++){
      BC0 = linearInpo(BC0,BE0,t);
      BC1 = linearInpo(BC1,BE1,t);
      BC2 = linearInpo(BC2,BE2,t);
      BC3 = linearInpo(BC3,BE3,t);
      for(int j = 0; j < 500; j++) {
        setColor(vec3(1,0,0));
        vec3 currP = calcVal(BC0,BC1,BC2,BC3,j/500.0f);
        drawLine(currP,currP+2.0f);
      }
      
    }

  }
  private:
    vec3 BS0,BS1,BS2,BS3; //starting curve ctrl pts
    vec3 BE0,BE1,BE2,BE3; //ending curve ctrl pts
    vec3 BC0,BC1,BC2,BC3; //current curve ctrl pts
    float duration;
    float t;

};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

