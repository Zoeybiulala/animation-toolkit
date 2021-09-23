#include "atkui/framework.h"
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
    BS1 = agl::randomUnitVector() * 500.0f;
    BS2 = agl::randomUnitVector() * 500.0f;
    BS3 = agl::randomUnitVector() * 500.0f;
    BS4 = agl::randomUnitVector() * 500.0f;
    
    //curve2
    BE1 = agl::randomUnitVector() * 500.0f;
    BE2 = agl::randomUnitVector() * 500.0f;
    BE3 = agl::randomUnitVector() * 500.0f;
    BE4 = agl::randomUnitVector() * 500.0f;

    //current curve
    BC1 = BS1;
    BC2 = BS2;
    BC3 = BS3;
    BC4 = BS4;

  }

  void scene() {
    setColor(vec3(1,1,1));
    for (int i=0; i<100; i++){
      vec3 currP = calcVal(BE1,BE2,BE3,BE4,1/100.0f);
      drawLine(currP,currP+0.01f);
    }

    duration = 2.0f;
    t = elapsedTime()/duration;
    t = glm::clamp(t, 0.0f, 1.0f); // clamp to range [0,1]
    
    BC1 = linearInpo(BC1,BE1,t);
    BC2 = linearInpo(BC2,BE2,t);
    BC3 = linearInpo(BC3,BE3,t);
    BC4 = linearInpo(BC4,BE4,t);
    
    setColor(vec3(1,0,0));
    for (int i=0; i<100; i++){
      vec3 currP = calcVal(BC1,BC2,BC3,BC4,t);
      drawLine(currP,currP+0.01f);
    }

  }
  private:
    vec3 BS1,BS2,BS3,BS4; //starting curve ctrl pts
    vec3 BE1,BE2,BE3,BE4; //ending curve ctrl pts
    vec3 BC1,BC2,BC3,BC4; //current curve ctrl pts
    float duration;
    float t;

};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

