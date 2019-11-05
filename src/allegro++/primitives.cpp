#include <allegro++/allegro_class.h>
//#include <thread>

namespace AllegroPP {

   std::vector<Allegro*> Allegro::instances;
   unsigned Allegro::loops = 0; // Stop loop when it reaches 0
   int Allegro::currently_drawing_or_listening_for_events = 0;
   
   ALLEGRO_FILE *Allegro::arial_file = nullptr;
   ALLEGRO_FONT *Allegro::default_font = nullptr;
   bool Allegro::loop_started = false;
   //std::vector<std::thread> Allegro::allegro_threads;
   std::stack<std::shared_ptr<ALLEGRO_THREAD> > Allegro::allegro_threads;
   std::stack<std::shared_ptr<ALLEGRO_THREAD> > Allegro::event_threads;
   //std::vector<std::thread> Allegro::event_threads;
   //std::timed_mutex Allegro::flip_display_mutex;
   std::timed_mutex Allegro::draw_text_mutex;
   
   template<class T>
   bool dont_destroy(T*){
      return false;
   }

   long int getTime(){
      return (long int)(al_get_time()*1000);
   }

   long int Allegro::getTime(){
      return (long int)(al_get_time()*1000);
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
      context = (void*)cont;
   }

   void* Allegro::getContext(){
      if(context == nullptr)
         throw new std::runtime_error("Context is not set !");
      return context;
   }

   void Allegro::set_pixel(int x, int y, ALLEGRO_COLOR color){
      al_put_pixel(x, y, color);
   }

   void Allegro::lockScreen(){
      al_lock_bitmap(display_bitmap.get(), al_get_bitmap_format(display_bitmap.get()), ALLEGRO_LOCK_READWRITE);
   }

   void Allegro::unlockScreen(){
      al_unlock_bitmap(display_bitmap.get());
   }

   void Allegro::setCursorVisibility(bool visible){
      if(!visible)
         al_hide_mouse_cursor(display.get());
      else
         al_show_mouse_cursor(display.get());
   }

   void Allegro::setStickCursorToCenter(bool stick){
      cursorSticked = stick;
      if(stick){
         al_set_mouse_xy(display.get(), al_get_display_height(display.get())/2, al_get_display_width(display.get())/2);
         al_inhibit_screensaver(true);
         al_grab_mouse(display.get());
      }else{
         al_inhibit_screensaver(false);
         al_ungrab_mouse();
      }
   }

