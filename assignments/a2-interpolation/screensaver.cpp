#include "atkui/framework.h"
#include <math.h> //for fmod
#include <list> //for iterator
#include <iterator> //for iterator
using namespace glm;

class Curve{
  public:
    vec3 _B0;
    vec3 _B1;
    vec3 _B2;
    vec3 _B3;
    vec3 _col;

  Curve(){}
  
  Curve(vec3 B0,vec3 B1, vec3 B2, vec3 B3, vec3 col){
    _B0 = B0;
    _B1 = B1;
    _B2 = B2;
    _B3 = B3;
    _col = col;
  }
};

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

  void randomize(){
    float w = width();
    BE0 = vec3(agl::random()*w,agl::random()*w,0);
    BE1 = vec3(agl::random()*w,agl::random()*w,0);
    BE2 = vec3(agl::random()*w,agl::random()*w,0);
    BE3 = vec3(agl::random()*w,agl::random()*w,0);
  }

  vec3 randomCol(){
    return vec3(agl::random(),agl::random(),agl::random());
  }

  void setup() {
    //curve1
    float w = width();
    BS0 = vec3(agl::random()*w,agl::random()*w,0);
    BS1 = vec3(agl::random()*w,agl::random()*w,0);
    BS2 = vec3(agl::random()*w,agl::random()*w,0);
    BS3 = vec3(agl::random()*w,agl::random()*w,0);
    
    //curve2
    randomize();

    //current curve
    BC0 = BS0;
    BC1 = BS1;
    BC2 = BS2;
    BC3 = BS3;

    col = randomCol();
    currCol = randomCol();
  }

  void scene() {
  
    t += dt()/2;

    //draw end curve
    setColor(col);
    for (int i=0; i<500; i++){
      vec3 currP = calcVal(BE0,BE1,BE2,BE3,i/500.0f);
      drawLine(currP,currP+2.0f);
    }
    
    //draw the current curve
    currCol = linearInpo(currCol,col,t/30);
    BC0 = linearInpo(BS0,BE0,t);
    BC1 = linearInpo(BS1,BE1,t);
    BC2 = linearInpo(BS2,BE2,t);
    BC3 = linearInpo(BS3,BE3,t);
    //trailing effect
    if(t-0.2 <0.01 || t-0.4 <0.01 || t-0.6 <0.01 || t-0.8 <0.01 || t-1 <0.01){
      Curve cur = Curve(BC0,BC1,BC2,BC3,currCol);
      lst.push_back(cur);
      if(lst.size() == 50){
        lst.pop_front();
      }
    }
    
    //reference:https://www.includehelp.com/stl/iterate-a-list-example-of-list-begin-and-list-end-functions.aspx#:~:text=To%20iterate%20a%20list%20in%20C%2B%2B%20STL%2C%20we,returns%20an%20iterate%20pointing%20to%20the%20last%20element.
    std::list<Curve>::reverse_iterator it;
    for(it = lst.rbegin();it !=lst.rend();it++){ 
      Curve cur = *it;
      setColor(cur._col);
      for(int j = 0; j < 500; j++) {
       vec3 currP = calcVal(cur._B0,cur._B1,cur._B2,cur._B3,j/500.0f);
       drawLine(currP,currP+2.0f);
      }
    }

    setColor(currCol);
    for(int j = 0; j < 500; j++) {
      vec3 currP = calcVal(BC0,BC1,BC2,BC3,j/500.0f);
      drawLine(currP,currP+2.0f);
    }
    


    if(t >= 1){
      t = 0;
      //curve 2  = new curve
      randomize();
      col = randomCol();

      BS0 = BC0;
      BS1 = BC1;
      BS2 = BC2;
      BS3 = BC3;

    }

  }
  private:
    vec3 BS0,BS1,BS2,BS3; //starting curve ctrl pts
    vec3 BE0,BE1,BE2,BE3; //ending curve ctrl pts
    vec3 BC0,BC1,BC2,BC3; //current curve ctrl pts
    float t;
    vec3 col,currCol;
    std::list<Curve> lst;
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

