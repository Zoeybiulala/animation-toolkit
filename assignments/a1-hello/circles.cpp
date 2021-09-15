#include "atkui/framework.h"
#include <math.h> //for the use of pi

using namespace glm;


class Particle {
  public:
    float _x;
    float _y;
    float _theta;
    vec3 _col;

    Particle(float x, float y, vec3 col,float theta) {
      _x = x;
      _y = y;
      _col = col;
      _theta = theta;
    }

    Particle() {} //default constructor
};

class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      //circular movement variables
      thetaRate = 0.2;
      //1 or -1 to indicate the direction of rotation
      dir = 1;

      //initialization of particles
      for(int i = 0; i < 12; i++) {
         for(int j = 0; j < 36; j++) {
            float r = i * 20.0f; //distance from the middle point of the circles
            float theta = j * M_PI/18; // each is 10 degree apart
            float px = width() * 0.5 + r * cos(theta);
            float py = height() * 0.5 + r  * sin(theta);
            vec3 color = platte[i % 5]; //choose color for each circle
            pArray[i][j] = Particle(px, py, color, theta); 
         }
      }
   }

   virtual void scene() {
      for (int i =0; i < 12; i++) {
         for(int j =0; j < 36; j++) {
            pArray[i][j]._theta += thetaRate * dt() * dir; //change theta so that it rotates
            pArray[i][j]._x = i * 40.0f * cos(pArray[i][j]._theta) + width() * 0.5;
            pArray[i][j]._y = i * 40.0f * sin(pArray[i][j]._theta) + height() * 0.5;
            setColor(pArray[i][j]._col);
            drawSphere(vec3(pArray[i][j]._x,pArray[i][j]._y,0),15);
         }
         dir *= -1; //change rotation direction
      }
   }

private:
   float thetaRate;
   int dir;
   Particle pArray[12][36];//12 concentric circles and each has 36 particles
    //https://www.color-hex.com/color-palette/82379
   vec3 platte[5] = {vec3(0,165,227)/255.0f,
      vec3(141,215,191)/255.0f,
      vec3(255,150,197)/255.0f,
      vec3(255,87,104)/255.0f,
      vec3(255,162,58)/255.0f};

};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
