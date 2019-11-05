#include <allegro++/allegro_class.h>

namespace AllegroPP {
   
   void Allegro::_start_loop(){
   //	al_start_timer(timer);
      while(!flip_display_mutex.try_lock_for(std::chrono::milliseconds(15))){
         rest(0.01);
      }
      looping = true;
      event_loop_working = true;
      loops++;
      ALLEGRO_THREAD* thr = al_create_thread(Allegro::_loop, (void*)this);
      allegro_threads.push(
         std::shared_ptr<ALLEGRO_THREAD>(
            thr,
            al_destroy_thread
         )
      );
      al_start_thread(allegro_threads.top().get());
      //al_start_thread(allegro_threads[allegro_threads.size() - 1].get());
      //allegro_threads.push_back(std::thread(_loop, this));
      
      thr = al_create_thread(Allegro::_event_loop, (void*)this);
      event_threads.push(
         std::shared_ptr<ALLEGRO_THREAD>(
            thr,
            al_destroy_thread
         )
      );
      al_start_thread(event_threads.top().get());
      //al_start_thread(event_threads[event_threads.size() - 1].get());
      
      //event_threads.push_back(std::thread(_event_loop, this));
      //allegro_threads[allegro_threads.size()-1].detach();
      thread_id = allegro_threads.size()-1;
      
      event_loop_working = false;
      flip_display_mutex.unlock();
   }

   void Allegro::_stop_loop(){
      looping = false;
      loops--;
      _quit();
   }
   
   void Allegro::_event_loop_element(){
      if(!display){
         printf("No display...\n");
         return;
      }
      
      if(!looping || !window_created)
         return;
         
      currently_drawing_or_listening_for_events++;
         
      if(!al_get_timer_started(timer.get()))
            al_start_timer(timer.get());
      
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue.get(), &ev);
      //al_wait_for_event_timed(event_queue, &ev, 0.2);
      
      if(ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN){
         focus = true;
      } else if(ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT){
         focus = false;
         
         for(unsigned i = 0; i<getGUI()->buttons.size(); i++){
            getGUI()->getBtn(i)->setState(0);
         }
      }
      
