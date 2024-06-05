/**********************************************************************
 * LAB 06
 * Lunar Lander simulation. This is the Game class and main()
 * Author:
 *    Br. Helfrich, Anna Arutyunova, Connor Hopkins
 **********************************************************************/

#include "position.h"    // everything should have a point
#include "acceleration.h"// for ACCELERATION
#include "lander.h"      // for LANDER
#include "star.h"        // for STAR
#include "uiInteract.h"  // for INTERFACE
#include "uiDraw.h"      // for RANDOM and DRAW*
#include "ground.h"      // for GROUND
#include "test.h"        // for the unit tests
#include <cmath>         // for SQRT
#include <cassert>       // for ASSERT
#include <vector>        // for 50 random stars
#include <iomanip>       // for setprecision()
using namespace std;


/*************************************************************************
 * SIMULATOR
 * Everything pertaining to the simulator.
 *************************************************************************/
class Simulator
{
public:
   Simulator(const Position & posUpperRight) : ground(posUpperRight), lander(posUpperRight)
   {
      for (int i = 0; i < 50; i++)
      {
         Star star(posUpperRight);
         stars.push_back(star);
      }
   }

   Angle a;
   Ground ground;
   Position posLander;
   vector<Star> stars;
   Lander lander;
   Thrust thrust;
};

/*************************************
 * CALLBACK
 * Handle one frame of the simulator
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. 
   // This is the first step of every single callback function 
   // in OpenGL. 
   Simulator * pSimulator = (Simulator *)p;

   ogstream gout;

   // set lander's thrust and acceleration to update when on screen
   pSimulator->thrust.set(pUI);
   Acceleration accel = pSimulator->lander.input(pSimulator->thrust, -1.0);
   pSimulator->lander.coast(accel, 0.1);

   // draw 50 stars
   for (Star& star : pSimulator->stars)
   {
      star.draw(gout);
   }

   // draw the ground
   pSimulator->ground.draw(gout);

   // draw the lander
   pSimulator->lander.draw(pSimulator->thrust, gout);
   
   // draw data in top left of screen
   gout = Position(20, 375);

   int altitude = round(
                  pSimulator->ground.getElevation(
                  pSimulator->lander.getPosition()));

   gout << "Fuel:       " << pSimulator->lander.getFuel() << " lbs" << endl;
   gout << "Altitude:   " << altitude << " meters" << endl;
   gout << "Speed:    "   << fixed << setprecision(2)
        << pSimulator->lander.getSpeed() << " m/s" << endl;

   // Variables to check our status without hurting our eyes
   Position pos    = pSimulator->lander.getPosition();
   int landerWidth = pSimulator->lander.getWidth   ();
   double speed    = pSimulator->lander.getSpeed   ();
   double maxSpeed = pSimulator->lander.getMaxSpeed();
   bool onPlatform = pSimulator->ground.onPlatform (pos, landerWidth);
   bool hitGround  = pSimulator->ground.hitGround  (pos, landerWidth);

   // We landed!
   if (onPlatform == true && speed <= maxSpeed)
   {
      pSimulator->lander.land();
      gout = Position(200, 200);
      gout << "The Eagle has landed!" << endl;
   }

   // We crashed :(
   if (hitGround == true)
   {
      pSimulator->lander.crash();
      gout = Position(200, 200);
      gout << "Houston, we have a problem!" << endl;
   }
}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my LM type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Run the unit tests
   testRunner();

   // Initialize OpenGL
   Position posUpperRight(400, 400);
   Interface ui("Lunar Lander", posUpperRight);

   // Initialize the game class
   Simulator simulator(posUpperRight);

   // Set everything into action
   if (simulator.lander.isFlying())
      ui.run(callBack, (void *)&simulator);

   return 0;
}
