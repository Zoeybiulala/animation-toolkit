#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"
#include <iostream>

//--------------------------------
// Hermite 
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1,0,0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       float t = (float)u;
       glm::vec3 b0 = mCtrlPoints[segment*2]; //p0
       glm::vec3 b1 = mCtrlPoints[segment*2+1]; //p0'
       glm::vec3 b2 = mCtrlPoints[segment*2+3]; //p1'
       glm::vec3 b3 = mCtrlPoints[segment*2+2]; //p1
    //    glm::vec3 p_t = (float)(pow((1-t),3)) * b0
    //             + 3 * t * (float)(pow((1-t),2)) * b1 
    //             + 3 * (float)(pow(t,2) * (1-t)) * b2 
    //             + (float)(pow(t,3)) * b3;
    //    return p_t;
    // std::cout<< to_string(b0) <<std::endl;
    // std::cout<< to_string(b1) <<std::endl;
    // std::cout<< to_string(b2) <<std::endl;
    // std::cout<< to_string(b3) <<std::endl;

       glm::vec3 p_t = (1- 3*t*t + 2*t*t*t) * b0 +
                       (t*(1-t)*(1-t)) * b1 +
                       ((-1) *t*t + t*t*t) * b2 +
                       (3*t*t - 2* t*t*t) * b3;



       return p_t;
    // return glm::vec3(0);
    }


    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
        int size = keys.size();
        int n = size-1;
        Eigen::MatrixXd A(size, size);
        Eigen::MatrixXd p(size, 3);
        Eigen::MatrixXd pPrime(size, 3); // slopes for each control point
        //initialize Matrix A with all 0
        for(int i=0; i< size; i++) {
            for (int j =0; j< size ; j++) {
                A(i,j) = 0;
            }
        }

        if(isClamped()){
            glm::vec3 v0,vn;
            A(0,0) = mClampDir[0]; A(0,1) = mClampDir[1]; A(0,2) = mClampDir[2]; //start point
            A(n,n) = mClampDir[0]; A(n, n-1) = mClampDir[1]; A(n, n-2) = mClampDir[2];  //end point

            v0 = mClampDir;
            p(0,0) = v0[0]; p(0,1) = v0[1]; p(0,2) = v0[2];

            glm::vec3 v;
            for(int i=1;i<size;i++){
                v = 3.0f * (keys[i+1]-keys[i-1]);
                p(i,0) = v[0]; p(i,1) = v[1]; p(i,2) = v[2];
            }

            vn = mClampDir;
            p(n,0) = vn[0]; p(n,1) = vn[1]; p(n,2) = vn[2];
  
        } else { //natural
            glm::vec3 v0,vn;
            A(0,0) = 2; A(0,1) = 1; //start point
            A(n,n) = 2; A(n, n-1) = 1; //end point

            v0 = 3.0f * (keys[1]-keys[0]);
            p(0,0) = v0[0]; p(0,1) = v0[1]; p(0,2) = v0[2];

            glm::vec3 v;
            for(int i=1;i<size;i++){
                v = 3.0f * (keys[i+1]-keys[i-1]);
                p(i,0) = v[0]; p(i,1) = v[1]; p(i,2) = v[2];
            }
            
            vn = 3.0f * (keys[n]-keys[n-1]);
            p(n,0) = vn[0]; p(n,1) = vn[1]; p(n,2) = vn[2];

        }
        //common parts of matrix A
        int j = 0;
        for(int i=1; i<n;i++){
                A(i,j) = 1;
                A(i,j+2) = 1;
                j++;
        }
        for(int i=1; i<n;i++){
            A(i,i) = 4;
        }
    
        pPrime = A.inverse() * p; //calculate the slopes

        //generate control points
        glm::vec3 b;
        for(int i=0; i< size;i++){
            b = glm::vec3(pPrime(i,0),pPrime(i,1),pPrime(i,2));
            mCtrlPoints.push_back(keys[i]);
            mCtrlPoints.push_back(b);
        }
        
      
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3& v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = false;
    glm::vec3 mClampDir;
    
};

#endif
