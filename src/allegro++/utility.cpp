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
   
   allegro_error::allegro_error(std::string msg) : std::runtime_error(msg.c_str()) {
      std::cerr << "ERROR: " << msg << std::endl;
   }
   
   allegro_invalid_argument::allegro_invalid_argument(std::string msg) : std::invalid_argument(msg.c_str()) {
      std::cerr << "ERROR: " << msg << std::endl;
   }
}