#include "atkui/framework.h"
using namespace glm;

class Unique : public atkui::Framework {
   public:
   Unique() : atkui::Framework(atkui::Perspective) {
   }
   //degree-4 interpolation - Bezier's
   vec3 degree4Inpo(float t){
      vec3 p_t = (float)(pow(t,4)) * B0
                + 4 * (1-t) * (float)(pow(t,3)) * B1 
                + 6 * (float)(pow(t,2) * pow((1-t),2)) * B2 
                + 4 * t * (float)(pow((1-t),3)) * B3 
                + (float)(pow((1-t),4)) * B4;
    return p_t;
   }
 
   
   //reference: https://easings.net/en#easeOutBounce
   float easeOutBounce(float x, float n1, float d1) {
      // float n1 = 7.5625;
      // float d1 = 2.75;

      if (x < 1 / d1) {
         return n1 * x * x;
      } else if (x < 2 / d1) {
         x -= 1.5;
         return n1 * (x / d1) * x + 0.75;
      } else if (x < 2.5 / d1) {
         x -= 2.25;
         return n1 * (x / d1) * x + 0.9375;
      } else {
         x -= 2.625;
         return n1 * (x / d1) * x + 0.984375;
      }
   }     

   vec3 linearInpo(vec3 B0, vec3 B1, float t) {
     return (1 - t) * B0 + t * B1;
   }
   vec3 randomCol(){
      return vec3(agl::random(),agl::random(),agl::random());
   }

   void setup(){
      currentPos = vec3(0,50,0);
      col = vec3(1,0,1);
      newCol = randomCol();
   }
   void scene(){
      //draw control points
      setColor(vec3(0,0,1));
      drawSphere(B0, 10);
      drawSphere(B4, 10);

      setColor(vec3(1,1,0));
      drawSphere(B1, 10);
      drawSphere(B2, 10);
      drawSphere(B3, 10);

      t += dt()/5;
      t  = fmod(t, 1);
      if(t>=0.99){
         newCol = randomCol();
      }
      col = linearInpo(col, newCol,t/30);
      currentPos = degree4Inpo(t);
      setColor(col);
      drawCube(currentPos, 
         vec3(100 + easeOutBounce(t,7.5625,2.75),
              100 + easeOutBounce(t,7.5625,5.0),
              100 + easeOutBounce(t,7.5625,2.75)));
   }

   private:
      vec3 currentPos;
      vec3 col,newCol; 
      vec3 B0 = vec3(-100,200,-300);
      vec3 B1 = vec3(0,0,0);
      vec3 B2 = vec3(-200,100,500);
      vec3 B3 = vec3(-300,300,500);
      vec3 B4 = vec3(100,200,-300);
      float t;
};


int main(int argc, char** argv) {
   Unique viewer;
   viewer.run();
}

