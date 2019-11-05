#include <allegro++/gui.h>

namespace AllegroPP {
   
   Sprite::Sprite(){
      
   }
   
   Sprite::Sprite(ALLEGRO_BITMAP* bmp){
      if(bmp == nullptr)
         throw new allegro_invalid_argument("Your bitmap pointer is NULL !");
         
      sprite = shared_ptr<ALLEGRO_BITMAP>(bmp, al_destroy_bitmap);
      defined = true;
   }
   
   Sprite::Sprite(shared_ptr<ALLEGRO_BITMAP> sprt){
      sprite = sprt;
      defined = true;
   }
   
   void Sprite::drawSprite(int x, int y, unsigned w, unsigned h, bool alignCenter){
      float sw = al_get_bitmap_width(sprite.get());
      float sh = al_get_bitmap_height(sprite.get());
      if(w + h == 0){
         if(alignCenter){
            x = x-sw/2;
            y = y-sh/2;
         }
         al_draw_bitmap(sprite.get(), x, y, 0);
      } else {
         if(alignCenter){
            x = x-w/2;
            y = y-h/2;
         }
         al_draw_scaled_bitmap(sprite.get(), 0, 0, sw, sh, x, y, w, h, 0);
      }
   }
   
   float Sprite::getWidth(){
      return al_get_bitmap_width(sprite.get());
   }
   
   float Sprite::getHeight(){
      return al_get_bitmap_height(sprite.get());
   }
   
   Sprite::operator bool(){
      return defined;
   }
   
   bool Sprite::isDefined(){
      return defined;
   }
   
   SpriteMap::SpriteMap(){
      
   }
   
   SpriteMap::SpriteMap(std::string rel_path){
      this->rel_path = rel_path;
      ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
      al_remove_path_component(path, -1);
      al_set_path_filename(path, rel_path.c_str());
      spritemap = shared_ptr<ALLEGRO_BITMAP>(al_load_bitmap(al_path_cstr(path, '/')), al_destroy_bitmap);
   }
   
   Sprite SpriteMap::getSprite(int x, int y, int w, int h){
      shared_ptr<ALLEGRO_BITMAP> sprt(al_create_sub_bitmap(spritemap.get(), x, y, w, h), al_destroy_bitmap);
      return Sprite(sprt);
   }
   
   Sprite SpriteMap::getWholeSprite(){
      shared_ptr<ALLEGRO_BITMAP> sprt(spritemap.get(), al_destroy_bitmap);
      return Sprite(sprt);
   }
}