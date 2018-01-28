#include "gui.h"

Button::Button(Allegro* allegro, std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*)){
	this->name = name;
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->id = id_increment;
	this->btn_clicked = btn_clicked;
	this->allegro_ptr = allegro;
	id_increment++;
}

uint16_t Button::id_increment = 0;
