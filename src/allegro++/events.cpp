#include <allegro++/allegro_class.h>

namespace AllegroPP{
   
   void Allegro::_exec_mouse_clicked_function(uint16_t ev){
      if(mouse->getBtn() == 1){
         mouse_clicked_func_ptr(this, context, MOUSE_L_CLICKED | ev, mouse->getX(), mouse->getY());
      } else if(mouse->getBtn() == 2)
         mouse_clicked_func_ptr(this, context, MOUSE_R_CLICKED | ev, mouse->getX(), mouse->getY());
   }

   void Allegro::_exec_mouse_moved_function(uint16_t ev){
      if(ev == MOUSE_MOVED)
         if(!cursorSticked)
            mouse_moved_func_ptr(this, context, MOUSE_MOVED, mouse->getX(), mouse->getY());
         else
            mouse_moved_func_ptr(this, context, MOUSE_MOVED_DELTA, mouse->getDX(), mouse->getDY());
      else if (ev == MOUSE_WHEELED)
         mouse_moved_func_ptr(this, context, MOUSE_WHEELED, mouse->getDZ(), 0);
   }

   void Allegro::_exec_key_down_function(uint8_t keycode){
      //std::cout << "key down" << std::endl;
      keys[keycode] = true;
      key_down_func_ptr(this, context, KEY_DOWN, keycode);
   }

   void Allegro::_exec_key_repeat_function(){
      key_down_func_ptr(this, context, KEY_REPEAT, 0);
   }

   void Allegro::_exec_key_up_function(uint8_t keycode){
      keys[keycode] = false;
      key_up_func_ptr(this, context, KEY_UP, keycode);
   }

   void Allegro::_exec_window_resized_function(){
      window_resized_func_ptr(this, context, WINDOW_RESIZED, getDisplayWidth(), getDisplayHeight());
   }
   
   void Allegro::_exec_window_closed_function(){
      window_closed_func_ptr(this, context);
   }
   
   void Allegro::_exec_window_created_function(){
      window_created_func_ptr(this, context);
   }

   void Allegro::bindMouseClick(void (*fptr)(Allegro*, void*, uint16_t, int, int)){
      mouse_clicked_func_ptr = fptr;
   }

   void Allegro::bindMouseMove(void (*fptr)(Allegro*, void*, uint16_t, int, int)){
      mouse_moved_func_ptr = fptr;
   }

   void Allegro::bindKeyDown(void (*fptr)(Allegro*, void*, uint16_t, uint8_t)){
      key_down_func_ptr = fptr;
   }

   void Allegro::bindKeyUp(void (*fptr)(Allegro*, void*, uint16_t, uint8_t)){
      key_up_func_ptr = fptr;
   }

   void Allegro::bindWindowResized( void(*fptr)(Allegro*, void*, uint16_t, int, int) ){
      window_resized_func_ptr = fptr;
   }
   
   void Allegro::bindWindowClosed( void(*fptr)(Allegro*, void*) ){
      window_closed_func_ptr = fptr;
   }
   
   void Allegro::bindWindowCreated( void(*fptr)(Allegro*, void*) ){
      window_created_func_ptr = fptr;
   }

   bool Allegro::isKeyDown(int keycode){
      return (keys[keycode]);
   }

   bool Allegro::isMouseBtnDown(int button){
      return mouseBtns[button];
   }
   
}