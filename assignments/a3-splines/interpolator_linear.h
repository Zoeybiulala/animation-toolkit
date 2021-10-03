#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Linear 
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
    InterpolatorLinear() : Interpolator("Linear") {}
    virtual glm::vec3 interpolate(int segment, double u) const {
       glm::vec3 b0 = mCtrlPoints[segment];
       glm::vec3 b1 = mCtrlPoints[segment + 1];
       return b0*(float)(1-u) + b1 * (float)u;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       mCtrlPoints = keys;
    }
};

#endif
