#pragma once

#include <allegro++/allegro.h>

namespace AllegroPP {
   
   namespace Math {
      const double PI = 3.141592653589793238462643383279502884197169399375105820974;
      
      int clamp(double x, int min, int max);

      float clamp(double x, float min, float max);
   }
}