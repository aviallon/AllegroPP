#include <allegro++/gui.h>

namespace AllegroPP {

   GUI::GUI(Allegro* allegro) {
      this->allegro = allegro;
   }

   unsigned GUI::newBtn(std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*)){
      //buttons.push_back(Button(allegro, name, x, y, height, width, std::function<void(Allegro*, Button*)>(btn_clicked)));
      buttons.push_back(Button(allegro, name, x, y, height, width, btn_clicked));
      return buttons.size()-1;
   }
   
//   unsigned GUI::newBtn(std::string name, int x, int y, int height, int width, std::function<void(Allegro*, Button*)> btn_clicked){
//      buttons.push_back(Button(allegro, name, x, y, height, width, btn_clicked));
//      return buttons.size()-1;
//   }

   unsigned GUI::newInputBox(std::string default_text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*)){
      InputBox inpt(allegro, default_text, x, y, height, width, input_validated);
      inpt.GUI_keyboard_capture = &keyboardCapture;
      input_boxes.push_back(inpt);
      return input_boxes.size()-1;
   }

   unsigned GUI::newImage(const char* filename, int x, int y, int height, int width){
      if(std::filesystem::exists(std::filesystem::path(filename))){
         images.push_back(Image(filename, x, y, width, height));
         return images.size()-1;
      } else {
         throw new allegro_error("Image does not exist !");
      }
   }

   unsigned GUI::newCursor(const char* filename, std::string name, int w ){
      cursors.push_back(Cursor(allegro, filename, name, w));
      return cursors.size()-1;
   }

   unsigned GUI::newCursor(Sprite sprite, std::string name, int w ){
      cursors.push_back(Cursor(allegro, sprite, name, w));
      return cursors.size()-1;
   }
   
   Button* GUI::getBtn(unsigned i){
      return &buttons.at(i);
   }

   unsigned GUI::getBtnId(unsigned i){
      return buttons.at(i).id;
   }

   unsigned GUI::getInputBoxId(unsigned i){
      return input_boxes.at(i).id;
   }

   void GUI::setBtnSprites(unsigned i, Sprite defaut, Sprite clicked, Sprite hovered){
      buttons.at(i).setSprites(defaut, clicked, hovered);
   }

   void GUI::setCursor(int i){
      if(cursor >= 0){
         cursors[cursor].eraseOldCursor();
      }
      cursor = i;
   }

   void GUI::drawCursor(int x, int y){
      if(cursor >= 0)
         cursors[cursor].drawCursor(x, y);
   }

   void GUI::drawBtn(int i) {
      buttons[i].drawBtn();
   }

   void GUI::drawAllBtns(){
      for(unsigned int i=0; i<buttons.size(); i++){
         drawBtn(i);
      }
   }

   void GUI::drawInputBox(int i){
      input_boxes[i].drawInputBox();
   }

   void GUI::drawAllInputBoxes(){
      for(unsigned i = 0; i<input_boxes.size(); i++){
         drawInputBox(i);
      }
   }

   void GUI::drawImage(int i){
      //images[i].drawImage(allegro, images[i].x, images[i].y);
      //std::cout << images[i].x << ", " << images[i].y << std::endl;
      images[i].drawScaledImage(allegro, images[i].x, images[i].y, images[i].width, images[i].height);
   }

   void GUI::drawAllImages(){ // not active by default
      for(unsigned i = 0; i<images.size(); i++){
         drawImage(i);
      }
   }

   void GUI::mouseClickHandle(uint16_t ev, int x, int y){
      if(ev & Allegro::MOUSE_L_CLICKED){
         for(unsigned int i=0; i < buttons.size(); i++){
            buttons[i].click(x, y, ev);
         }
         
         for(unsigned int i=0; i < input_boxes.size(); i++){
            input_boxes[i].click(x, y, ev);
         }
      }
   }

   void GUI::mouseHoveringHandle(uint16_t ev, int x, int y){
      if(ev & Allegro::MOUSE_MOVED){
         for(unsigned int i=0; i < buttons.size(); i++){
            buttons[i].hover(x, y);
         }
         
         for(unsigned int i=0; i < input_boxes.size(); i++){
            input_boxes[i].hover(x, y);
         }
      }
   }

   void GUI::keyHandle(uint16_t ev, uint16_t key, char car){
      for(unsigned int i=0; i < input_boxes.size(); i++){
         input_boxes[i].key_press(ev, key, car);
      }
   }

   void GUI::drawLastMessage(){
      
      if (messages.size() == 0)
         return;
         
      if(!messages.front().being_drawn.try_lock_for(std::chrono::milliseconds(10))){
         return;
      }
      
      if(messages.front().t0 == 0)
         messages.front().t0 = allegro->getTime();
      
      float duree = MAX((allegro->getTime() - messages.front().t0), 0);
      if(duree >= messages.front().duration){
         messages.front().being_drawn.unlock();
         messages.pop();
         return;
      }

      /* Very cool text fade animation */
      float limit = MIN(messages.front().duration, 750);
      float alpha = 1.0f;
      if(messages.front().duration - duree <= limit){
         alpha = MAX((messages.front().duration - duree), 0)/limit;
         alpha *= alpha;
         alpha = sin(alpha*Math::PI/2);
      } else if (duree <= 250) {
         float val = duree / 250;
         alpha = sin(val*val*Math::PI/2);
      }
      Color c = Color(messages.front().color);
      c.setAlpha(alpha);
      
      allegro->draw_text(allegro->getDisplayWidth()/2, allegro->getDisplayHeight()-20, messages.front().message, c);
      
      messages.front().being_drawn.unlock();
   }

   void GUI::displayMessage(std::string message, float duration, Color color){
      messages.push(Message(message, duration, 0, color));
   }
   
   void GUI::displayMessage(std::string message, float duration){
      messages.push(Message(message, duration, 0, Color(Colors::black)));
   }

   int GUI::getBtnIndexByID(int id){
      for(unsigned i = 0; i<= buttons.size(); i++){
         if (buttons[i].id == id){
            return i;
         }
      }
      return -1;
   }

   int GUI::getInputBoxIndexByID(int id){
      for(unsigned i = 0; i<= input_boxes.size(); i++){
         if (input_boxes[i].id == id){
            return i;
         }
      }
      return -1;
   }

   void GUI::eraseBtn(Button* btn){
      for(unsigned i = 0; i<= buttons.size(); i++){
         if (buttons[i].id == btn->id){
            buttons.erase(buttons.begin() + i);
         }
      }
   }

   void GUI::eraseInputBox(InputBox* inptbx){
      for(unsigned i = 0; i<= input_boxes.size(); i++){
         if (input_boxes[i].id == inptbx->id){
            input_boxes.erase(input_boxes.begin() + i);
         }
      }
   }

}