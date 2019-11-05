#include <allegro++/gui.h>

namespace AllegroPP {
   
   Message::Message(std::string message, float duration, float currentTime, Color color)
      : message(message), duration(duration), t0(currentTime) {
//      this->message = message;
//      this->duration = duration;
//      this->t0 = currentTime;
      this->color = color.toAllegro();
   }
   
   Message::Message(const Message& msg){
      this->message = msg.message;
      this->color = msg.color;
      this->duration = msg.duration;
      this->t0 = msg.t0;
   }
   
   Message::~Message(){
      while(!this->being_drawn.try_lock_for(std::chrono::milliseconds(10))){
         std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
   }
   
}