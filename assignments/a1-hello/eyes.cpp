#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }
    
    virtual void setup() {
      routeleng = 40; //the pupil's max move range
      x1 = width() * 0.3; //the left eye's x position
      y = height() * 0.5; 
      x2 = width() * 0.7; //the right eye's x position
      r = 120; //eye's radius
    }

    virtual void scene() {
      // draw the left eye
      setColor(vec3(1,1,1));
      drawSphere(vec3(x1,y,-1000), r);
      //left pupil
      setColor(vec3(0,0,0));
      //the x position of the pupil move in time 
      float px1 = x1 + routeleng * sin(elapsedTime()); 
      drawSphere(vec3(px1,y,0),30);

      //draw the right eye
      setColor(vec3(1,1,1));
      drawSphere(vec3(x2,y,-1000), r);
      //right pupil
      setColor(vec3(0,0,0));
      //the x position of the pupil move in time 
      float px2 = x2 + routeleng * sin(elapsedTime());
      drawSphere(vec3(px2,y,0),30);
    }
  private:
    float routeleng;
    float x1,y, x2,r;

};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
