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

class Image{
public:
	Image(){
		
	}
	
	Image(const char* filename, int x = 0, int y = 0, int width = 0, int height = 0){
		image = al_load_bitmap(filename);
		if(image != 0 && (width == 0 || height == 0)){
			this->width =  al_get_bitmap_width(image);
			this->height = al_get_bitmap_height(image);
		}else{
			this->width = width;
			this->height = height;
		}
		this->x = x;
		this->y = y;
	}
	
	void drawImage(Allegro* allegro, int x = -1, int y = -1){
		//std::cout << "I've been called" << std::endl;
		if(x == -1 && y == -1){
			x = this->x;
			y = this->y;
		}
		if(image != 0){
			allegro->draw_image(x, y, image);
		}
	}
	
	void drawScaledImage(Allegro* allegro, int x, int y, int w, int h){
		if(image != 0)
			allegro->draw_scaled_image(x, y, w, h, image);
	}
	
	int x, y;
	int width;
	int height;
	ALLEGRO_BITMAP* image;
};

class Bitmap{
public:
	Bitmap(bool ok = false){
		isOk = ok;
	}
	
	ALLEGRO_BITMAP* getBitmap(){
		if(isOk){
			return al_load_bitmap_f(bitmap_memfile, ".bmp");
		} else {
			std::cerr << "Bitmap not correct" << std::endl;
			throw(5);
		}
	}
	
	Image getImage(){
		if(isOk){
			ALLEGRO_BITMAP* bmp = al_load_bitmap_f(bitmap_memfile, ".bmp");
			Image img;
			img.image = bmp;
			img.width = al_get_bitmap_width(bmp);
			img.height = al_get_bitmap_height(bmp);
			return img;
		}
	}
	
	bool isOk;
	ALLEGRO_FILE* bitmap_memfile;
};

class Cursor{
public:
	
	Cursor(Allegro* allegro, const char* filename, std::string name){
		cursor = new Image(filename);
		this->name = name;
	}
	
	void eraseOldCursor(){
		allegro->draw_rectangle(cursor->x, cursor->y, cursor->x+cursor->width, cursor->y+cursor->height, allegro->rgb(255, 255, 255), 1, true);
	}
	
	void drawCursor(int x, int y){
		allegro->draw_rectangle(cursor->x, cursor->y, cursor->x+cursor->width, cursor->y+cursor->height, allegro->rgb(255, 255, 255), 1, true);
		//if(old_place.isOk){
		//	std::cout << "Test" << std::endl;
		//	old_place.getImage().drawImage(allegro, cursor->x, cursor->y);
		//}
		/*if(ancien_emplacement.image != 0){
			//ancien_emplacement.drawImage(allegro, this->x, this->y);
		}*/
		cursor->x = x;
		cursor->y = y;
		//old_place = allegro->getSubBitmapFromDisplay(x, y, cursor->width, cursor->height);
		//ancien_emplacement = allegro->getSubBitmapFromDisplay(x, y, cursor->width, cursor->height);
		
		cursor->drawImage(allegro, x, y);
	}
	
	std::string name;
	Allegro* allegro;
	int x, y;
	Bitmap old_place;
	Image* cursor;
};

class Message{
public:
	
	Message(std::string message, float duration, float currentTime){
		this->message = message;
		this->duration = duration;
		this->t0 = currentTime;
	}
	
	std::string message;
	float duration;
	float t0 = 0;
};

class GUI{
public:
	GUI(Allegro* allegro) {
		this->allegro = allegro;
	}
	
	Button* newBtn(std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*)){
		buttons.push_back(Button(allegro, name, x, y, height, width, btn_clicked));
		return &buttons[buttons.size()-1];
	}
	
	Image* newImage(const char* filename, int x, int y, int height = 0, int width = 0){
		images.push_back(Image(filename, x, y, width, height));
		return &images[images.size()-1];
	}
	
	int newCursor(const char* filename, std::string name){
		cursors.push_back(Cursor(allegro, filename, name));
		return cursors.size()-1;
	}
	
	void setCursor(int i){
		if(cursor >= 0)
			cursors[cursor].eraseOldCursor();
		cursor = i;
	}
	
	void drawCursor(int x, int y){
		if(cursor >= 0)
			cursors[cursor].drawCursor(x, y);
	}

	void drawBtn(int i) {
		buttons[i].drawBtn();
	}
	
	void drawAllBtns(){
		for(unsigned int i=0; i<buttons.size(); i++){
			drawBtn(i);
		}
	}
	
	void drawImage(int i){
		//images[i].drawImage(allegro, images[i].x, images[i].y);
		//std::cout << images[i].x << ", " << images[i].y << std::endl;
		images[i].drawScaledImage(allegro, images[i].x, images[i].y, images[i].width, images[i].height);
	}
	
	void drawAllImages(){ // not active by default
		for(unsigned i = 0; i<images.size(); i++){
			drawImage(i);
		}
	}
	
	void checkBtnClick(uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_L_CLICKED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].click(x, y, ev);
			}
		}
	}
	
	void btnHovering(uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_MOVED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].hover(x, y);
			}
		}
	}
	
	void drawLastMessage(){
		if (messages.size() == 0)
			return;
		
		Message lastMessage = messages[messages.size() - 1];
		
		float duree = MAX((allegro->getTime() - lastMessage.t0), 0);
		if(duree >= lastMessage.duration){
			messages.pop_back();
			return;
		}
	
		int alpha = 0;
		if(lastMessage.duration - duree <= 1000){
			alpha = -255*MAX((lastMessage.duration - duree), 0)/1000;
		}
		
		allegro->draw_text(allegro->getDisplayWidth()/2, allegro->getDisplayHeight()-20, lastMessage.message, allegro->rgba(255-alpha, 255-alpha, 255-alpha, 0));
		if(duree >= lastMessage.duration){
			messages.pop_back();
		}
	}
	
	void displayMessage(std::string message, float duration){
		messages.push_back(Message(message, duration, allegro->getTime()));
	}
	
	Allegro* allegro;

	int cursor = -1;
	std::vector<Cursor> cursors;
	std::vector<Image> images;
	std::vector<Button> buttons;
	std::vector<Message> messages;
};