   void Allegro::setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR id){
      al_set_system_mouse_cursor(display.get(), id);
   }

   void Allegro::quit(){
      ALLEGRO_EVENT ev;
      ev.type = ALLEGRO_EVENT_DISPLAY_CLOSE;
      al_emit_user_event(&user_generated, &ev, NULL);
   }
      
   void Allegro::_quit(){
      
      printf("Quit...\n");
      //al_destroy_bitmap(display_bitmap);
      bool looping = false;
      unsigned counter = 0;
      while(event_loop_working && counter < 100){
         std::this_thread::sleep_for(std::chrono::microseconds(500));
         counter++;
         //std::cout << "Waiting to quit..." << std::endl;
      }
      event_loop_working = true;
      //display.reset();
      al_stop_timer(timer.get());
      flip_display_mutex.lock();
      if(event_queue)
         al_destroy_event_queue(event_queue.get());
      if(timer)
         al_destroy_timer(timer.get());
      if(display)
         al_destroy_display(display.get());
      flip_display_mutex.unlock();
   }

   // LIGNE
   void Allegro::draw_line(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width){
      al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, color, (float)width);
   }
   
   void Allegro::draw_line(int x1, int y1, int x2, int y2, Color color, int width){
      al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, color.toAllegro(), (float)width);
   }

   // ELLIPSE
   void Allegro::draw_ellipse_r(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color, int width, bool filled){
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color);
      else
         al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color, (float)width);
   }
   
   void Allegro::draw_ellipse_r(int cx, int cy, int rx, int ry, Color color, int width, bool filled){
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color.toAllegro());
      else
         al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color.toAllegro(), (float)width);
   }

   void Allegro::draw_ellipse(int x1, int y1, int x2, int y2, Color color, int width, bool filled){
      int cx = (x1+x2)/2;
      int cy = (y1+y2)/2;
      int rx = (x2-x1)/2;
      int ry = (y2-y1)/2;
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color.toAllegro());
      else
         al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color.toAllegro(), (float)width);
   }
   
   void Allegro::draw_ellipse(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled){
      int cx = (x1+x2)/2;
      int cy = (y1+y2)/2;
      int rx = (x2-x1)/2;
      int ry = (y2-y1)/2;
      if(filled)
         al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color);
      else
         al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color, (float)width);
   }


   // RECTANGLE
   void Allegro::draw_rectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled){
      if(filled)
         al_draw_filled_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color);
      else
         al_draw_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color, (float)width);
   }
   
   void Allegro::draw_rectangle(int x1, int y1, int x2, int y2, Color color, int width, bool filled){
      if(filled)
         al_draw_filled_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color.toAllegro());
      else
         al_draw_rectangle((float)x1, (float)y1, (float)x2, (float)y2, color.toAllegro(), (float)width);
   }

   void Allegro::draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align, ALLEGRO_FONT* font){
      if(font == nullptr)
         font = default_font;
      
      const char *c_text = text.c_str();
      // Mutexes are required in these function because otherwise text becomes lost when having multiple threads
      Allegro::draw_text_mutex.try_lock_for(std::chrono::milliseconds(20));
      al_draw_text(font, color, x, y, align, c_text);
      Allegro::draw_text_mutex.unlock();
   }
   
   void Allegro::draw_text(int x, int y, std::string text, Color color, int align, ALLEGRO_FONT* font){
      if(font == nullptr)
         font = default_font;
      
      const char *c_text = text.c_str();
      // Mutexes are required in these function because otherwise text becomes lost when having multiple threads
      Allegro::draw_text_mutex.try_lock_for(std::chrono::milliseconds(20));
      al_draw_text(font, color.toAllegro(), x, y, align, c_text);
      Allegro::draw_text_mutex.unlock();
   }
   
   void Allegro::draw_text(int x, int y, std::string text){
      return draw_text(x, y, text, Color());
   }

   void Allegro::draw_image(int x, int y, ALLEGRO_BITMAP* image){
      if(image == nullptr)
         throw new std::invalid_argument("Provided bitmap is nullptr !");
      al_draw_bitmap(image, x, y, 0);
   }
   
   void Allegro::draw_image(int x, int y, Sprite image){
      image.drawSprite(x, y);
   }

   void Allegro::draw_scaled_image(int x, int y, int w, int h, ALLEGRO_BITMAP* image){
      if(image == nullptr)
         throw new std::invalid_argument("Provided bitmap is nullptr !");
         
      al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), x, y, w, h, 0);
   }
   
   void Allegro::draw_scaled_image(int x, int y, int w, int h, Sprite image){
      image.drawSprite(x, y, w, h);         
   }

   Sprite Allegro::getSubBitmapFromDisplay(int x, int y, int w, int h){
      std::cerr << "WARNING : getSubBitmapFromDisplay not working !!!" << std::endl;
      
   //	ALLEGRO_LOCKED_REGION* lock = al_lock_bitmap(display_bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
      //ALLEGRO_BITMAP* disp_clone = al_clone_bitmap(display_bitmap);

      std::shared_ptr<ALLEGRO_BITMAP> screen(al_clone_bitmap(al_create_sub_bitmap(display_bitmap.get(), x, y, w, h)), al_destroy_bitmap);
      
      //al_unlock_bitmap(display_bitmap);
      //al_destroy_bitmap(disp_clone);
      
      return Sprite(screen);
   }

   int Allegro::getTextWidth(std::string text, ALLEGRO_FONT* font){
      if(font == nullptr)
         font = default_font;
      return al_get_text_width(font, text.c_str());
   }

   int Allegro::getFontHeight(ALLEGRO_FONT* font){
      if(font == nullptr)
         font = default_font;
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
      return al_show_native_message_box(display.get(), title, heading, text, buttons, flags);
   }

   const char* Allegro::askFile(char const* initial_path, char const* title, char const* patterns, int mode){
      ALLEGRO_FILECHOOSER* filechooser = al_create_native_file_dialog(initial_path, title, patterns, mode);
      if(al_show_native_file_dialog(display.get(), filechooser)){
         return al_get_native_file_dialog_path(filechooser, 0);
      } else {
         return "";
      }
      
   }

   bool Allegro::screenshot(const char* filename, int x, int y, int x2, int y2){
      if(x2 < 0 || y2 < 0){
         return al_save_bitmap(filename, display_bitmap.get());
      } else {
         ALLEGRO_BITMAP* bmp = al_create_sub_bitmap(display_bitmap.get(), x, y, x2-x, y2-y);
         bool res = al_save_bitmap(filename, bmp);
         al_destroy_bitmap(bmp);
         return res;
      }
   }

   void Allegro::flipDisplay(){
//      while(!flip_display_mutex.try_lock_for(std::chrono::milliseconds(1)))
//         {}
      currently_drawing_or_listening_for_events++;
      al_set_target_backbuffer(display.get());
      al_flip_display();
      currently_drawing_or_listening_for_events--;
//      flip_display_mutex.unlock();
   }

   int Allegro::getDisplayWidth(){
      return al_get_display_width(display.get());
   }

   int Allegro::getDisplayHeight(){
      return al_get_display_height(display.get());
   }

   ALLEGRO_FONT* Allegro::getDefaultFont(int fs){
      if(fs == -1)
         return default_font;
      else {
//         #if (defined (LINUX) || defined (__linux__) || 0)
//            /* arial_file already in memory */
//            fonts.push_back(al_load_ttf_font_f(arial_file, 0, fs, 0));
//
//            /* yep, the font file is embeded in the executable at link time :D */
//         #endif
//         #if (defined (_WIN32) || defined (_WIN64))
//            fonts.push_back(al_load_ttf_font("fonts/Arimo-Regular.ttf", fs, 0));
//         #endif
         
         return fonts.back();
      }
   }

   void Allegro::toggleFullscreen(bool activate){
      al_toggle_display_flag(display.get(), ALLEGRO_FULLSCREEN_WINDOW, activate);
   }

   bool Allegro::isInFullscreen(){
      return (al_get_display_flags(display.get()) & ALLEGRO_FULLSCREEN_WINDOW);
   }

   void Allegro::clearScreen(){
      al_clear_to_color(Color(Colors::white).toAllegro());
   }
   
   double Allegro::getFrametime(){
      return effective_frametime * 1e3;
   }
   
   unsigned Allegro::getThreadId(){
      return thread_id;
   }
   
   GUI* Allegro::getGUI(){
      if(gui_ptr)
         return gui_ptr.get();
      else
         throw new std::runtime_error("GUI is not initialized !");
   }

   int Allegro::createWindow(float FPS, int width, int height, int flags)
   {
      if(display || window_created)
         throw new std::runtime_error("Window already created");

      while(!flip_display_mutex.try_lock_for(std::chrono::milliseconds(10))){
         al_rest(0.01);
      }
      al_set_new_display_flags(/*ALLEGRO_RESIZABLE |*/ flags);
      display = std::shared_ptr<ALLEGRO_DISPLAY>(al_create_display(width, height), /*al_destroy_display*/ dont_destroy<ALLEGRO_DISPLAY>);
      if (!display)
      {
         al_destroy_display(display.get());
         throw new std::runtime_error("Display failed to init");
         return -1;
      }

      display_bitmap = std::shared_ptr<ALLEGRO_BITMAP>(al_get_backbuffer(display.get()), dont_destroy<ALLEGRO_BITMAP>);

      timer = std::shared_ptr<ALLEGRO_TIMER>(al_create_timer(1.0 / FPS), dont_destroy<ALLEGRO_TIMER>);
      if (!timer)
      {
         al_destroy_timer(timer.get());
         al_destroy_display(display.get());
         throw new std::runtime_error("Timer failed to init");
         return -1;
      }

      event_queue = std::shared_ptr<ALLEGRO_EVENT_QUEUE>(al_create_event_queue(), dont_destroy<ALLEGRO_EVENT_QUEUE>);
      if (!event_queue)
      {
         al_destroy_event_queue(event_queue.get());
         al_destroy_timer(timer.get());
         al_destroy_display(display.get());
         throw new std::runtime_error("Event queue failed to init");
         return -1;
      }
      
      al_init_user_event_source(&user_generated);
      al_register_event_source(event_queue.get(), &user_generated);

      al_register_event_source(event_queue.get(), al_get_display_event_source(display.get()));
      al_register_event_source(event_queue.get(), al_get_timer_event_source(timer.get()));
      al_register_event_source(event_queue.get(), al_get_keyboard_event_source());
      al_register_event_source(event_queue.get(), al_get_mouse_event_source());

      al_clear_to_color(al_map_rgb(255, 255, 255));
      al_flip_display();
      
      m_FPS = FPS;
      
      window_created = true;
      
      _exec_window_created_function();
      
      flip_display_mutex.unlock();
      
      return 0;
   }
   
}
     