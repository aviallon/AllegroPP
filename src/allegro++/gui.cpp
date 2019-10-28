#include <allegro++/gui.h>

namespace AllegroPP {


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

   Message::Message(std::string message, float duration, float currentTime){
      this->message = message;
      this->duration = duration;
      this->t0 = currentTime;
   }

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

   uint16_t Button::id_increment = 1;
   uint16_t InputBox::id_increment = 1;

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
         throw new std::logic_error("Image does not exist !");
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
      
      Message lastMessage = messages[messages.size() - 1];
      
      float duree = MAX((allegro->getTime() - lastMessage.t0), 0);
      if(duree >= lastMessage.duration){
         messages.pop_back();
         return;
      }

      int alpha = 0;
      if(lastMessage.duration - duree <= 1000){
         alpha = -255*MAX((lastMessage.duration - duree), 0)/1000;
      }
      
      allegro->draw_text(allegro->getDisplayWidth()/2, allegro->getDisplayHeight()-20, lastMessage.message, allegro->rgba(255-alpha, 255-alpha, 255-alpha, 0));
      if(duree >= lastMessage.duration){
         messages.pop_back();
      }
   }

   void GUI::displayMessage(std::string message, float duration){
      messages.push_back(Message(message, duration, allegro->getTime()));
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