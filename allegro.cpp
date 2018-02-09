#include "allegro.h"
//#include "avlib.h"

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
    timer = NULL;
    event_queue = NULL;
	default_font = NULL;
	arial_file = NULL;
	ancien_emplacement = NULL;
	mouse_clicked_func_ptr = &Allegro::_undefined_;
	mouse_moved_func_ptr = &Allegro::_undefined_;
	redraw_func_ptr = &Allegro::_undefined_;
	animate_func_ptr = &Allegro::_undefined_;
	key_down_func_ptr = &Allegro::_undefined_;
	key_up_func_ptr = &Allegro::_undefined_;
	
	keys = std::vector<bool>(255, false);
	mouseBtns = std::vector<bool>(4, false);
	
	old_x = -1, old_y = -1;
	
	context = NULL;

    looping = true, redraw = false, redraw_paused = false;
}

Allegro::~Allegro()
{
	try{
		al_fclose(arial_file);
		
	} catch(...){
		std::cerr << "Erreur lors de la fermeture de arial_file;" << std::endl;
		std::flush(std::cerr);
	}
	
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
}

void Allegro::_exec_mouse_clicked_function(uint16_t ev){
	if(mouse.getBtn() == 1){
		mouse_clicked_func_ptr(this, context, MOUSE_L_CLICKED | ev, mouse.getX(), mouse.getY());
	} else if(mouse.getBtn() == 2)
		mouse_clicked_func_ptr(this, context, MOUSE_R_CLICKED | ev, mouse.getX(), mouse.getY());
}

void Allegro::_exec_mouse_moved_function(uint16_t ev){
	if(ev == MOUSE_MOVED)
		if(!cursorSticked)
			mouse_moved_func_ptr(this, context, MOUSE_MOVED, mouse.getX(), mouse.getY());
		else
			mouse_moved_func_ptr(this, context, MOUSE_MOVED_DELTA, mouse.getDX(), mouse.getDY());
	else if (ev == MOUSE_WHEELED)
		mouse_moved_func_ptr(this, context, MOUSE_WHEELED, mouse.getDZ(), 0);
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
	return mouse.getX();
}

int Allegro::getMouseY(){
	return mouse.getY();
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

void Allegro::quit(){
	ALLEGRO_EVENT ev;
	ev.type = ALLEGRO_EVENT_DISPLAY_CLOSE;
	al_emit_user_event(&user_generated, &ev, NULL);
}

// LIGNE
void Allegro::draw_line(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width){
	al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, color, (float)width);
}

void Allegro::draw_line(int x1, int y1, int x2, int y2){
	al_draw_line((float)x1, (float)y1, (float)x2, (float)y2, al_map_rgb(0,0,0), 1);
}

// ELLIPSE
void Allegro::draw_ellipse_r(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color, int width, bool filled = false){
	if(filled)
		al_draw_filled_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color);
	else
		al_draw_ellipse((float)cx, (float)cy, (float)rx, (float)ry, color, (float)width);
}

void Allegro::draw_ellipse(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled = false){
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
void Allegro::draw_rectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled = false){
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
	al_draw_text(font, color, x, y, align, c_text);
}

void Allegro::draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align){
	const char *c_text = text.c_str();
	al_draw_text(default_font, color, x, y, align, c_text);
}

void Allegro::draw_text(int x, int y, const char* text, ALLEGRO_COLOR color, int align){
	/* Draws text on the screen at the specifed coordinates. Add "\0" at the end of your string to avoid the appearance of weird characters. */
	al_draw_text(default_font, color, x, y, align, text);
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

Bitmap Allegro::getSubBitmapFromDisplay(int x, int y, int w, int h){
	std::cout << "WARNING : getSubBitmapFromDisplay not working !!!" << std::endl;
	
	int real_size = 0;
	Bitmap bmp(true);
	void* buffer;
	
	ALLEGRO_BITMAP* image = al_create_sub_bitmap(display_bitmap, x, y, w, h);
	int data_size = al_get_bitmap_width(image) * al_get_bitmap_height(image) * al_get_pixel_size(al_get_bitmap_format(image)) * 2;
	buffer = calloc(data_size, 1);
	if(!buffer)
		return Bitmap(false);
	
	//ALLEGRO_LOCKED_REGION* lock = al_lock_bitmap(display_bitmap, ALLEGRO_PIXEL_FORMAT_ABGR_8888, ALLEGRO_LOCK_READONLY);
	
	bmp.bitmap_memfile = al_open_memfile(buffer, data_size, "rw");
	if(!bmp.bitmap_memfile){
		std::cerr << "Failed to open memfile!" << std::endl;
	} else {
		al_save_bitmap_f(bmp.bitmap_memfile, ".bmp", image);
		real_size = al_ftell(bmp.bitmap_memfile);
		if(real_size < 1){
			std::cerr << "Real size too small!" << std::endl;
			return Bitmap(false);
		}
	}
	
	return bmp;
}

int Allegro::getTextWidth(std::string text, ALLEGRO_FONT* font){
	return al_get_text_width(font, text.c_str());
}

int Allegro::getTextWidth(const char* text){
	return al_get_text_width(default_font, text);
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

int Allegro::showDialogMessage(char const* title, char const* heading, char const* text, char const* buttons, int flags = ALLEGRO_MESSAGEBOX_YES_NO){
	return al_show_native_message_box(display, title, heading, text, buttons, flags);
}

const char* Allegro::askFile(char const* initial_path, char const* title, char const* patterns, int mode = 0){
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
		return al_save_bitmap(filename, bmp);
	}
}

int Allegro::init()
{
    if (!al_init())
    {
        return -1;
    }
	
	#ifdef __ENABLE_GUI__
	gui_ptr = (void*)(new GUI(this));
	#endif

	white = al_map_rgb(255, 255, 255);
	black = al_map_rgb(0, 0, 0);

    return 0;
}

void Allegro::flipDisplay(){
	al_flip_display();
}

int Allegro::getDisplayWidth(){
	return al_get_display_width(display);
}

int Allegro::getDisplayHeight(){
	return al_get_display_height(display);
}

void Allegro::toggleFullscreen(bool activate){
	al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, activate);
}

