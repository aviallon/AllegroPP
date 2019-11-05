#pragma once
#ifndef ALLEGRO_H_
#define ALLEGRO_H_

#include <allegro++/allegro.h>

namespace AllegroPP {
   
   using std::shared_ptr;

   class Mouse;
   class Button;
   class GUI;
   class Image;
   class Sprite;
   class SpriteMap;
   class InputBox;
   class Color;

   typedef unsigned char uchar;

   //std::this_thread::sleep_for(std::chrono)

   class Allegro
   {
   private:

   /* statics */
      static std::vector<Allegro*> instances;
      static std::stack<std::shared_ptr<ALLEGRO_THREAD> > allegro_threads;
      //static std::vector<std::thread> allegro_threads;
      static std::stack<std::shared_ptr<ALLEGRO_THREAD> > event_threads;
      //static std::vector<std::thread> event_threads;
      static unsigned loops; // Stop loop when it reaches 0
      static int currently_drawing_or_listening_for_events;
      static ALLEGRO_FILE *arial_file;
      static ALLEGRO_FONT *default_font;
      static std::vector<ALLEGRO_FONT*> fonts;
      static bool loop_started;
      static std::timed_mutex draw_text_mutex;

   /* end of statics */

      std::shared_ptr<ALLEGRO_DISPLAY> display;
      std::shared_ptr<ALLEGRO_BITMAP> display_bitmap;
      std::shared_ptr<ALLEGRO_TIMER> timer;
      std::shared_ptr<ALLEGRO_EVENT_QUEUE> event_queue;
      
      std::timed_mutex flip_display_mutex;
      
      bool event_loop_working = false;
      
      bool window_created = false;
      
      bool focus = true;
      
      std::shared_ptr<ALLEGRO_BITMAP> ancien_emplacement;
      
      void (*mouse_clicked_func_ptr)(Allegro*, void* /*context*/, uint16_t, int, int);
      void (*mouse_moved_func_ptr)(Allegro*, void* /*context*/, uint16_t, int, int);
      
      void (*key_down_func_ptr)(Allegro*, void* /*context*/, uint16_t, uint8_t);
      void (*key_up_func_ptr)(Allegro*, void* /*context*/, uint16_t, uint8_t);
      
      void (*window_resized_func_ptr)(Allegro*, void* /*context*/, uint16_t, int, int);
      void (*window_closed_func_ptr)(Allegro*, void* /*context*/);
      void (*window_created_func_ptr)(Allegro*, void* /*context*/);
      
      void (*redraw_func_ptr)(Allegro*, float /*fps*/);
      void (*animate_func_ptr)(Allegro*, float /*fps*/);
      
      void* context = nullptr;
      
      std::shared_ptr<Mouse> mouse;
//      int old_x;
//      int old_y;
      
      std::vector<bool> keys;
      std::vector<bool> mouseBtns;

      bool looping = false, redraw = false, redraw_paused = false;
      
      void _exec_mouse_clicked_function(uint16_t ev);
      void _exec_mouse_moved_function(uint16_t ev);
      
      void _exec_key_down_function(uint8_t keycode);
      void _exec_key_up_function(uint8_t keycode);
      void _exec_key_repeat_function();
      
      void _exec_window_resized_function();
      void _exec_window_closed_function();
      void _exec_window_created_function();
      
      void _stop_loop();
      void _start_loop();
      void _loop_element();
      void _event_loop_element();
      void _quit();
      static void* _loop(ALLEGRO_THREAD* thr, void* ctx);
      //static void _loop(Allegro* allegro);
      //static void _event_loop(Allegro* allegro);
      static void* _event_loop(ALLEGRO_THREAD* thr, void* ctx);
      
      ALLEGRO_EVENT_SOURCE user_generated;
      
      
      float m_FPS = 0;
      bool cursorSticked = false;
      unsigned thread_id = 0;
      unsigned frame_skipped = 0;
      bool flush_event_queue = false;
      bool timer_fired = false;
      
      double effective_frametime;
      timestamp last_frame;

   public:

      Allegro(bool start_on_copy = false);
      Allegro(const Allegro& allegro);
      
      ~Allegro();

