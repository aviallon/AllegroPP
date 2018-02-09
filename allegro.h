#pragma once
#ifndef ALLEGRO_H_
#define ALLEGRO_H_

#include <iostream>
#include <string>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/file.h>
#include <cmath>
#include <vector>
#include <algorithm>

#define MAX(a, b) (((a > b))?(a):(b))
#define MIN(a, b) (((a < b))?(a):(b))

#include <exception>
#include <memory>

#define __ENABLE_GUI__

#include "mouse.h"
//#include "gui.h"

class Mouse;
class Button;
class GUI;
class Image;
class Bitmap;

typedef unsigned char uchar;

class Allegro
{
private:
    ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP *display_bitmap;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_FONT *default_font;
	ALLEGRO_FILE *arial_file;
	
	ALLEGRO_BITMAP *ancien_emplacement;
	
	void (*mouse_clicked_func_ptr)(Allegro*, void*, uint16_t, int, int);
	void (*mouse_moved_func_ptr)(Allegro*, void*, uint16_t, int, int);
	
	void (*key_down_func_ptr)(Allegro*, void*, uint16_t, uint8_t);
	void (*key_up_func_ptr)(Allegro*, void*, uint16_t, uint8_t);
	
	void (*redraw_func_ptr)(Allegro*, float);
	void (*animate_func_ptr)(Allegro*, float);
	
	void* context;
	
	Mouse mouse;
	int old_x;
	int old_y;
	
	std::vector<bool> keys;
	std::vector<bool> mouseBtns;

    bool looping, redraw, redraw_paused;
	
	void _exec_mouse_clicked_function(uint16_t ev);
	void _exec_mouse_moved_function(uint16_t ev);
	
	void _exec_key_down_function(uint8_t keycode);
	void _exec_key_up_function(uint8_t keycode);
	void _exec_key_repeat_function();
	
	ALLEGRO_EVENT_SOURCE user_generated;
	
	
	float m_FPS;
	bool cursorSticked = false;

public:
    Allegro();
    ~Allegro();

    int init();
    int createWindow(float FPS, int w, int h);
    void gameLoop();
	
	void bindMouseClick(void (*fptr)(Allegro*, void*, uint16_t, int, int));
	void bindMouseMove(void (*fptr)(Allegro*, void*, uint16_t, int, int));
	
	void bindKeyDown(void (*fptr)(Allegro*, void*, uint16_t, uint8_t));
	void bindKeyUp(void (*fptr)(Allegro*, void*, uint16_t, uint8_t));
	
	void setRedrawFunction(void (*fptr)(Allegro*, float));
	void setAnimateFunction(void (*fptr)(Allegro*, float));
	
	static const uint16_t MOUSE_R_CLICKED = 1<<0;
	static const uint16_t MOUSE_L_CLICKED = 1<<7;
	static const uint16_t MOUSE_DOWN = 1<<8;
	static const uint16_t MOUSE_UP = 1<<9;
	static const uint16_t MOUSE_MOVED = 1<<1;
	static const uint16_t KEY_DOWN = 1<<2;
	static const uint16_t KEY_UP = 1<<3;
	static const uint16_t KEY_REPEAT = 1<<4;
	static const uint16_t MOUSE_WHEELED = 1<<5;
	static const uint16_t MOUSE_MOVED_DELTA = 1<<6;
	
	ALLEGRO_COLOR white;
	ALLEGRO_COLOR black;
	
	
	struct ALLEGRO_COLOR rgb(int r, int g, int b);
	struct ALLEGRO_COLOR rgba(int r, int g, int b, int a);
	
	/* Les fonctions pour dessiner ! Enfin ! */
	
	void set_pixel(int x, int y, ALLEGRO_COLOR color);
	
	void draw_line(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width);
	void draw_line(int x1, int y1, int x2, int y2);
	
	void draw_ellipse(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled);
	void draw_ellipse(int x1, int y1, int x2, int y2);
	
	void draw_ellipse_r(int cx, int cy, int rx, int ry, ALLEGRO_COLOR color, int width, bool filled);
	
	void draw_rectangle(int x1, int y1, int x2, int y2, ALLEGRO_COLOR color, int width, bool filled);
	void draw_rectangle(int x1, int y1, int x2, int y2);
	
	void draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align, ALLEGRO_FONT* font);
	void draw_text(int x, int y, std::string text, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_CENTER);
	void draw_text(int x, int y, const char* text, ALLEGRO_COLOR color, int align = ALLEGRO_ALIGN_CENTER);
	
	void draw_image(int x, int y, ALLEGRO_BITMAP* image);
	void draw_scaled_image(int x, int y, int w, int h, ALLEGRO_BITMAP* image);
	
	Bitmap getSubBitmapFromDisplay(int x, int y, int w, int h);
	
	int getTextWidth(std::string text, ALLEGRO_FONT* font);
	int getTextWidth(const char* text);
	
	ALLEGRO_FONT* getDefaultFont(){
		return default_font;
	}
	
	int showDialogMessage(char const *title, char const *heading, char const *text, char const *buttons, int flags);
	const char* askFile(char const* initial_path, char const* title, char const* patterns, int mode);
	
	// Locks the buffer. Screen is only updated after it as been unlocked
	void lockScreen();
	void unlockScreen();
	
	void setCursorVisibility(bool visible);
	void setStickCursorToCenter(bool stick);
	
	void stopRedraw();
	void resumeRedraw();
	
	void flipDisplay();
	
	long int getTime();
	
	int getMouseX();
	int getMouseY();
	
	bool isKeyDown(int keycode);
	bool isMouseBtnDown(int button);
	
	void setContext(void* cont);
	void* getContext();
	
	int getDisplayWidth();
	int getDisplayHeight();
	
	void toggleFullscreen(bool activate);
	bool isInFullscreen();
	
	bool screenshot(const char* filename, int x = 0, int y = 0, int x2 = -1, int y2 = -1);
	
	void quit();
	
	
	static void _undefined_(Allegro* master, void* context, uint16_t event, int x, int y);
	static void _undefined_(Allegro* master, void* context, uint16_t event, uint8_t keycode);
	static void _undefined_(Allegro* master, float FPS);
	
	// This is a static RGB function.
	static struct ALLEGRO_COLOR rgbS(int r, int g, int b){
		return al_map_rgb(r, g, b);
	}
	
	static std::string colorToStr(ALLEGRO_COLOR color){
		std::stringstream ss;
		ss << color.r*255 << "," << color.g*255 << "," << color.b*255 << "," << color.a*255;
		return ss.str();
	}
	
	static ALLEGRO_COLOR strToColor(std::string str){
		std::replace(str.begin(), str.end(), ',', ' ');
		std::stringstream ss;
		//std::cout << str << std::endl;
		ss << str;
		int r, g, b, a;
		ss >> r >> g >> b >> a;
		return al_map_rgb(r, g, b);
	}
	
	void* gui_ptr;
	
	GUI* getGUI(){
		return (GUI*)gui_ptr;
	}
};

#include "gui.h"

#endif