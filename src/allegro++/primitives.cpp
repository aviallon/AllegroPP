#include <allegro++/primitives.h>
//#include <thread>

namespace AllegroPP {

   std::vector<Allegro*> Allegro::instances;
   unsigned Allegro::loops; // Stop loop when it reaches 0
   ALLEGRO_COLOR Allegro::white;
   ALLEGRO_COLOR Allegro::black;
   ALLEGRO_FILE *Allegro::arial_file;
   ALLEGRO_FONT *Allegro::default_font;
   bool Allegro::loop_started = false;
   std::vector<std::thread> Allegro::allegro_threads;
   std::mutex Allegro::flip_display_mutex;
   std::timed_mutex Allegro::draw_text_mutex;

   void Allegro::_undefined_(Allegro* master, void* context, uint16_t event, int x, int y){
      // Do nothing
   }

   void Allegro::_undefined_(Allegro* master, float FPS){
      
   }

   void Allegro::_undefined_(Allegro* master, void* context, uint16_t event, uint8_t keycode){
      
   }

   long int getTime(){
      return (long int)(al_get_time()*1000);
   }

   long int Allegro::getTime(){
      return (long int)(al_get_time()*1000);
   }

   Allegro::Allegro()
   {
      display = NULL;
      display_bitmap = NULL;
       //timer = NULL;
       event_queue = NULL;
      //default_font = NULL;
      //arial_file = NULL;
      ancien_emplacement = NULL;
      mouse_clicked_func_ptr = &Allegro::_undefined_;
      mouse_moved_func_ptr = &Allegro::_undefined_;
      redraw_func_ptr = &Allegro::_undefined_;
      animate_func_ptr = &Allegro::_undefined_;
      key_down_func_ptr = &Allegro::_undefined_;
      key_up_func_ptr = &Allegro::_undefined_;
      window_resized_func_ptr = &Allegro::_undefined_;
      
      keys = std::vector<bool>(255, false);
      mouseBtns = std::vector<bool>(4, false);
      
      mouse = new Mouse();
      
      old_x = -1, old_y = -1;
      
      context = NULL;

      redraw = false, redraw_paused = false;
      
      _start_loop();
      
      focus = true;
      
      #ifdef __ENABLE_GUI__
      gui_ptr = (void*)(new GUI(this));
      #endif
      
      instances.push_back(this);
   }

   Allegro::~Allegro()
   {
      delete mouse;
      delete (GUI*)gui_ptr;
      if(loops == 0){
   //		try{
   //			al_fclose(arial_file);
   //			
   //		} catch(...){
   //			std::cerr << "Erreur lors de la fermeture de arial_file;" << std::endl;
   //			std::flush(std::cerr);
   //		}
         
         //exit(0);
      }
   }

   void Allegro::_start_loop(){
   //	al_start_timer(timer);
      looping = true;
      loops++;
      allegro_threads.push_back(std::thread(_loop, this));
      //allegro_threads[allegro_threads.size()-1].detach();
      thread_id = allegro_threads.size()-1;
   }

   void Allegro::_stop_loop(){
      looping = false;
      loops--;
      quit();
   }

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

   bool Allegro::isKeyDown(int keycode){
      return (keys[keycode]);
   }

   bool Allegro::isMouseBtnDown(int button){
      return mouseBtns[button];
   }

   void Allegro::setRedrawFunction(void (*fptr)(Allegro*, float)){
      redraw_func_ptr = fptr;
   }

   void Allegro::setAnimateFunction(void (*fptr)(Allegro*, float)){
      animate_func_ptr = fptr;
   }

   int Allegro::getMouseX(){
      return mouse->getX();
   }

   int Allegro::getMouseY(){
      return mouse->getY();
   }

   void Allegro::setContext(void* cont){
      context = cont;
   }

   void* Allegro::getContext(){
      return context;
   }

   void Allegro::set_pixel(int x, int y, ALLEGRO_COLOR color){
      al_put_pixel(x, y, color);
   }

   void Allegro::lockScreen(){
      al_lock_bitmap(display_bitmap, al_get_bitmap_format(display_bitmap), ALLEGRO_LOCK_READWRITE);
   }

   void Allegro::unlockScreen(){
      al_unlock_bitmap(display_bitmap);
   }