      /**
       * @brief Inits allegro. Required before doing anything else.
       * @return Returns 0 if no errors are encountered.
       */
       static int init();
      
      
      /**
       * @brief Creates a window.
       * 
       * Only one active window is currently supported.
       * @param FPS Framerate of the window. Events will be polled FPS times a second.
       * @param w Width of the window
       * @param h Height of the window
       * @return Returns 0 if no errors are encountered.
       */
       int createWindow(float FPS, int w, int h, int flags = ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
      
      /**
       * @brief Starts the global loop. Stops when all opened windows are closed.
       */
      static void startLoop();
      
      /**
       * @brief Calls the given function when the mouse is clicked.
       * @param fptr Pointer to a function matching this footprint : void onMouseClick(Allegro* allegro, void* context, uint16_t event, int x, int y)
       */
      void bindMouseClick(void (*fptr)(Allegro*, void*, uint16_t, int, int));
      
      /**
       * @brief Calls the given function when the mouse is moved.
       * @param fptr Pointer to a function matching this footprint : void onMouseMove(Allegro* allegro, void* context, uint16_t event, int x, int y)
       */
      void bindMouseMove(void (*fptr)(Allegro*, void*, uint16_t, int, int));
      
      /**
       * @brief Calls the given function when a key is downed.
       * @param fptr Pointer to a function matching this footprint : void onKeyDown(Allegro* allegro, void* context, uint16_t event, uint8_t keycode)
       */
      void bindKeyDown(void (*fptr)(Allegro*, void*, uint16_t, uint8_t));
      
      /**
       * @brief Calls the given function when a key is upped.
       * @param fptr Pointer to a function matching this footprint : void onKeyUp(Allegro* allegro, void* context, uint16_t event, uint8_t keycode)
       */
      void bindKeyUp(void (*fptr)(Allegro*, void*, uint16_t, uint8_t));
      
      /**
       * @brief Set the function called each time the window is resized
       * @param fptr Pointer to a function matching this footprint : void onWindowResized(Allegro* allegro, void* context, uint16_t event, int w, int h)
       */
      void bindWindowResized( void(*fptr)(Allegro*, void* /*context*/, uint16_t, int, int) );
      
      /**
       * @brief Set the function called when the window is closed (either by function call or by user interaction)
       * @param fptr Pointer to a function matching this footprint : void onWindowClosed(Allegro* allegro, void* context) 
       */
      void bindWindowClosed( void(*fptr)(Allegro*, void*) );
      
      /**
       * @brief Set the function called when the window is created
       * @param fptr Pointer to a function matching this footprint : void onWindowCreated(Allegro* allegro, void* context) 
       */
      void bindWindowCreated( void(*fptr)(Allegro*, void*) );
      
      /**
       * @brief Set the function called when the screen has to be redrawn.
       * 
       * You basically draw all your stuff here. Weird glitches will happen if you draw during other events.
       * @param fptr Pointer to a function matching this footprint : void redraw(Allegro* allegro, float FPS)
       */
      void setRedrawFunction(void (*fptr)(Allegro*, float));
      
      /**
       * @brief Function called - exactly FPS times a second - for animations
       * 
       * It's better to do animations here as it won't suffer effects cause by slow rendering in the redraw function or things like that.
       * @param fptr Pointer to a function matching this footprint : void animate(Allegro* allegro, float FPS)
       */
      void setAnimateFunction(void (*fptr)(Allegro*, float));
      
      static const uint16_t MOUSE_R_CLICKED = 1<<0;
      static const uint16_t MOUSE_L_CLICKED = 1<<1;
      static const uint16_t MOUSE_DOWN = 1<<2;
      static const uint16_t MOUSE_UP = 1<<3;
      static const uint16_t MOUSE_MOVED = 1<<4;
      static const uint16_t KEY_DOWN = 1<<5;
      static const uint16_t KEY_UP = 1<<6;
      static const uint16_t KEY_REPEAT = 1<<7;
      static const uint16_t KEY_CHAR = 1<<8;
      static const uint16_t MOUSE_WHEELED = 1<<9;
      static const uint16_t MOUSE_MOVED_DELTA = 1<<10;
      static const uint16_t WINDOW_RESIZED = 1<<11;
      
      /**
       * @brief Returns an allegro color struct by giving color as Red Green Blue.
       * 
       * Values have to be between 0 and 255.
       * @param r Red color
       * @param g Green color
       * @param b Blue color
       * @return Returns an Allegro color struct
       * @deprecated
       */
      struct ALLEGRO_COLOR rgb(int r, int g, int b);
      
      /**
       * @brief Returns an allegro color struct by giving color as Red Green Blue and Alpha.
       * 
       * Values have to be between 0 and 255.
       * @param r Red color
       * @param g Green color
       * @param b Blue color
       * @param a Alpha channel - for transparency
       * @return Returns an Allegro color struct
       * @deprecated
       */
      struct ALLEGRO_COLOR rgba(int r, int g, int b, int a);
      
      /* Les fonctions pour dessiner ! Enfin ! */
      
      /**
       * @brief Sets a pixel to the specified color at the specified coordinates
       * @param x Abscissa of the pixel
       * @param y Ordinate of the pixel
       * @param color Well, the pixel's color
       */
      void set_pixel(int x, int y, ALLEGRO_COLOR color);
      void set_pixel(int x, int y, Color color);
      
      /**
       * @brief Draws a line between the two specified pixels.
       * @param x1 First pixel's abscissa
       * @param y1 First pixel's ordinate
       * @param x2 Second pixel's abscissa
       * @param y2 Second pixel's ordinate
       * @param color Line color
       * @param width Line width (in pixels). Default : 1px
       */
      void draw_line(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width = 1);
      void draw_line(int x1, int y1, int x2, int y2, Color color, int width = 1);
      
      /**
       * @brief Draws an ellipsis inside the specified rect.
       * 
       * It means that if you draw a rectangle exactly surrounding your ellipsis, then the top-left pixel will be pixel 1 and the bottom-right pixel will be pixel 2.
       * @param x1 First pixel's abscissa
       * @param y1 First pixel's ordinate
       * @param x2 Second pixel's abscissa
       * @param y2 Second pixel's ordinate
       * @param color Ellipsis color
       * @param width Ellipsis border's width. Default : 1px
       * @param filled Fills the ellipsis with its border's color. Default : false
       */
      void draw_ellipse(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width = 1, bool filled = false);
      void draw_ellipse(int x1, int y1, int x2, int y2, Color color, int width = 1, bool filled = false);
      
      /**
       * @brief Draws an ellipsis at the specified point with given radius.
       * @param cx Center's absissa
       * @param cy Center's ordinate
       * @param rx Horizontal radius. Must be positive.
       * @param ry Vertical radius. Must be positive.
       * @param color Ellipsis' color
       * @param width Ellipsis' width. Default : 1px
       * @param filled Fills the ellipsis with its border's color. Default : false
       */
      void draw_ellipse_r(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color, int width = 1, bool filled = false);
      void draw_ellipse_r(int cx, int cy, int rx, int ry, Color color, int width = 1, bool filled = false);
      
      
      /**
       * @brief Draws a rectangle.
       * @param x1 Topleft abscissa
       * @param y1 Topleft ordinate
       * @param x2 Bottom-right abscissa
       * @param y2 Bottom-right ordinate
       * @param color Rectangle's color
       * @param width Rectangle's width. Default : 1px
       * @param filled Fills the rectangle with its border's color. Default : false
       */
      void draw_rectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width = 1, bool filled = false);
      void draw_rectangle(int x1, int y1, int x2, int y2, Color color, int width = 1, bool filled = false);
      void draw_rectangle(int x1, int y1, int x2, int y2);
      