      if(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE){
         al_acknowledge_resize(display.get());
         _exec_window_resized_function();
      }
      if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) { 
         mouse->updateMouseState();
         mouse->setDZ(ev.mouse.dz);
         mouse->setDX(ev.mouse.dx);
         mouse->setDY(ev.mouse.dy);
         
         if(mouse->hasMouseFocusOnDisplay(display.get())){
         
            if(ev.mouse.dz != 0){
               _exec_mouse_moved_function(MOUSE_WHEELED);
            }
            _exec_mouse_moved_function(MOUSE_MOVED);
            
            #ifdef __ENABLE_GUI__
            
            getGUI()->mouseHoveringHandle(MOUSE_MOVED, mouse->getX(), mouse->getY());
            #endif
         
         }
         
         if(cursorSticked)
            al_set_mouse_xy(display.get(), al_get_display_height(display.get())/2, al_get_display_width(display.get())/2);
         
      }
      
      if(ev.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY){
         if(mouse->hasMouseFocusOnDisplay(display.get())){
            // Loose focus on all buttons inside a window when mouse leaves it
            for(unsigned i = 0; i<getGUI()->buttons.size(); i++){
               getGUI()->getBtn(i)->setState(0);
            }
         }
      }
      
      if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
         mouse->updateMouseState();
         
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
            
            //flip_display_mutex.try_lock();
            _exec_mouse_clicked_function(MOUSE_DOWN);
            //flip_display_mutex.unlock();
            
         }
      }else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
         mouse->updateMouseState();
         
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
            
            //flip_display_mutex.try_lock();
            _exec_mouse_clicked_function(MOUSE_UP);
            //flip_display_mutex.unlock();
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
         }
      }
      
      if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      {
         //flip_display_mutex.try_lock();
         _exec_window_closed_function();
         //flip_display_mutex.unlock();
         _stop_loop();
         
         currently_drawing_or_listening_for_events--;
         
         return;
      }
      
      if(ev.type == ALLEGRO_EVENT_TIMER){
         timer_fired = true;
         
         if(!display || looping == false){
            currently_drawing_or_listening_for_events--;
            return;
         }
         _exec_key_repeat_function();
      }
      
      currently_drawing_or_listening_for_events--;
      
   }

   void Allegro::_loop_element(){
      if(!display){
         printf("No display...\n");
         return;
      }
      
      if(!looping || !window_created)
         return;
         
      currently_drawing_or_listening_for_events++;
      
      al_set_target_backbuffer(display.get());
      
      if(timer_fired)
      {
         timer_fired = false;
         
         animate_func_ptr(this, m_FPS);        
         
         
         if(!redraw_paused){
            redraw = true;
         }else{
            flipDisplay();
         }
         
         //std::cout << "Redraw paused ? " << redraw_paused << ", redraw : " << redraw << std::endl;
      }

      if (redraw)
      {
         redraw = false;
         
         //printf("We are redrawing #1\n");
         
         std::shared_ptr<ALLEGRO_BITMAP> display_bmp_clone;
         if(looping && display_bitmap && flip_display_mutex.try_lock_for(std::chrono::milliseconds(20))) {
            display_bmp_clone = std::shared_ptr<ALLEGRO_BITMAP>(al_clone_bitmap(display_bitmap.get()), al_destroy_bitmap);
            flip_display_mutex.unlock();
         } else {
            currently_drawing_or_listening_for_events--;
            return;
         }
         al_set_target_bitmap(display_bmp_clone.get());
         
         //printf("We are redrawing #2\n");
         
         redraw_func_ptr(this, m_FPS);
         
         #ifdef __ENABLE_GUI__
         
         getGUI()->drawAllBtns();
         
         getGUI()->drawAllInputBoxes();
         
         getGUI()->drawLastMessage();
         
         getGUI()->drawCursor(mouse->getX(), mouse->getY());
         
         #endif
         
         if(flip_display_mutex.try_lock()){
            if(looping && display_bitmap) {
               al_set_target_bitmap(display_bitmap.get());
               al_draw_bitmap(display_bmp_clone.get(), 0, 0, 0);
               al_flip_display();
            }
            flip_display_mutex.unlock();
         }
         //al_destroy_bitmap(display_bmp_clone);
      }
      currently_drawing_or_listening_for_events--;
   }
   
   void* Allegro::_loop(ALLEGRO_THREAD *thr, void* ctx){
      
      if(ctx == nullptr)
         throw new std::runtime_error("Allegro ptr is NULL !");
      
      Allegro* allegro = (Allegro*)ctx;
      //shared_ptr<Allegro> allegro((Allegro*)ctx);
      
//      if(!allegro || true)
//         throw new std::runtime_error("Not good...");
      
      while(!loop_started || !allegro->window_created){
         //std::cout << "Window not created or loop not started : " << allegro->getThreadId() << std::endl;
         rest(0.001);
         //std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      
      while(allegro->looping){
         if(!allegro->event_loop_working){
            allegro->_loop_element();
            duration dt = clock::now() - allegro->last_frame;
            allegro->effective_frametime = dt.count();
            allegro->last_frame = clock::now();
         }
         //std::cout << "Thread ID : " << allegro->getThreadId() << std::endl;
         rest(0.0001);
         //std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
      
      return ctx;
   }
   
   void* Allegro::_event_loop(ALLEGRO_THREAD* thr, void* ctx){
      
      if(ctx == nullptr)
         throw new std::runtime_error("Allegro ptr is NULL !");
      
      //shared_ptr<Allegro> allegro((Allegro*)ctx);
      Allegro* allegro = (Allegro*)ctx;
      
//      if(!allegro || true)
//         throw new std::runtime_error("Not good...");
      
      while(!loop_started || !allegro->window_created){
         rest(0.001);
         //std::this_thread::sleep_for(std::chrono::microseconds(500));
      }
      
      while(allegro->looping){
         if(!allegro->event_loop_working){
            allegro->event_loop_working = true;
            allegro->_event_loop_element();
            allegro->event_loop_working = false;
         }
         rest(0.0001);
         //std::this_thread::sleep_for(std::chrono::microseconds(100));
      }
      
      return ctx;
   }


   void Allegro::startLoop()
   {
      loop_started = true;
      
      for(Allegro* allegro : instances){
         allegro->_start_loop();
      }
      
      unsigned instance_num = instances.size();
      while(loops > 0){
         rest(0.001);
         if(instance_num != instances.size()){
            std::cout << "Allegro instance was created/destroyed : +(" << instances.size()-instance_num << ")" << std::endl;
            instance_num = instances.size();
         }
      }
      
      loop_started = false;
      
//      for(std::thread& event_thd : event_threads){
//         event_thd.join();
//      }
//      for(Allegro* allegro : instances){
//         //al_pause_event_queue(allegro->event_queue.get(), false);
//         //allegro->_stop_loop();
//      }
      
//      while(currently_drawing_or_listening_for_events > 0){
//         std::cout << "Waiting for redrawing functions to stop... (" << currently_drawing_or_listening_for_events << ") left." << std::endl;
//         rest(0.01);
//      }
      
      // Needed to avoid race conditions...
      std::this_thread::sleep_for(std::chrono::seconds(1));
      
      for(Allegro* allegro : instances){
         allegro->_stop_loop();
         delete allegro;
      }
      
//      std::shared_ptr<ALLEGRO_THREAD> thd;
//      while(thd = event_threads.top()){
//         al_set_thread_should_stop(thd.get());
//         event_threads.pop();
//      }
//      
//      while(thd = allegro_threads.top()){
//         al_set_thread_should_stop(thd.get());
//         allegro_threads.pop();
//      }

//      for(std::shared_ptr<ALLEGRO_THREAD> event_thd : event_threads){
//         al_set_thread_should_stop(event_thd.get());
//      }
////      for(std::thread& allegro_thd : allegro_threads){
////         allegro_thd.join();
////      }
//      for(std::shared_ptr<ALLEGRO_THREAD> allegro_thd : allegro_threads){
//         al_set_thread_should_stop(allegro_thd.get());
//      }
      
   }
   
}