   void Allegro::setCursorVisibility(bool visible){
      if(!visible)
         al_hide_mouse_cursor(display);
      else
         al_show_mouse_cursor(display);
   }

   void Allegro::setStickCursorToCenter(bool stick){
      cursorSticked = stick;
      if(stick){
         al_set_mouse_xy(display, al_get_display_height(display)/2, al_get_display_width(display)/2);
         al_inhibit_screensaver(true);
         al_grab_mouse(display);
      }else{
         al_inhibit_screensaver(false);
         al_ungrab_mouse();
      }
   }

   void Allegro::setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR id){
      al_set_system_mouse_cursor(display, id);
   }

   void Allegro::quit(){
      //ALLEGRO_EVENT ev;
      //ev.type = ALLEGRO_EVENT_DISPLAY_CLOSE;
      //al_emit_user_event(&user_generated, &ev, NULL);
      
      printf("Quit...\n");
      //al_destroy_bitmap(display_bitmap);
       al_destroy_event_queue(event_queue);
       al_destroy_timer(timer);
       al_destroy_display(display);
   }

   // LIGNE
   void Allegro::draw_line(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width){
      al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, color, (float)width);
   }

   void Allegro::draw_line(int x1, int y1, int x2, int y2){
      al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, al_map_rgb(0,0,0), 1);
   }

   // ELLIPSE
   void Allegro::draw_ellipse_r(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color, int width, bool filled){
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color);
      else
         al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color, (float)width);
   }

   void Allegro::draw_ellipse(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled){
      int cx = (x1+x2)/2;
      int cy = (y1+y2)/2;
      int rx = (x2-x1)/2;
      int ry = (y2-y1)/2;
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color);
      else
         al_draw_circle((float)cx, (float)cy, (float)rx, color, (float)width);
   }

   void Allegro::draw_ellipse(int x1, int y1, int x2, int y2){
      draw_ellipse(x1, y1, x2, y2, al_map_rgb(0,0,0), 1);
   }


   // RECTANGLE
   void Allegro::draw_rectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled){
      if(filled)
         al_draw_filled_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color);
      else
         al_draw_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color, (float)width);
   }

   void Allegro::draw_rectangle(int x1, int y1, int x2, int y2){
      al_draw_rectangle((float)x1, (float)y1, (float)x2, (float)y2, al_map_rgb(0,0,0), 1);
   }

   void Allegro::draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align, ALLEGRO_FONT* font){
      const char *c_text = text.c_str();
      // Mutexes are required in these function because otherwise text becomes lost when having multiple threads
      Allegro::draw_text_mutex.try_lock_for(std::chrono::milliseconds(20));
      al_draw_text(font, color, x, y, align, c_text);
      Allegro::draw_text_mutex.unlock();
   }

   void Allegro::draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align){
      const char *c_text = text.c_str();
      Allegro::draw_text_mutex.try_lock_for(std::chrono::milliseconds(20));
      al_draw_text(default_font, color, x, y, align, c_text);
      Allegro::draw_text_mutex.unlock();
   }

   void Allegro::draw_text(int x, int y, const char* text, ALLEGRO_COLOR color, int align){
      /* Draws text on the screen at the specifed coordinates. Add "\0" at the end of your string to avoid the appearance of weird characters. */
      Allegro::draw_text_mutex.try_lock_for(std::chrono::milliseconds(20));
      al_draw_text(default_font, color, x, y, align, text);
      Allegro::draw_text_mutex.unlock();
   }

   void Allegro::draw_image(int x, int y, ALLEGRO_BITMAP* image){
      if(image == 0)
         throw(10);
      al_draw_bitmap(image, x, y, 0);
   }

   void Allegro::draw_scaled_image(int x, int y, int w, int h, ALLEGRO_BITMAP* image){
      if(image == 0)
         throw(10);
      al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), x, y, w, h, 0);
   }

   Sprite Allegro::getSubBitmapFromDisplay(int x, int y, int w, int h){
      std::cerr << "WARNING : getSubBitmapFromDisplay not working !!!" << std::endl;
      
   //	ALLEGRO_LOCKED_REGION* lock = al_lock_bitmap(display_bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
      //ALLEGRO_BITMAP* disp_clone = al_clone_bitmap(display_bitmap);

      std::shared_ptr<ALLEGRO_BITMAP> screen(al_clone_bitmap(al_create_sub_bitmap(display_bitmap, x, y, w, h)), al_destroy_bitmap);
      
      //al_unlock_bitmap(display_bitmap);
      //al_destroy_bitmap(disp_clone);
      
      return Sprite(screen);
   }

   int Allegro::getTextWidth(std::string text, ALLEGRO_FONT* font){
      return al_get_text_width(font, text.c_str());
   }

   int Allegro::getTextWidth(const char* text){
      return al_get_text_width(default_font, text);
   }

   int Allegro::getFontHeight(ALLEGRO_FONT* font){
      return al_get_font_line_height(font);
   }


   struct ALLEGRO_COLOR Allegro::rgb(int r, int g, int b){
      return al_map_rgb(r, g, b);
   }

   struct ALLEGRO_COLOR Allegro::rgba(int r, int g, int b, int a){
      return al_map_rgba(r, g, b, a);
   }

   void Allegro::stopRedraw(){
      redraw_paused = true;
   }

   void Allegro::resumeRedraw(){
      redraw_paused = false;
   }

   int Allegro::showDialogMessage(char const* title, char const* heading, char const* text, char const* buttons, int flags){
      return al_show_native_message_box(display, title, heading, text, buttons, flags);
   }

   const char* Allegro::askFile(char const* initial_path, char const* title, char const* patterns, int mode){
      ALLEGRO_FILECHOOSER* filechooser = al_create_native_file_dialog(initial_path, title, patterns, mode);
      if(al_show_native_file_dialog(display, filechooser)){
         return al_get_native_file_dialog_path(filechooser, 0);
      } else {
         return "";
      }
      
   }

   bool Allegro::screenshot(const char* filename, int x, int y, int x2, int y2){
      if(x2 < 0 || y2 < 0){
         return al_save_bitmap(filename, display_bitmap);
      } else {
         ALLEGRO_BITMAP* bmp = al_create_sub_bitmap(display_bitmap, x, y, x2-x, y2-y);
         bool res = al_save_bitmap(filename, bmp);
         al_destroy_bitmap(bmp);
         return res;
      }
   }

   int Allegro::init()
   {
       if (!al_init())
       {
           return -1;
       }

      white = al_map_rgb(255, 255, 255);
      black = al_map_rgb(0, 0, 0);
      
      al_install_keyboard();
      al_install_mouse();
       al_init_image_addon();
      al_init_primitives_addon();
      al_init_font_addon();
      al_init_ttf_addon();
      
      
      //default_font = al_load_ttf_font("arial.ttf", 25, 0);
      

   #if (defined (LINUX) || defined (__linux__)) && 0
      /* load font from memory */
      #if defined (ALLEGRO_WRAPPER_DEV)
         
         extern uint8_t arial_data[]	asm("_binary_fonts_Arimo_Regular_ttf_start");
         extern uint8_t arial_data_size[]	asm("_binary_fonts_Arimo_Regular_ttf_size");
      
      #else
      
         extern uint8_t arial_data[]	asm("_binary_allegro_fonts_Arimo_Regular_ttf_start");
         extern uint8_t arial_data_size[]	asm("_binary_allegro_fonts_Arimo_Regular_ttf_size");
      
      #endif

      size_t arial_size = (size_t)((void *)arial_data_size);

      arial_file = al_open_memfile(arial_data, (int64_t)arial_size, "r");
      default_font = al_load_ttf_font_f(arial_file, 0, 12, 0);

      /* yep, the font file is embeded in the executable at link time :D */
   #else
      /*uint8_t* arial_data;
      uint8_t* arial_data_size;
      __asm mov _binary_allegro_arial_ttf_start, [ebx]arial_data
      __asm mov _binary_allegro_arial_ttf_size, [ebx]arial_data_size*/

      default_font = al_load_ttf_font("/home/aviallon/git/AllegroPP/fonts/Arimo-Regular.ttf", 12, 0);

   #endif
      
      
      if(!default_font){
         std::cerr << "Warning ! Could not load default font !" << std::endl;
         std::flush(std::cerr);
         throw new std::logic_error("Could not load default font !");
      } else {
         //std::cout << "Succesfuly loaded arial.ttf" << std::endl;
      }

       return 0;
   }

   void Allegro::flipDisplay(){
      flip_display_mutex.lock();
      al_set_target_backbuffer(display);
      al_flip_display();
      flip_display_mutex.unlock();
   }

   int Allegro::getDisplayWidth(){
      return al_get_display_width(display);
   }

   int Allegro::getDisplayHeight(){
      return al_get_display_height(display);
   }

   ALLEGRO_FONT* Allegro::getDefaultFont(int fs){
      if(fs == -1)
         return default_font;
      else {
         #if (defined (LINUX) || defined (__linux__))
            /* arial_file already in memory */
            fonts.push_back(al_load_ttf_font_f(arial_file, 0, fs, 0));

            /* yep, the font file is embeded in the executable at link time :D */
         #endif
         #if (defined (_WIN32) || defined (_WIN64))
            fonts.push_back(al_load_ttf_font("fonts/Arimo-Regular.ttf", fs, 0));
         #endif
         
         return fonts.back();
      }
   }

   void Allegro::toggleFullscreen(bool activate){
      al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, activate);
   }

   bool Allegro::isInFullscreen(){
      return (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW);
   }

   void Allegro::clearScreen(){
      al_clear_to_color(white);
   }
   
   GUI* Allegro::getGUI(){
      if(gui_ptr != nullptr)
         return (GUI*)gui_ptr;
      else
         throw new std::logic_error("GUI is not initialized !");
   }

   int Allegro::createWindow(float FPS, int width, int height, int flags)
   {
      if(display != NULL)
         return -5;
         
      //al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
      al_set_new_display_flags(/*ALLEGRO_RESIZABLE |*/ flags);
       display = al_create_display(width, height);
       if (!display)
       {
           al_destroy_display(display);
           return -1;
       }
      
      display_bitmap = al_get_backbuffer(display);

       timer = al_create_timer(1.0 / FPS);
       if (!timer)
       {
           al_destroy_timer(timer);
           al_destroy_display(display);
           return -1;
       }

       event_queue = al_create_event_queue();
       if (!event_queue)
       {
           al_destroy_event_queue(event_queue);
           al_destroy_timer(timer);
           al_destroy_display(display);
           return -1;
       }
      
      al_init_user_event_source(&user_generated);
      al_register_event_source(event_queue, &user_generated);

       al_register_event_source(event_queue, al_get_display_event_source(display));
       al_register_event_source(event_queue, al_get_timer_event_source(timer));
       al_register_event_source(event_queue, al_get_keyboard_event_source());
      al_register_event_source(event_queue, al_get_mouse_event_source());

      al_clear_to_color(al_map_rgb(255, 255, 255));
      al_flip_display();
      
      m_FPS = FPS;
      
      //std::cout << toString(KEY_UP) << std::endl;
      
       return 0;
   }

   void Allegro::_loop_element(){
      if(display == NULL)
         return;
      
      //ALLEGRO_BITMAP display_buffer = al_clone_bitmap(display_bitmap);
      //auto lock = al_lock_bitmap(display_bitmap);
      al_set_target_backbuffer(display);
      
      if(!al_get_timer_started(timer))
            al_start_timer(timer);
      
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
      
      if(ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN){
         focus = true;
      } else if(ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){
         focus = false;
      }
      
      if(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE){
         al_acknowledge_resize(display);
         _exec_window_resized_function();
      }
      if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) { 
         al_get_mouse_state(mouse->getStatePtr());
         mouse->setDZ(ev.mouse.dz);
         mouse->setDX(ev.mouse.dx);
         mouse->setDY(ev.mouse.dy);
         
         if(mouse->getStatePtr()->display == display){
         
            if(ev.mouse.dz != 0){
               _exec_mouse_moved_function(MOUSE_WHEELED);
            }
            _exec_mouse_moved_function(MOUSE_MOVED);
            
            #ifdef __ENABLE_GUI__
            getGUI()->drawCursor(mouse->getX(), mouse->getY());
            
            getGUI()->mouseHoveringHandle(MOUSE_MOVED, mouse->getX(), mouse->getY());
            #endif
         
         }
         
         if(cursorSticked)
            al_set_mouse_xy(display, al_get_display_height(display)/2, al_get_display_width(display)/2);
         
      }
      if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
         al_get_mouse_state(mouse->getStatePtr()); // Needed to avoid visual bugs with GUI
         
         mouseBtns[ev.mouse.button] = true;
         mouse->setBtn(ev.mouse.button);
         
         if(focus){
            #ifdef __ENABLE_GUI__
            int btnEv = 0;
            if(mouse->getBtn() == 1)
               btnEv = MOUSE_L_CLICKED;
            else
               btnEv = MOUSE_R_CLICKED;
               
            getGUI()->mouseClickHandle(btnEv | MOUSE_DOWN, mouse->getX(), mouse->getY());
            getGUI()->mouseHoveringHandle(MOUSE_MOVED, mouse->getX(), mouse->getY());
            #endif
            
            _exec_mouse_clicked_function(MOUSE_DOWN);
            
         }
      }else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
         al_get_mouse_state(mouse->getStatePtr()); // Needed to avoid visual bugs with GUI
         
         mouseBtns[ev.mouse.button] = false;
         mouse->setBtn(ev.mouse.button);
         
         if(focus){
            #ifdef __ENABLE_GUI__
            int btnEv = 0;
            if(mouse->getBtn() == 1)
               btnEv = MOUSE_L_CLICKED;
            else
               btnEv = MOUSE_R_CLICKED;
               
            getGUI()->mouseClickHandle(btnEv | MOUSE_UP, mouse->getX(), mouse->getY());
            getGUI()->mouseHoveringHandle(MOUSE_MOVED, mouse->getX(), mouse->getY());
            #endif
            
            _exec_mouse_clicked_function(MOUSE_UP);
         }
      }
      if(focus){
         if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            
            #ifdef __ENABLE_GUI__
            getGUI()->keyHandle(KEY_DOWN, ev.keyboard.keycode, 0);
            
            if(getGUI()->keyboardCapture == 0){
            #endif
               _exec_key_down_function(ev.keyboard.keycode);
            #ifdef __ENABLE_GUI__
            }
            #endif
         } else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            
            #ifdef __ENABLE_GUI__
            getGUI()->keyHandle(KEY_UP, ev.keyboard.keycode, 0);
            
            if(getGUI()->keyboardCapture == 0){
            #endif
               _exec_key_up_function(ev.keyboard.keycode);
            #ifdef __ENABLE_GUI__
            }
            #endif
         }
         
         if(ev.type == ALLEGRO_EVENT_KEY_CHAR){
            #ifdef __ENABLE_GUI__
            getGUI()->keyHandle(KEY_CHAR, ev.keyboard.keycode, ev.keyboard.unichar);
            #endif
            //ev.keyboard.unichar;
         }
      }
      
      if(ev.type == ALLEGRO_EVENT_TIMER)
      {
         _exec_key_repeat_function();
         
         animate_func_ptr(this, m_FPS);
         
         if(!redraw_paused)
            redraw = true;
         else{
            al_flip_display();
         }
      }
      else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      {
         _stop_loop();
         
         //delete this;
         return;
      }

      if (redraw && al_is_event_queue_empty(event_queue))
      {
         redraw = false;
         
         ALLEGRO_BITMAP* display_bmp_clone = al_clone_bitmap(display_bitmap);
         al_set_target_bitmap(display_bmp_clone);
         
         redraw_func_ptr(this, m_FPS);
         
         #ifdef __ENABLE_GUI__
         
         getGUI()->drawAllBtns();
         
         getGUI()->drawAllInputBoxes();
         
         getGUI()->drawLastMessage();
         
         getGUI()->drawCursor(mouse->getX(), mouse->getY());
         //old_x = mouse->getX();
         //old_y = mouse->getY();
         #endif
         
         //lockScreen();
         flip_display_mutex.lock();
         al_set_target_bitmap(display_bitmap);
         al_draw_bitmap(display_bmp_clone, 0, 0, 0);
         al_flip_display();
         flip_display_mutex.unlock();
         al_destroy_bitmap(display_bmp_clone);
         //unlockScreen();
      }
   }

   void Allegro::_loop(Allegro* allegro){
      
      while(!loop_started){
         std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      
      while(allegro->looping){
         allegro->_loop_element();
      }
      
   }

   void Allegro::startLoop()
   {
      loop_started = true;
      
      while(loops > 0){
         std::this_thread::sleep_for(std::chrono::microseconds(1000));
      }
      
      for(std::thread& allegro_thd : allegro_threads){
         allegro_thd.join();
      }
   }
   
}