      /**
       * @brief Draws text at the specified point with the given font
       * @param x Abscissa
       * @param y Ordinate
       * @param text The text as a std::string
       * @param color Text's color
       * @param align Text align. Can be ALLEGRO_ALIGN_(LEFT|RIGHT|CENTER|CENTRE) (CENTRE and CENTER are the same)
       * @param font Font in ALLEGRO_FONT format. You can grab the default one using getDefaultFont();
       */
      void draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_CENTER, ALLEGRO_FONT* font = nullptr);
      void draw_text(int x, int y, std::string text, Color color, int align = ALLEGRO_ALIGN_CENTER, ALLEGRO_FONT* font = nullptr);
      void draw_text(int x, int y, std::string text);
      
      /**
       * @brief Draws an allegro bitmap at the specified coordinates
       * @param x Abscissa
       * @param y Ordinate
       * @param image The bitmap in ALLEGRO format.
       */
      void draw_image(int x, int y, ALLEGRO_BITMAP* image);
      void draw_image(int x, int y, Sprite image);

      
      /**
       * @brief Draws a scaled allegro bitmap at the specified coordinates
       * @param x Abscissa
       * @param y Ordinate
       * @param w New width
       * @param h New height
       * @param image The bitmap in ALLEGRO format.
       */
      void draw_scaled_image(int x, int y, int w, int h, ALLEGRO_BITMAP* image);
      void draw_scaled_image(int x, int y, int w, int h, Sprite image);

      
      /**
       * @brief WIP function. Doesn't work at all yet
       * @param x
       * @param y
       * @param w
       * @param h
       * @return 
       */
      Sprite getSubBitmapFromDisplay(int x, int y, int w, int h);
      
      /**
       * @brief Get the actuall length of a string using the specified font
       * @param text The considered text
       * @param font The font in Allegro format
       * @return Return how much pixels long the displayed string would be on screen.
       */
      int getTextWidth(std::string text, ALLEGRO_FONT* font = nullptr);
      
      /**
       * @brief Get the font's max height. Can be conbined with getTextWidth to know precisely what space a string would use on screen.
       * @param font The font in Allegro format.
       * @return Return the font height in pixels.
       */
      int getFontHeight(ALLEGRO_FONT* font = nullptr);
      
