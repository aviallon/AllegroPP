#ifndef MOUSE_H_
#define MOUSE_H_

#include <allegro++/allegro.h>

namespace AllegroPP {

   class Mouse{
      
   public:
      Mouse();
      ~Mouse();
      
      int getX();
      int getY();
      
      int getDZ();
      void setDZ(int dz);
      int getDX();
      int getDY();
      void setDX(int dx);
      void setDY(int dy);
      
      int getBtn();
      void setBtn(int btn);
      
      bool hasMouseFocusOnDisplay(ALLEGRO_DISPLAY* display);
      
      void updateMouseState();
      
      struct ALLEGRO_MOUSE_STATE* getStatePtr();
      
   private:

      int mouseDZ = 0;
      int mouseDX = 0;
      int mouseDY = 0;
      int btn = 0;
      ALLEGRO_MOUSE_STATE *state;
   };

}
#endif
