#include "behaviors.h"
#include "steerable.h"
#include <cmath>

using namespace glm;
using namespace atk;

//--------------------------------------------------------------
// Shared properties for all behaviors
ABehavior::ABehavior(const char* name) : _name(name)
{
   // TODO: set good values
   setParam("MaxSpeed", 100);
   setParam("AgentRadius", 50);
}

//--------------------------------------------------------------
// Seek behavior

ASeek::ASeek() : ABehavior("Seek") 
{ 
}

//
// Steer the actor TOWARDS a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 ASeek::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   vec3 pos = actor.getPosition();
   vec3 dir = target-pos;
   dir = normalize(dir);
   if(distance(pos, target) < getParam("AgentRadius")) return vec3(0);
   return getParam("MaxSpeed") * dir;
   
}

//--------------------------------------------------------------
// Flee behavior

AFlee::AFlee() : ABehavior("Flee") 
{
}

//
// Steer the actor AWAY from a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 AFlee::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{  
   vec3 pos = actor.getPosition();
   vec3 dir = targetPos-pos;
   dir = normalize(dir);
    return (-1)*getParam("MaxSpeed") * dir;
}

//--------------------------------------------------------------
// Arrival behavior

AArrival::AArrival() : ABehavior("Arrival") 
{
   // TODO: Set good parameters
   setParam("kArrival", 10);
   setParam("TargetRadius", 100);
}

//
// Returns a velocity whose speed is proportional to the actors distance
// from the target
//
// @param actor: steerable character to move
// @return desired velocity
vec3 AArrival::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   float r = getParam("TargetRadius");
   vec3 pos = actor.getPosition();
   vec3 targetOffset = targetPos - pos;
   float distance = length(targetPos);
   float speed = getParam("MaxSpeed");
   if(distance <= r) speed = (distance/r) * getParam("MaxSpeed");
   return speed * normalize(targetOffset);
}

//--------------------------------------------------------------
// Departure behavior

ADeparture::ADeparture() : ABehavior("Departure") 
{
   setParam("InnerRadius", 1);
   setParam("OuterRadius", 10);
   setParam("kDeparture", 1);
}

//
// Calculate a repelent velocity based on the actor's 
// distance from the target
vec3 ADeparture::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
   vec3 pos = actor.getPosition();
   float speed = getParam("MaxSpeed");
   float distance = length(targetPos - pos);
   if (distance <= getParam("OuterRadius"))
      speed = (distance / getParam("TargetRadius")) * getParam("MaxSpeed");

   return -normalize(targetPos-pos) * speed;
}

//--------------------------------------------------------------
// Avoid behavior

AAvoid::AAvoid() : ABehavior("Avoid") 
{
   setParam("kAvoid", 1);
}

// If an actor is near an obstacle, avoid adds either a tangential or
// normal response velocity
//  Obstacles are in getWorld()->getObstacle(i) and have class type AObstacle
vec3 AAvoid::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& targetPos)
{
    return vec3(0,0,0);
}
//--------------------------------------------------------------
// Wander behavior

AWander::AWander() : ABehavior("Wander")
{
   setParam("kWander", 1);
}

// Wander returns a velocity whose direction changes randomly (and smoothly)
vec3 AWander::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{  
   //wander strength
   float r1 = 100.0f;
   //wander rate
   float r2 = 10.0f;
   vec3 vd = actor.getDesiredVelocity();
   vec3 jitter = vec3(r2 * agl::random(-1,1), 0, r2 * agl::random(-1,1));
   vd += r1 * normalize(jitter);
   return vd; 


}

//--------------------------------------------------------------
// Separation behavior

ASeparation::ASeparation() : ABehavior("Separation")
{
   setParam("Neighborhood", 1);
   setParam("kSeparation", 1);
}


// Separation tries to maintain a constant distance between all agents
// within the neighborhood of the agent
// getWorld()->getAgent(i) returns the ith agent in the environment
// you can check whether two agents are the same with ==
vec3 ASeparation::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}


//--------------------------------------------------------------
// Cohesion behavior

ACohesion::ACohesion() : ABehavior("Cohesion")
{
   setParam("Neighborhood", 1);
   setParam("kCohesion", 1);
}

// Cohesion moves actors towards the center of a group of agents
vec3 ACohesion::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
    return vec3(0,0,0);
}

//--------------------------------------------------------------
// Alignment behavior
AAlignment::AAlignment() : ABehavior("Alignment")
{
   setParam("Neighborhood", 1);
   setParam("kAlignment", 1);
}

// Alignment returns an average velocity of all active agents
vec3 AAlignment::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
    return vec3(0);
}

//--------------------------------------------------------------
// Flocking behavior
AFlocking::AFlocking() : ABehavior("Flocking")
{
}

// Flocking combines separation, cohesion, and alignment
vec3 AFlocking::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}

//--------------------------------------------------------------
// Leader
ALeader::ALeader() : ABehavior("Leader")
{
   setParam("CSeparation", 1);
   setParam("CCohesion", 1);
   setParam("CAlignment", 1);
   setParam("CArrival", 1);
}

// You need to find the leader, who is always the first agent in agents
// If the actor is the leader, move towards the target; otherwise, 
// follow the leader as a flock
vec3 ALeader::calculateDesiredVelocity(const ASteerable& actor,
   const AWorld& world, const vec3& target)
{
   return vec3(0,0,0);
}


