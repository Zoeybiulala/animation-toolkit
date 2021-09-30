#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       float t = (float)u;
       glm::vec3 b0 = mCtrlPoints[segment*3];
       glm::vec3 b1 = mCtrlPoints[segment*3+1];
       glm::vec3 b2 = mCtrlPoints[segment*3+2];
       glm::vec3 b3 = mCtrlPoints[segment*3+3];
       glm::vec3 p_t = (float)(pow((1-t),3)) * b0
                + 3 * t * (float)(pow((1-t),2)) * b1 
                + 3 * (float)(pow(t,2) * (1-t)) * b2 
                + (float)(pow(t,3)) * b3;
       return p_t;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       int size = keys.size();
       int n = keys.size()-1; // n is the the last element's index and also the number of segments
       glm::vec3 b_0,b_1,b_2,b_3;
       for(int i=0; i < n; i++) {
          b_0 = keys[i];
          mCtrlPoints.push_back(b_0);

          if(i == 0) { //start point
             b_1 = keys[0] + (1/6.0f) * (keys[1]-keys[0]);
             mCtrlPoints.push_back(b_1);
          } else {
             b_1 = keys[i] + (1/6.0f) * (keys[i+1] - keys[i-1]);
             mCtrlPoints.push_back(b_1);
          }

          if(i == n-1) { //end point
             b_2 = keys[n]- (1/6.0f) * (keys[n]-keys[n-1]);
             mCtrlPoints.push_back(b_2);
             b_3 = keys[i+1];
             mCtrlPoints.push_back(b_3);
          } else {
             b_2 = keys[i+1] - (1/6.0f) * (keys[i+2]-keys[i]);
             mCtrlPoints.push_back(b_2);
          }
       }

    }
};

#endif
