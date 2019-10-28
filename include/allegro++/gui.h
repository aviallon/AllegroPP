#pragma once

#include <allegro++/allegro.h>
#include <functional>
#include <filesystem>

namespace AllegroPP {
   
   class Allegro;
   class Sprite;

   using std::shared_ptr;

   class Button {
   private:
      static uint16_t id_increment;
   public:
      Button(Allegro* allegro, std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*));
      Button(Allegro* allegro, std::string name, int x, int y, int height, int width, std::function<void(Allegro*, Button*)> btn_clicked);
      int x;
      int y;
      int height;
      int width;
      short state = 0;
      Allegro* allegro_ptr;
      uint16_t id;
      long data;
      std::string name;
      
      //bool use_sprites = false;
      Sprite btn_sprite;
      Sprite btn_sprite_clicked;
      Sprite btn_sprite_hovered;
      
      
      //std::function<void(Allegro*, Button*)> btn_clicked;
      void (*btn_clicked)(Allegro*, Button*);
      
      void setSprites(Sprite defaut, Sprite clicked, Sprite hovered);
      
      bool isInside(int x, int y);
      
      void click(int x, int y, uint16_t ev);
      
      void hover(int x, int y);
      
      void setState(short s);
      
      short getState();
      
      void drawBtn();
      
      static void _undefined_(Allegro* allegro, Button* btn);
   };

   class Image{
   public:
      Image();
      
      Image(const char* filename, int x = 0, int y = 0, int width = 0, int height = 0);
      
      void drawImage(Allegro* allegro, int x = -1, int y = -1);
      
      void drawScaledImage(Allegro* allegro, int x, int y, int w, int h);
      
      int x, y;
      int width;
      int height;
      Sprite sprite;
   };

   class Cursor{
   public:
      
      Cursor(Allegro* allegro, const char* filename, std::string name, int w = 32);
      
      Cursor(Allegro* allegro, Sprite sprite, std::string name, int w = 32);
      
      void eraseOldCursor();
      
      void drawCursor(int x, int y);
      
      std::string name;
      Allegro* allegro;
      int x, y;
      float w, h;
      Sprite old_place;
      Sprite cursor;
   };

   class Message{
   public:
      
      Message(std::string message, float duration, float currentTime);
      
      std::string message;
      float duration;
      float t0 = 0;
   };

   class InputBox{
   private:
      static uint16_t id_increment;
   public:
      InputBox(Allegro* allegro, std::string text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*));
      
      int x;
      int y;
      int height;
      int width;
      short state = 0;
      Allegro* allegro_ptr;
      uint16_t* GUI_keyboard_capture = NULL;
      uint16_t id;
      long data;
      std::string text;
      std::string authorized_chars = "";
      uint_fast32_t cur_pos = 0;
      
      void (*input_validated)(Allegro*, InputBox*);
      
      void setAuthorizedChars(std::string auth_cars);
      
      bool isInside(int x, int y);
      
      void setState(short s);
      
      int getCurPosFromX(int x);
      
      void click(int x, int y, uint16_t ev);
      
      void hover(int x, int y);
      
      void key_press(uint16_t ev, uint16_t keycode, char car);
      
      void drawCaret();
      
      void drawInputBox();
   };

   class GUI{
   public:
      GUI(Allegro* allegro);
      
      unsigned newBtn(std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*));
      
      //unsigned newBtn(std::string name, int x, int y, int height, int width, std::function<void(Allegro*, Button*)> btn_clicked);
      
      unsigned newInputBox(std::string default_text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*));
      
      unsigned newImage(const char* filename, int x, int y, int height = 0, int width = 0);
      
      void drawImage(int i);
      
      void drawAllImages();
      
      unsigned newCursor(const char* filename, std::string name, int w = 32);
      
      unsigned newCursor(Sprite sprite, std::string name, int w = 32);
      
      Button* getBtn(unsigned i);
      
      unsigned getBtnId(unsigned i);
      
      unsigned getInputBoxId(unsigned i);
      
      void setBtnSprites(unsigned i, Sprite defaut, Sprite clicked, Sprite hovered);
      
      void setCursor(int i);
      
      void drawCursor(int x, int y);

      void drawBtn(int i);
      
      void drawAllBtns();
      
      void drawInputBox(int i);
      
      void drawAllInputBoxes();
      
      void mouseClickHandle(uint16_t ev, int x, int y);
      
      void mouseHoveringHandle(uint16_t ev, int x, int y);
      
      void keyHandle(uint16_t ev, uint16_t key, char car);
      
      void drawLastMessage();
      
      void displayMessage(std::string message, float duration);
      
      int getBtnIndexByID(int id);
      
      int getInputBoxIndexByID(int id);
      
      void eraseBtn(Button* btn);
      
      void eraseInputBox(InputBox* inptbx);
      
      Allegro* allegro;

      int cursor = -1;
      
      uint16_t keyboardCapture = 0;
      
      std::vector<Cursor> cursors;
      std::vector<Image> images;
      std::vector<Button> buttons;
      std::vector<InputBox> input_boxes;
      std::vector<Message> messages;
   };

}