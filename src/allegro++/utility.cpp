#include <allegro++/utility.h>

#define ALLEGRO_REST

namespace AllegroPP{
   #ifdef ALLEGRO_REST
   void rest(double seconds){
      al_rest(seconds);
   }
   #else
   void rest(double seconds){
      std::this_thread::sleep_for(std::chrono::microseconds(seconds*1e6));
   }
   #endif
}