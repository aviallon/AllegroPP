#include <allegro++/allegro_class.h>

namespace AllegroPP {
   
   void Allegro::_undefined_(Allegro* master, void* context, uint16_t event, int x, int y){
      // Do nothing
   }

   void Allegro::_undefined_(Allegro* master, float FPS){
      
   }

   void Allegro::_undefined_(Allegro* master, void* context, uint16_t event, uint8_t keycode){
      
   }
   
   void Allegro::_undefined_(Allegro* master, void* context){
      
   }
   
   Allegro::Allegro(bool start_on_copy)
   {
//      display = nullptr;
//      display_bitmap = nullptr;
//      timer = nullptr;
//      event_queue = nullptr;
      //default_font = NULL;
      //arial_file = NULL;
//      ancien_emplacement = nullptr;
      mouse_clicked_func_ptr = &Allegro::_undefined_;
      mouse_moved_func_ptr = &Allegro::_undefined_;
      redraw_func_ptr = &Allegro::_undefined_;
      animate_func_ptr = &Allegro::_undefined_;
      key_down_func_ptr = &Allegro::_undefined_;
      key_up_func_ptr = &Allegro::_undefined_;
      window_resized_func_ptr = &Allegro::_undefined_;
      window_closed_func_ptr = &Allegro::_undefined_;
      window_created_func_ptr = &Allegro::_undefined_;
      
      keys = std::vector<bool>(255, false);
      mouseBtns = std::vector<bool>(4, false);
      
      mouse = std::shared_ptr<Mouse>(new Mouse());
      
      //old_x = -1, old_y = -1;
      
      context = nullptr;

      redraw = false, redraw_paused = false;
      
      frame_skipped = 0;
      
      effective_frametime = 0;
      
      last_frame = clock::now();
      
      if(!start_on_copy)
         _start_loop();
      
      focus = true;
      
      #ifdef __ENABLE_GUI__
      gui_ptr = std::shared_ptr<GUI>(new GUI(this));
      #endif
      
      instances.push_back(this);
   }
   
   Allegro::Allegro(const Allegro& allegro){
      
      display = allegro.display;
      display_bitmap = allegro.display_bitmap;
      timer = allegro.timer;
      event_queue = allegro.event_queue;
      ancien_emplacement = allegro.ancien_emplacement;
      
      mouse_clicked_func_ptr = allegro.mouse_clicked_func_ptr,
      mouse_moved_func_ptr = allegro.mouse_moved_func_ptr;
      redraw_func_ptr = allegro.redraw_func_ptr;
      animate_func_ptr = allegro.animate_func_ptr;
      key_down_func_ptr = allegro.key_down_func_ptr;
      key_up_func_ptr = allegro.key_up_func_ptr;
      window_resized_func_ptr = allegro.window_resized_func_ptr;
      window_closed_func_ptr = allegro.window_closed_func_ptr;
      window_created_func_ptr = allegro.window_created_func_ptr;
      
      keys = allegro.keys;
      mouseBtns = allegro.mouseBtns;
      
      mouse = allegro.mouse;
      
      context = allegro.context;
      
      redraw = allegro.redraw;
      redraw_paused = allegro.redraw_paused;
      
      frame_skipped = allegro.frame_skipped;
      effective_frametime = allegro.effective_frametime;
      
      last_frame = allegro.last_frame;
      
      focus = allegro.focus;
      
      gui_ptr = allegro.gui_ptr;
      
      window_created = false;
      
      _start_loop();
   }

   Allegro::~Allegro()
   {
      event_loop_working = true;
      looping = false;
      while(looping || !flip_display_mutex.try_lock_for(std::chrono::milliseconds(10))){
         al_rest(0.02);
         //flip_display_mutex.unlock();
      }
//      delete mouse;
//      delete (GUI*)gui_ptr;
    //  if(loops == 0){
   //		try{
   //			al_fclose(arial_file);
   //			
   //		} catch(...){
   //			std::cerr << "Erreur lors de la fermeture de arial_file;" << std::endl;
   //			std::flush(std::cerr);
   //		}
         
         //exit(0);
    //  }
   }
   
   int Allegro::init()
   {
       if (!al_init())
       {
           return -1;
       }
      
      al_install_keyboard();
      al_install_mouse();
      al_init_image_addon();
      al_init_primitives_addon();
      al_init_font_addon();
      al_init_ttf_addon();
      
   // Disabled memory font because it caused too much bugs.
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

      default_font = al_load_ttf_font("@InstallDir@/fonts/Arimo-Regular.ttf", 12, 0);

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
   
}