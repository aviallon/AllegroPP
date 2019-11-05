#include <allegro++/gui.h>

namespace AllegroPP {
   
   Cursor::Cursor(Allegro* allegro, const char* filename, std::string name, int w){
      cursor = Sprite(SpriteMap(filename).getWholeSprite());
      this->w = w;
      this->h = w*cursor.getHeight()/cursor.getWidth();
      this->name = name;
   }

   Cursor::Cursor(Allegro* allegro, Sprite sprite, std::string name, int w){
      cursor = sprite;
      this->w = w;
      this->h = w*cursor.getHeight()/cursor.getWidth();
      this->name = name;
   }

   void Cursor::eraseOldCursor(){
      //allegro->draw_rectangle(this->x, this->y, this->x+w, this->y+h, allegro->rgb(255, 255, 255), 1, true);
      
      if(old_place)
         old_place.drawSprite(this->x, this->y);
   }

   void Cursor::drawCursor(int x, int y){
      //allegro->draw_rectangle(this->x, this->y, this->x+w, this->y+h, allegro->rgb(255, 255, 255), 1, true);
      
      if(old_place)
         old_place.drawSprite(this->x, this->y);
         
      this->x = x;
      this->y = y;
      //old_place = allegro->getSubBitmapFromDisplay(x, y, w, h);
      
      cursor.drawSprite(x, y, w, h);
   }
   
}