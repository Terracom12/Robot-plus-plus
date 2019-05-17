/*
* Copyright (c) 2019 Matthew Reese
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// TODO: Implement some simple STL containers

// Defining class methods
#include <math.h>
#include <stdio.h>
#include <unistd.h>

vec2_array init_array(size_t size)
{
  vec2 *data = (vec2 *)malloc(size * sizeof(vec2));
  vec2_array result = { data, size };
  return result;
}

double distance2angle(double distance, double radius)
{
  return (distance/radius)*180/M_PI;
}

Robot::Robot()
{
  m_robo = new CLinkbotI();
}
Robot::~Robot()
{
  delete m_robo;
  m_robo = NULL;
}

void Robot::init(double x, double y, double angle, double radius)
{
  m_robo->initPosition(x, y, angle);

  m_radius = radius;
}


void Robot::drive(double distance)
{
  m_robo->driveDistance(distance, m_radius);
}

void Robot::driveNB(double distance)
{
  m_robo->driveDistanceNB(distance, m_radius);
}

void Robot::driveTo(double x, double y)
{
  m_robo->drivexyTo(x, y, m_radius, TRACKWIDTH);
}
void Robot::driveToNB(double x, double y)
{
  m_robo->drivexyToNB(x, y, m_radius, TRACKWIDTH);
}

#include "pathfinder.inl"

void Robot::driveShape(int kind_shape, double side_length)
{
  if(kind_shape == CIRCLE)
  {
    turnArc(side_length, 360);
  }

  int i;
  double angle = (360/kind_shape);
  for(i = 0; i < kind_shape; i++)
  {
    drive(side_length);
    turn(angle);
  }
}

void Robot::turn(double degrees)
{
  if(degrees > 0)
    m_robo->turnRight(degrees, m_radius, TRACKWIDTH);
  else
    m_robo->turnLeft(-degrees, m_radius, TRACKWIDTH);
}
void Robot::turnNB(double degrees)
{
  if(degrees > 0)
    m_robo->turnRightNB(degrees, m_radius, TRACKWIDTH);
  else
    m_robo->turnLeftNB(-degrees, m_radius, TRACKWIDTH);
}

void Robot::turnArc(double radius, double degrees)
{
  if(radius < 2)
  {
    fputs("You cannot drive in an arc with a radius less than 2!", stderr);
    exit(-1);
  }

  // Outside wheel radius that it has to travel
  double outRad = radius + (TRACKWIDTH / 2);
  // Inside wheel radius that it has to travel
  double inRad = radius - (TRACKWIDTH / 2);

  // The distance that the joints have to move
  double _jointMoveDistance = ((M_PI*2*outRad)/360)*degrees;
  // To an angle in degrees
  double jointMoveDistance = distance2angle(_jointMoveDistance, m_radius);
  // Turning right!
  if(degrees > 0)
  {
    m_robo->setJointSpeed(JOINT3, 90*(inRad/outRad));
    m_robo->move(jointMoveDistance, NaN, -jointMoveDistance*(inRad/outRad));
    // Revert the joint speeds back
    m_robo->setJointSpeed(JOINT3, 90);
  }
  // Turning left!
  else
  {
    m_robo->setJointSpeed(JOINT1, 90*(inRad/outRad));
    m_robo->move(-jointMoveDistance*(inRad/outRad), NaN, jointMoveDistance);
    // Revert the joint speeds back
    m_robo->setJointSpeed(JOINT1, 90);
  }
}

void Robot::wait(double seconds)
{
  sleep(seconds);
}
void Robot::waitMillis(double milliseconds)
{
  sleep(milliseconds / 1000);
}

CLinkbotI *Robot::getRobot()
{
  return m_robo;
}

vec2 Robot::getPosition()
{
  // Angle is unused but has to be passed in
  double _angle, x, y;
  m_robo->getPosition(x, y, _angle);
  vec2 result = { x, y };
  return result;
}

double Robot::getRotation()
{
  // X and Y are unused but have to be passed in
  double angle, _x, _y;
  m_robo->getPosition(_x, _y, angle);
  return angle;
}
