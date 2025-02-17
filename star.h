/***********************************************************************
 * Header File:
 *    STAR
 * Author:
 *    Br. Helfrich, Anna Arutyunova, Connor Hopkins
 * Summary:
 *    A single star that twinkles
 ************************************************************************/

#pragma once
#include "position.h"   // for POSITION
#include "uiDraw.h"     // for RANDOM and DRAWSTAR

/*****************************************************
 * STAR
 * A star that twinkles
 *****************************************************/
class Star
{
public:
   // constructors
   // default
   Star() : phase(0) {}

   // non-default
   Star(const Position& posUpperRight)
   {
      reset(posUpperRight.getX(), posUpperRight.getY());
   }

   // reset a star to random position
   void reset(double width, double height) 
   {
      pos.setX(random(0.0, width));
      pos.setY(random(0.0, height));
      phase = random(0, 255);
   }

   // show on screen
   void draw(ogstream & gout)
   {
      gout.drawStar(pos, phase++);
   }

private:
   Position pos;
   unsigned char phase = random(0.0, 255.0);
};
