/***********************************************************************
 * Source File:
 *    LANDER
 * Author:
 *    Br. Helfrich, Anna Arutyunova, Connor Hopkins
 * Summary:
 *    All the information about the lunar lander
 ************************************************************************/

#include "lander.h"
#include "acceleration.h"

 /***************************************************************
  * RESET
  * Reset the lander and its position to start the game over
  ***************************************************************/
void Lander :: reset(const Position & posUpperRight)
{
   // straight up
   angle.setUp();
   
   // the velocity is random
   velocity.setDX(random(-10.0, -4.0));
   velocity.setDY(random(-2.0, 2.0));

   // the position is at the right side of the screen
   pos.setX(posUpperRight.getX() - 1.0);
   pos.setY(random(0.75 * posUpperRight.getY(), 0.95 * posUpperRight.getY()));
   
   status = PLAYING;
   fuel = FUEL_MAX;
}

/***************************************************************
 * DRAW
 * Draw the lander on the screen
 ***************************************************************/
void Lander :: draw(const Thrust & thrust, ogstream & gout) const
{
   gout.drawLander(pos, angle.getRadians());
   if (isFlying() && fuel > 0.0)
      gout.drawLanderFlames(pos, angle.getRadians(), 
         thrust.isMain(), thrust.isClock(), thrust.isCounter());

   if (isLanded())
   {
      gout = Position(200, 200);
      gout << "The Eagle has landed!\n";
   }

   if (isDead())
   {
      gout = Position(200, 200);
      gout << "Houston, we have a problem!\n";
   }
}

/***************************************************************
 * INPUT
 * Accept input from the Neil Armstrong
 ***************************************************************/
Acceleration Lander :: input(const Thrust& thrust, double gravity)
{
   // Acceleration due to gravity
   Acceleration a;
   
   // add gravity
   a.addDDY(gravity);
   
   // are we out of gas?
   if (fuel == 0.0)
      return a;

   // main engines
   if (thrust.isMain())
   {
      double power = (LANDER_THRUST / LANDER_WEIGHT);
      a.addDDX(-sin(angle.getRadians()) * power);
      a.addDDY(cos(angle.getRadians()) * power);
      fuel -=  FUEL_MAIN_THRUST;
   }
      
   // clockwise
   if (thrust.isClock())
   {
      angle.add(0.1);
      fuel -= FUEL_ROTATE;
   }

   // counterclockwise
   if (thrust.isCounter())
   {
      angle.add(-0.1);
      fuel -= FUEL_ROTATE;
   }

   if (fuel < 0.0)
      fuel = 0.0;

      return a;
}

/******************************************************************
 * COAST
 * What happens when we coast?
 * s = s_0 + vt + 1/2 a t^2
 *******************************************************************/
void Lander :: coast(Acceleration & acceleration, double time)
{
   if (!isFlying())
      return;

   pos.add(acceleration, velocity, time);
   velocity.add(acceleration, time);
}
