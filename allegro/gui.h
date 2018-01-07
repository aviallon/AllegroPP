#pragma once
#include <string>
#include "allegro.h"


class Button {
private:
	static uint16_t id_increment;
public:
	Button(Allegro* allegro, std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*));
	int x;
	int y;
	int height;
	int width;
	short state = 0;
	Allegro* allegro_ptr;
	uint16_t id;
	long data;
	std::string name;
	void (*btn_clicked)(Allegro*, Button*);
	
	bool isInside(int x, int y){
		if(this->x < x && this->x+width > x && this->y < y && this->y+height > y){
			return true;
		}
		return false;
	}
	
	void click(int x, int y, uint16_t ev){
		if(isInside(x, y) && (ev & Allegro::MOUSE_UP) && state == 1){
			state = 2;
			drawBtn();
			btn_clicked(allegro_ptr, this);
		} else if (isInside(x, y)){
			state = 1;
			drawBtn();
		}
	}
	
	void hover(int x, int y){
		if(!allegro_ptr->isMouseBtnDown(1)){
			if(isInside(x, y)){
				state = 2;
				drawBtn();
			} else {
				state = 0;
			}
		}
	}
	
	void drawBtn(){
		ALLEGRO_COLOR color;
		if(state == 1)
			color = allegro_ptr->rgb(100, 100, 200);
		else if(state == 0)
			color = allegro_ptr->rgb(100, 100, 100);
		else if(state == 2){
			color = allegro_ptr->rgb(100, 100, 150);
		}
		allegro_ptr->draw_rectangle(x, y, x+width, y+height, color, 1, true);
		allegro_ptr->draw_text(x+width/2, y+height/2, name, allegro_ptr->rgb(0, 0, 0));
	}
};


class GUI{
public:
	GUI() {

	}
	
	Button* newBtn(Allegro* allegro, std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*)){
		buttons.push_back(Button(allegro, name, x, y, height, width, btn_clicked));
		return &buttons[buttons.size()-1];
	}

	void drawBtn(int i) {
		buttons[i].drawBtn();
	}
	
	void drawAllBtns(){
		for(unsigned int i=0; i<buttons.size(); i++){
			drawBtn(i);
		}
	}
	
	void checkBtnClick(Allegro* allegro, uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_L_CLICKED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].click(x, y, ev);
			}
		}
	}
	
	void btnHovering(Allegro* allegro, uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_MOVED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].hover(x, y);
			}
		}
	}

	std::vector<Button> buttons;
};