      /**
       * @brief Get the default font
       * @param fs If different from -1, load the default font and add it to fonts.
       * @return Return the font in ALLEGRO_FONT* format, useful for getTextWidth for example.
       */
      ALLEGRO_FONT* getDefaultFont(int fs=-1);
      
      /**
       * @brief Shows a little standardized display box.
       * @param title The title
       * @param heading
       * @param text The box message
       * @param buttons The buttons
       * @param flags Defaults to a yes/no behaviour
       * @return Return the number of the clicked button.
       */
      int showDialogMessage(char const *title, char const *heading, char const *text, char const *buttons, int flags = ALLEGRO_MESSAGEBOX_YES_NO);
      
      /**
       * @brief Displays a system file chooser
       * @param initial_path Default path
       * @param title
       * @param patterns File types authorized for choosing
       * @param mode Blackmagick
       * @return Returns the path of the choosen file.
       */
      const char* askFile(char const* initial_path, char const* title, char const* patterns, int mode = 0);
      
      /**
       * @brief Locks the screen buffer. Screen is only updated after it as been unlocked
       */
      void lockScreen();
      /**
       * @brief Unlocks the screen buffer.
       */
      void unlockScreen();
      
      /**
       * @brief Draw a filled, white rectangle over the whole window.
       */
      void clearScreen();
      
      /**
       * @brief Show/Hide cursor. Useful for games.
       * @param visible If set to true, show the cursor. If set to false, hide it.
       */
      void setCursorVisibility(bool visible);
      /**
       * @brief Teleport cursor to the window's center each frame. Usefull for games.
       * @param stick
       */
      void setStickCursorToCenter(bool stick);
      /**
       * @brief Set system's cursor.
       * @param id
       */
      void setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR id);
      
      /**
       * @brief Pauses window redrawing : screen won't be updated each loop, but events will still be recorded.
       */
      void stopRedraw();
      
      /**
       * @brief Resume screen redrawing. See stopRedraw()
       */
      void resumeRedraw();
      
      /**
       * @brief Copy buffer to actual display. Can't be used by two threads at once (protected by a mutex).
       */
      void flipDisplay();
      
      /**
       * @brief Get time in milliseconds since epoch.
       * @return 
       */
      long int getTime();
      
      /**
       * @brief Get effective frametime in milliseconds
       * @return 
       */
      double getFrametime();
      
      /**
       * @brief Returns mouse X coordinate from top-left of window
       * @return 
       */
      int getMouseX();
      /**
       * @brief Returns mouse Y coordinate from top-left of window
       * @return 
       */
      int getMouseY();
      
      /**
       * @brief Check if a key is currently pressed.
       * @param keycode The key's keycode in ALLEGRO format. See their documentation for more details.
       * @return 
       */
      bool isKeyDown(int keycode);
      /**
       * @brief Check mouse button pressed state.
       * @param button Mouse button id from 1 to 3, 1 is left, 2 is right and 3 is middle.
       * @return 
       */
      bool isMouseBtnDown(int button);
      
      /**
       * @brief Pass a multipurpose pointer to Allegro's instance, accessible from all Allegro functions.
       * @param cont A void* pointer.
       */
      void setContext(void* cont);
      
      /**
       * @brief Get the multipurpose pointer.
       */
      void* getContext();
      
      /**
       * @brief Explicit. Get the windows's width.
       * @return 
       */
      int getDisplayWidth();
      /**
       * @brief Get the window's height.
       * @return 
       */
      int getDisplayHeight();
      
      /**
       * @brief Activate/deactivate fullscreen based on the parameter you give.
       * @param activate If true, go into fullscreen mode.
       */
      void toggleFullscreen(bool activate);
      /**
       * @brief Returns true if the window is currently in fullscreen mode
       * @return 
       */
      bool isInFullscreen();
      
      /**
       * @brief Saves (a part of) the window's display as a .bmp file.
       * @param filename
       * @param x 
       * @param y
       * @param x2
       * @param y2
       * @return Returns true if everything went well.
       */
      bool screenshot(const char* filename, int x = 0, int y = 0, int x2 = -1, int y2 = -1);
      
      /**
       * @brief Quit current Allegro's instance.
       */
      void quit();
      
      
      unsigned getThreadId();
      
      /**
       * @brief Fake functions used as placeholders if an event/action handler is not defined
       * @param master
       * @param context
       * @param event
       * @param x
       * @param y
       */
      static void _undefined_(Allegro* master, void* context, uint16_t event, int x, int y);
      static void _undefined_(Allegro* master, void* context, uint16_t event, uint8_t keycode);
      static void _undefined_(Allegro* master, float FPS);
      static void _undefined_(Allegro* master, void* context);
      
      std::shared_ptr<GUI> gui_ptr;
      
      /**
       * @brief Simply get the GUI pointer
       * @return 
       */
      GUI* getGUI();
   };

}

#endif
