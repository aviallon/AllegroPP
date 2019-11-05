#include <allegro++/gui.h>

namespace AllegroPP{

   Button::Button(Allegro* allegro, std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*) = nullptr){
      this->name = name;
      this->x = x;
      this->y = y;
      this->height = height;
      this->width = width;
      this->id = id_increment;
      if(btn_clicked == nullptr)
         btn_clicked = &Button::_undefined_;
      this->btn_clicked = btn_clicked;
      this->allegro_ptr = allegro;
      id_increment++;
   }
   
//   Button::Button(Allegro* allegro, std::string name, int x, int y, int height, int width, std::function<void(Allegro*, Button*)> btn_clicked){
//      this->name = name;
//      this->x = x;
//      this->y = y;
//      this->height = height;
//      this->width = width;
//      this->id = id_increment;
//      this->btn_clicked = btn_clicked;
//      this->allegro_ptr = allegro;
//      id_increment++;
//   }

   void Button::drawBtn(){
      Color color;
      Sprite* sprt;
      if(state == 1){
         color = Color(100, 100, 200);
         sprt = &btn_sprite_clicked;
      }else if(state == 0){
         color = Color(100, 100, 100);
         sprt = &btn_sprite;
      }else if(state == 2){
         color = Color(100, 100, 150);
         sprt = &btn_sprite_hovered;
      }
      if(!btn_sprite){
         allegro_ptr->draw_rectangle(x, y, x+width, y+height, color.toAllegro(), 1, true);
      } else {
         sprt->drawSprite(x, y, width, height);
      }
      allegro_ptr->draw_text(x+width/2, y+height/2, name, Color(0, 0, 0).toAllegro());
   }

   void Button::hover(int x, int y){
      if(!allegro_ptr->isMouseBtnDown(1)){
         if(isInside(x, y)){
            state = 2;
         } else {
            state = 0;
         }
      }
   }

   void Button::click(int x, int y, uint16_t ev){
      if(isInside(x, y) && (ev & Allegro::MOUSE_UP) && state == 1){
         state = 2;
         btn_clicked(allegro_ptr, this);
      } else if (isInside(x, y)){
         state = 1;
      }
   }

   bool Button::isInside(int x, int y){
      if(this->x < x && this->x+width > x && this->y < y && this->y+height > y){
         return true;
      }
      return false;
   }

   void Button::setSprites(Sprite defaut, Sprite clicked, Sprite hovered){
      btn_sprite = defaut;
      btn_sprite_clicked = clicked;
      btn_sprite_hovered = hovered;
   }
   
   void Button::_undefined_(Allegro* allegro, Button* btn){
      return;
   }
   
   void Button::setState(short s){
      this->state = Math::clamp(s, 0, 3);
   }
   
   short Button::getState(){
      return state;
   }
   
   uint16_t Button::id_increment = 1;
   
}