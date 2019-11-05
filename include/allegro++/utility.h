#pragma once

#include <allegro++/allegro.h>
#include <chrono>

namespace AllegroPP {
   void rest(double seconds);
   
   class allegro_error : std::runtime_error {
   public:
      allegro_error(std::string msg = "");
   };
   
   class allegro_invalid_argument : std::invalid_argument {
   public:
      allegro_invalid_argument(std::string msg = "");
   };
}