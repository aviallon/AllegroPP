#include <allegro++/gui.h>

namespace AllegroPP {
   
   Image::Image(){
      
   }

   Image::Image(const char* filename, int x, int y, int width, int height){
      sprite = SpriteMap(filename).getWholeSprite();
      if(sprite && (width == 0 || height == 0)){
         this->width = sprite.getWidth();
         this->height = sprite.getHeight();
      }else{
         this->width = width;
         this->height = height;
      }
      this->x = x;
      this->y = y;
   }

   void Image::drawImage(Allegro* allegro, int x, int y){
      if(x == -1 && y == -1){
         x = this->x;
         y = this->y;
      }
      if(sprite){
         sprite.drawSprite(x, y);
      }
   }

   void Image::drawScaledImage(Allegro* allegro, int x, int y, int w, int h){
      if(sprite)
         sprite.drawSprite(x, y, w, h);
   }
   
}