#include <allegro++/math.h>

namespace AllegroPP {
   namespace Math {
      int clamp(double x, int min, int max){
         return floor((x>max) ? max : ((x<min) ? min : x));
      }

      float clamp(double x, float min, float max){
         return (x>max) ? max : ((x<min) ? min : x);
      }
   }
}
