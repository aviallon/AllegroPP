#pragma once

#include <allegro++/allegro.h>

namespace AllegroPP{

   class Color {
   public:
      Color(int r, int g, int b, float a = 1.0f);
      
      Color(int hex);
      
      Color();
      
      Color(bool notColor);
      
      Color operator*(double k);
      
      Color operator/(double k);
      
      Color operator+(const Color& c);
      
      Color mix(const Color& c);
      
      Color blend(const Color& c);
      
      std::string toHex() const;
      
      ALLEGRO_COLOR toAllegro();
      
      int _r, _g, _b;
      float _a;
      bool notColor = false;
   };
   
}