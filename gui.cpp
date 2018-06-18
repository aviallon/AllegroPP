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
			drawBtn();
		} else {
			state = 0;
		}
	}
}

void Button::click(int x, int y, uint16_t ev){
	if(isInside(x, y) && (ev & Allegro::MOUSE_UP) && state == 1){
		state = 2;
		drawBtn();
		btn_clicked(allegro_ptr, this);
	} else if (isInside(x, y)){
		state = 1;
		drawBtn();
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

uint16_t Button::id_increment = 1;
uint16_t InputBox::id_increment = 1;