bool Allegro::isInFullscreen(){
	return (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW);
}

int Allegro::createWindow(float FPS, int width, int height)
{
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_WINDOWED);
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

    al_install_keyboard();
	al_install_mouse();
    al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	
	
	//default_font = al_load_ttf_font("arial.ttf", 25, 0);
	

#if (defined (LINUX) || defined (__linux__))
	/* load font from memory */
	extern uint8_t arial_data[]	asm("_binary_allegro_fonts_Arimo_Regular_ttf__start");
	extern uint8_t arial_data_size[]	asm("_binary_allegro_fonts_Arimo_Regular_ttf_size");

	size_t arial_size = (size_t)((void *)arial_data_size);

	arial_file = al_open_memfile(arial_data, (int64_t)arial_size, "r");
	default_font = al_load_ttf_font_f(arial_file, 0, 12, 0);

	/* yep, the font file is embeded in the executable at link time :D */
#endif
#if (defined (_WIN32) || defined (_WIN64))
	/*uint8_t* arial_data;
	uint8_t* arial_data_size;
	__asm mov _binary_allegro_arial_ttf_start, [ebx]arial_data
	__asm mov _binary_allegro_arial_ttf_size, [ebx]arial_data_size*/

	default_font = al_load_ttf_font("fonts/Arimo-Regular.ttf", 25, 0);

#endif
	
	
	if(!default_font){
		std::cerr << "Warning ! Could not load default font !" << std::endl;
		std::flush(std::cerr);
		throw new std::logic_error("Could not load default font !");
	} else {
		//std::cout << "Succesfuly loaded arial.ttf" << std::endl;
	}

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

void Allegro::gameLoop()
{
    al_start_timer(timer);
    while (looping)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
		
		if(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE){
			al_acknowledge_resize(display);
			//WIDTH = getDisplayWidth();
			//HEIGHT = getDisplayHeight();
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			mouseBtns[ev.mouse.button] = true;
			mouse.setBtn(ev.mouse.button);
			
			#ifdef __ENABLE_GUI__
			int btnEv = 0;
			if(mouse.getBtn() == 1)
				btnEv = MOUSE_L_CLICKED;
			else
				btnEv = MOUSE_R_CLICKED;
			getGUI()->checkBtnClick(btnEv | MOUSE_DOWN, mouse.getX(), mouse.getY());
			#endif
			
			_exec_mouse_clicked_function(MOUSE_DOWN);
		}else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouseBtns[ev.mouse.button] = false;
			mouse.setBtn(ev.mouse.button);
			
			#ifdef __ENABLE_GUI__
			int btnEv = 0;
			if(mouse.getBtn() == 1)
				btnEv = MOUSE_L_CLICKED;
			else
				btnEv = MOUSE_R_CLICKED;
			getGUI()->checkBtnClick(btnEv | MOUSE_UP, mouse.getX(), mouse.getY());
			#endif
			
			_exec_mouse_clicked_function(MOUSE_UP);
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) { 
			al_get_mouse_state(mouse.getStatePtr());
			mouse.setDZ(ev.mouse.dz);
			mouse.setDX(ev.mouse.dx);
			mouse.setDY(ev.mouse.dy);
			if(ev.mouse.dz != 0){
				_exec_mouse_moved_function(MOUSE_WHEELED);
			}
			_exec_mouse_moved_function(MOUSE_MOVED);
			
			#ifdef __ENABLE_GUI__
			getGUI()->drawCursor(mouse.getX(), mouse.getY());
			
			getGUI()->btnHovering(MOUSE_MOVED, mouse.getX(), mouse.getY());
			#endif
			
			if(cursorSticked)
				al_set_mouse_xy(display, al_get_display_height(display)/2, al_get_display_width(display)/2);
			
		} else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			_exec_key_down_function(ev.keyboard.keycode);
		} else if(ev.type == ALLEGRO_EVENT_KEY_UP){
			_exec_key_up_function(ev.keyboard.keycode);
		}
		
		
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
			_exec_key_repeat_function();
			
			animate_func_ptr(this, m_FPS);
			
			if(!redraw_paused)
				redraw = true;
			else
				al_flip_display();
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            looping = false;
			exit(0);
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
			
			redraw_func_ptr(this, m_FPS);
			
			#ifdef __ENABLE_GUI__
			
			getGUI()->drawAllBtns();
			
			getGUI()->drawLastMessage();
			
			/*ancien_emplacement = al_create_sub_bitmap(display_bitmap, mouse.getX(), mouse.getY(), mouse.getX()+32, mouse.getY()+32);
			if(ancien_emplacement != 0 && old_x != -1){
				al_draw_scaled_bitmap(ancien_emplacement, 0, 0, al_get_bitmap_width(ancien_emplacement), al_get_bitmap_height(ancien_emplacement), old_x, old_y, 30, 30, 0);
			}
			//ancien_emplacement = al_create_sub_bitmap(display_bitmap, mouse.getX(), mouse.getY(), mouse.getX()+32, mouse.getY()+32);*/
			getGUI()->drawCursor(mouse.getX(), mouse.getY());
			//old_x = mouse.getX();
			//old_y = mouse.getY();
			#endif

            al_flip_display();
        }
    }
}
