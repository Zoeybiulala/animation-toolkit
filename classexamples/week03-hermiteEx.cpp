#include <iostream>
#include <Eigen/Dense>
#include "agl/aglm.h"

using glm::vec2;

glm::vec3 mCtrlPoints[5];

glm::vec3 interpolate(int segment, float u) {
       glm::vec3 b0 = mCtrlPoints[segment*2];
       glm::vec3 b1 = mCtrlPoints[segment*2+1];
       glm::vec3 b2 = mCtrlPoints[segment*2+2];
       glm::vec3 b3 = mCtrlPoints[segment*2+3];
       glm::vec3 p_t = (float)(pow((1-u),3)) * b0
                + 3 * u * (float)(pow((1-u),2)) * b1 
                + 3 * (float)(pow(u,2) * (1-u)) * b2 
                + (float)(pow(u,3)) * b3;
       return p_t;
    // return glm::vec3(0);
    }

int main()
{
    
    vec2 p0(0,0); // Z will just be zero
    vec2 p1(1,2);
    vec2 p2(3,3);
    vec2 p3(6,3);
    vec2 p4(8,0);

    // Solve the system of linear equations
    Eigen::MatrixXd A(5, 5);
    Eigen::MatrixXd p(5, 2);
    Eigen::MatrixXd pPrime(5, 2); // slopes for each control point

    // natural endpoints
    A(0,0) =  2; A(0,1) = 1; A(0,2) = 0; A(0,3) = 0; A(0,4) = 0;
    A(1,0) =  1; A(1,1) = 4; A(1,2) = 1; A(1,3) = 0; A(1,4) = 0;
    A(2,0) =  0; A(2,1) = 1; A(2,2) = 4; A(2,3) = 1; A(2,4) = 0;
    A(3,0) =  0; A(3,1) = 0; A(3,2) = 1; A(3,3) = 4; A(3,4) = 1;
    A(4,0) =  0; A(4,1) = 0; A(4,2) = 0; A(4,3) = 1; A(4,4) = 2;

    vec2 v0 = 3.0f *(p1 - p0);
    vec2 v1 = 3.0f *(p2 - p0);
    vec2 v2 = 3.0f *(p3 - p1);
    vec2 v3 = 3.0f *(p4 - p2);
    vec2 v4 = 3.0f *(p4 - p3);
    p(0,0) = v0[0]; p(0,1) = v0[1];
    p(1,0) = v1[0]; p(1,1) = v1[1]; 
    p(2,0) = v2[0]; p(2,1) = v2[1]; 
    p(3,0) = v3[0]; p(3,1) = v3[1]; 
    p(4,0) = v4[0]; p(4,1) = v4[1]; 
    for(int i= 0; i< 5;i++) {
                for (int j = 0; j< 2; j++) {
                    std::cout<< p(i,j)<<" ";
                }
                std::cout<< " " << std::endl;
            }
    

    pPrime = A.inverse() * p;
    for (int i = 0; i < 5; i++)
    {   
        mCtrlPoints[i] = glm::vec3(pPrime(i,0),pPrime(i,1),0);
        std::cout<<mCtrlPoints[i]<<std::endl;
        std::cout << "slope " << i << " = " << pPrime(i,0) << " " << pPrime(i,1) << std::endl;
    }


glm::vec3 value;
   value = interpolate(0, 0.0);
   std::cout << "(clamped) interpolate(0, 0) = " << value << std::endl;

   value = interpolate(3, 1.0);
   std::cout << "(clamped) interpolate(3, 1) = " << value << std::endl;

   value = interpolate(2, 0.5);
   std::cout << "(clamped) interpolate(2, 0.5) = " << value << std::endl;

}


