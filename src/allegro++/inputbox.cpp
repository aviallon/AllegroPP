#include <allegro++/gui.h>

namespace AllegroPP{
   
   InputBox::InputBox(Allegro* allegro, std::string text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*)){
      this->text = text;
      this->x = x;
      this->y = y;
      this->height = height;
      this->width = width;
      this->id = id_increment;
      this->input_validated = input_validated;
      this->allegro_ptr = allegro;
      id_increment++;
      cur_pos = text.size();
   }

   void InputBox::setAuthorizedChars(std::string auth_cars){
      authorized_chars = auth_cars;
   }

   bool InputBox::isInside(int x, int y){
      if(this->x < x && this->x+width > x && this->y < y && this->y+height > y){
         return true;
      }
      return false;
   }

   void InputBox::setState(short s){
      if(s == 2){
         state = 2; // We are focused on it
         allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
         if(GUI_keyboard_capture != NULL){
            (*GUI_keyboard_capture) = id;
         }
         drawInputBox();
      } else if(s == 1){
         state = 1;
         allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
         if(GUI_keyboard_capture != NULL){
            (*GUI_keyboard_capture) = 0;
         }
         drawInputBox();
      } else {
         state = 0;
         allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
         if(GUI_keyboard_capture != NULL){
            (*GUI_keyboard_capture) = 0;
         }
         drawInputBox();
      }
   }

   int InputBox::getCurPosFromX(int x){
      uint_fast16_t prev_distance = UINT16_MAX;
      int pos = 0;
      for(unsigned i = 0; i<=text.length(); i++){
         uint_fast16_t distance = abs(x - allegro_ptr->getTextWidth(text.substr(0, i).c_str()));
         if(distance < prev_distance){
            pos = i;
            prev_distance = distance;
         }
      }
      return pos;
   }

   void InputBox::click(int x, int y, uint16_t ev){
      if (not(ev & Allegro::MOUSE_DOWN)){
         return;
      }
      if(isInside(x, y)){
         setState(2);
         int x_pos = x-(this->x + 3);
         
         cur_pos = getCurPosFromX(x_pos);
         //std::cout << getCurPosFromX(x_pos) << std::endl;
      } else {
         setState(0);
      }
   }

   void InputBox::hover(int x, int y){
      if(!allegro_ptr->isMouseBtnDown(1) && state != 2){
         if(isInside(x, y)){
            setState(1);
         } else {
            setState(0);
         }
      }
   }

   void InputBox::key_press(uint16_t ev, uint16_t keycode, char car){
      if(state != 2)
         return;
      if(ev & Allegro::KEY_CHAR){
         if(keycode == ALLEGRO_KEY_ENTER){
            input_validated(allegro_ptr, this);
         } else if(keycode == ALLEGRO_KEY_BACKSPACE){
            if(text.length() > 0){
               text.erase(text.begin()+cur_pos-1);
               //text = text.substr(0, text.length()-1);
               cur_pos = std::min(cur_pos-1, text.length());
            }
         } else if(keycode == ALLEGRO_KEY_RIGHT){
            if(cur_pos < text.length())
               cur_pos++;
         } else if(keycode == ALLEGRO_KEY_LEFT){
            if(cur_pos > 0)
               cur_pos--;
         } else {
            if(!isprint(car))
               return;
            if(authorized_chars.empty() || authorized_chars.find(car) != authorized_chars.npos){
               if(cur_pos >= 0 && cur_pos < text.length())
                  text.insert(text.begin()+cur_pos, car);
               else
                  text += car;
               cur_pos++;
            }
         }
      }
   }

   void InputBox::drawCaret(){
      int x_pos = x+allegro_ptr->getTextWidth(text.substr(0, cur_pos).c_str()) + 4;
      int y_pos = y+height/2-3;
      int h = allegro_ptr->getFontHeight(allegro_ptr->getDefaultFont());
      
      allegro_ptr->draw_line(x_pos, y_pos, x_pos, y_pos+h, allegro_ptr->rgb(0, 0, 0), 1);
      //std::cout << cur_pos << std::endl;
   }

   void InputBox::drawInputBox(){
      ALLEGRO_COLOR color;
      if(state == 1)
         color = allegro_ptr->rgb(100, 100, 200);
      else if(state == 0)
         color = allegro_ptr->rgb(0, 0, 0);
      else if(state == 2){
         color = allegro_ptr->rgb(0, 0, 255);
      }
      allegro_ptr->draw_rectangle(x, y, x+width, y+height, color, 1, false);
      allegro_ptr->draw_rectangle(x, y, x+width-1, y+height-1, allegro_ptr->rgb(255, 255, 255), 1, true);
      int len = allegro_ptr->getTextWidth(text.c_str());
      std::string printed_text = text;
      int n = text.length()-1;
      //std::cout << id << " : " << width << " px vs len = " << len << " px" << std::endl;
      bool shortenedString = false;
      while(len + 3 + 1 + allegro_ptr->getTextWidth("\u2026") >= width){
         shortenedString = true;
         n--;
         printed_text = text.substr(0, n);
         len = allegro_ptr->getTextWidth(printed_text.c_str());
      }
      if(shortenedString){
         printed_text += "\u2026"; // ellipsis symbol (...)
      }
      allegro_ptr->draw_text(x+3, y+height/2-3, printed_text, allegro_ptr->rgb(0, 0, 0), ALLEGRO_ALIGN_LEFT);
      
      if(state == 2){
         std::chrono::time_point<std::chrono::system_clock> mnt = std::chrono::system_clock::now();
         uint_fast32_t tmp = mnt.time_since_epoch().count()/1000000;
         //std::cout << tmp << std::endl;
         if( tmp%600 > 300 )
            drawCaret();
      }
   }
   
   uint16_t InputBox::id_increment = 1;
   
}