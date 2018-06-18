#pragma once
#include <string>
#include <chrono>
#include "allegro.h"
#include "sprites.h"

inline int between(double x, int min, int max){
	return floor((x>max) ? max : ((x<min) ? min : x));
}

inline float between(double x, float min, float max){
	return (x>max) ? max : ((x<min) ? min : x);
}

class Color {
public:
	Color(int r, int g, int b, float a = 1){
		_r = between(r, 0, 255);
		_g = between(g, 0, 255);
		_b = between(b, 0, 255);
		_a = between(a, 0, 255);
	}
	
	Color(){
		Color(0, 0, 0);
	}
	
	Color(bool notColor){
		Color(0, 0, 0);
		this->notColor = notColor;
	}
	
	Color operator*(double k){
		return Color(this->_r*k, this->_g*k, this->_b*k);
	}
	
	Color operator/(double k){
		return Color(this->_r/k, this->_g/k, this->_b/k);
	}
	
	Color operator+(const Color& c){
		return Color(this->_r+c._r, this->_g+c._g, this->_b+c._b);
	}
	
	Color mix(const Color& c){
		return Color(between(this->_r, 0, c._r), between(this->_g, 0, c._g), between(this->_b, 0, c._b));
	}
	
	Color blend(const Color& c){
		return Color((this->_r + c._r)/2, (this->_g + c._g)/2, (this->_b + c._b)/2);
	}
	
	ALLEGRO_COLOR toAllegro(){
		if(_a != 1)
			return al_map_rgba(_r, _g, _b, _a);
		else
			return al_map_rgb(_r, _g, _b);
	}
	
	int _r, _g, _b;
	float _a;
	bool notColor = false;
};

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
	
	//bool use_sprites = false;
	Sprite btn_sprite;
	Sprite btn_sprite_clicked;
	Sprite btn_sprite_hovered;
	
	void (*btn_clicked)(Allegro*, Button*);
	
	void setSprites(Sprite defaut, Sprite clicked, Sprite hovered);
	
	bool isInside(int x, int y);
	
	void click(int x, int y, uint16_t ev);
	
	void hover(int x, int y);
	
	void drawBtn();
};

class Image{
public:
	Image(){
		
	}
	
	Image(const char* filename, int x = 0, int y = 0, int width = 0, int height = 0){
		sprite = SpriteMap(filename).getWholeSprite();
		if(sprite && (width == 0 || height == 0)){
			this->width = sprite.getWidth();
			this->height = sprite.getHeight();
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
		if(sprite){
			sprite.drawSprite(x, y);
		}
	}
	
	void drawScaledImage(Allegro* allegro, int x, int y, int w, int h){
		if(sprite)
			sprite.drawSprite(x, y, w, h);
	}
	
	int x, y;
	int width;
	int height;
	Sprite sprite;
};

class [[deprecated]] Bitmap{
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

class InputBox{
private:
	static uint16_t id_increment;
public:
	InputBox(Allegro* allegro, std::string text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*));
	
	int x;
	int y;
	int height;
	int width;
	short state = 0;
	Allegro* allegro_ptr;
	uint16_t* GUI_keyboard_capture = NULL;
	uint16_t id;
	long data;
	std::string text;
	std::string authorized_chars = "";
	uint_fast32_t cur_pos = 0;
	
	void (*input_validated)(Allegro*, InputBox*);
	
	void setAuthorizedChars(std::string auth_cars){
		authorized_chars = auth_cars;
	}
	
	bool isInside(int x, int y){
		if(this->x < x && this->x+width > x && this->y < y && this->y+height > y){
			return true;
		}
		return false;
	}
	
	void setState(short s){
		if(s == 2){
			state = 2; // We are focused on it
			allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
			if(GUI_keyboard_capture != NULL){
				(*GUI_keyboard_capture) = id;
			}
			drawInputBox();
		} else if(s == 1){
			state = 1;
			allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT);
			if(GUI_keyboard_capture != NULL){
				(*GUI_keyboard_capture) = 0;
			}
			//allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			drawInputBox();
		} else {
			state = 0;
			allegro_ptr->setSystemCursor(ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			if(GUI_keyboard_capture != NULL){
				(*GUI_keyboard_capture) = 0;
			}
			drawInputBox();
		}
	}
	
	int getCurPosFromX(int x){
		uint_fast16_t prev_distance = UINT16_MAX;
		int pos = 0;
		for(unsigned i = 0; i<=text.length(); i++){
			uint_fast16_t distance = abs(x - allegro_ptr->getTextWidth(text.substr(0, i).c_str()));
			if(distance < prev_distance){
				pos = i;
				prev_distance = distance;
			}
		}
		return pos;
	}
	
	void click(int x, int y, uint16_t ev){
		if (not(ev & Allegro::MOUSE_DOWN)){
			return;
		}
		if(isInside(x, y)){
			setState(2);
			int x_pos = x-(this->x + 3);
			
			cur_pos = getCurPosFromX(x_pos);
			//std::cout << getCurPosFromX(x_pos) << std::endl;
		} else {
			setState(0);
		}
	}
	
	void hover(int x, int y){
		if(!allegro_ptr->isMouseBtnDown(1) && state != 2){
			if(isInside(x, y)){
				setState(1);
			} else {
				setState(0);
			}
		}
	}
	
	void key_press(uint16_t ev, uint16_t keycode, char car){
		if(state != 2)
			return;
		if(ev & Allegro::KEY_CHAR){
			if(keycode == ALLEGRO_KEY_ENTER){
				input_validated(allegro_ptr, this);
			} else if(keycode == ALLEGRO_KEY_BACKSPACE){
				if(text.length() > 0){
					text.erase(text.begin()+cur_pos-1);
					//text = text.substr(0, text.length()-1);
					cur_pos = std::min(cur_pos-1, text.length());
				}
			} else if(keycode == ALLEGRO_KEY_RIGHT){
				if(cur_pos < text.length())
					cur_pos++;
			} else if(keycode == ALLEGRO_KEY_LEFT){
				if(cur_pos > 0)
					cur_pos--;
			} else {
				if(!isprint(car))
					return;
				if(authorized_chars.empty() || authorized_chars.find(car) != authorized_chars.npos){
					if(cur_pos >= 0 && cur_pos < text.length())
						text.insert(text.begin()+cur_pos, car);
					else
						text += car;
					cur_pos++;
				}
			}
		}
	}
	
	void drawCaret(){
		int x_pos = x+allegro_ptr->getTextWidth(text.substr(0, cur_pos).c_str()) + 4;
		int y_pos = y+height/2-3;
		int h = allegro_ptr->getFontHeight(allegro_ptr->getDefaultFont());
		
		allegro_ptr->draw_line(x_pos, y_pos, x_pos, y_pos+h, allegro_ptr->rgb(0, 0, 0), 1);
		//std::cout << cur_pos << std::endl;
	}
	
	void drawInputBox(){
		ALLEGRO_COLOR color;
		if(state == 1)
			color = allegro_ptr->rgb(100, 100, 200);
		else if(state == 0)
			color = allegro_ptr->rgb(0, 0, 0);
		else if(state == 2){
			color = allegro_ptr->rgb(0, 0, 255);
		}
		allegro_ptr->draw_rectangle(x, y, x+width, y+height, color, 1, false);
		allegro_ptr->draw_rectangle(x, y, x+width-1, y+height-1, allegro_ptr->rgb(255, 255, 255), 1, true);
		int len = allegro_ptr->getTextWidth(text.c_str());
		std::string printed_text = text;
		int n = text.length()-1;
		//std::cout << id << " : " << width << " px vs len = " << len << " px" << std::endl;
		bool shortenedString = false;
		while(len + 3 + 1 + allegro_ptr->getTextWidth("\u2026") >= width){
			shortenedString = true;
			n--;
			printed_text = text.substr(0, n);
			len = allegro_ptr->getTextWidth(printed_text.c_str());
		}
		if(shortenedString){
			printed_text += "\u2026"; // ellipsis symbol (...)
		}
		allegro_ptr->draw_text(x+3, y+height/2-3, printed_text, allegro_ptr->rgb(0, 0, 0), ALLEGRO_ALIGN_LEFT);
		
		if(state == 2){
			std::chrono::time_point<std::chrono::system_clock> mnt = std::chrono::system_clock::now();
			uint_fast32_t tmp = mnt.time_since_epoch().count()/1000000;
			//std::cout << tmp << std::endl;
			if( tmp%600 > 300 )
				drawCaret();
		}
	}
};

class GUI{
public:
	GUI(Allegro* allegro) {
		this->allegro = allegro;
	}
	
	unsigned newBtn(std::string name, int x, int y, int height, int width, void (*btn_clicked)(Allegro*, Button*)){
		buttons.push_back(Button(allegro, name, x, y, height, width, btn_clicked));
		return buttons.size()-1;
	}
	
	unsigned newInputBox(std::string default_text, int x, int y, int height, int width, void (*input_validated)(Allegro*, InputBox*)){
		InputBox inpt(allegro, default_text, x, y, height, width, input_validated);
		inpt.GUI_keyboard_capture = &keyboardCapture;
		input_boxes.push_back(inpt);
		return input_boxes.size()-1;
	}
	
	unsigned newImage(const char* filename, int x, int y, int height = 0, int width = 0){
		images.push_back(Image(filename, x, y, width, height));
		return images.size()-1;
	}
	
	unsigned newCursor(const char* filename, std::string name){
		cursors.push_back(Cursor(allegro, filename, name));
		return cursors.size()-1;
	}
	
	unsigned getBtnId(unsigned i){
		return buttons.at(i).id;
	}
	
	unsigned getInputBoxId(unsigned i){
		return input_boxes.at(i).id;
	}
	
	void setBtnSprites(unsigned i, Sprite defaut, Sprite clicked, Sprite hovered){
		buttons.at(i).setSprites(defaut, clicked, hovered);
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
	
	void drawInputBox(int i){
		input_boxes[i].drawInputBox();
	}
	
	void drawAllInputBoxes(){
		for(unsigned i = 0; i<input_boxes.size(); i++){
			drawInputBox(i);
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
	
	void mouseClickHandle(uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_L_CLICKED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].click(x, y, ev);
			}
			
			for(unsigned int i=0; i < input_boxes.size(); i++){
				input_boxes[i].click(x, y, ev);
			}
		}
	}
	
	void mouseHoveringHandle(uint16_t ev, int x, int y){
		if(ev & Allegro::MOUSE_MOVED){
			for(unsigned int i=0; i < buttons.size(); i++){
				buttons[i].hover(x, y);
			}
			
			for(unsigned int i=0; i < input_boxes.size(); i++){
				input_boxes[i].hover(x, y);
			}
		}
	}
	
	void keyHandle(uint16_t ev, uint16_t key, char car){
		for(unsigned int i=0; i < input_boxes.size(); i++){
			input_boxes[i].key_press(ev, key, car);
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
	
	int getBtnIndexByID(int id){
		for(unsigned i = 0; i<= buttons.size(); i++){
			if (buttons[i].id == id){
				return i;
			}
		}
	}
	
	int getInputBoxIndexByID(int id){
		for(unsigned i = 0; i<= input_boxes.size(); i++){
			if (input_boxes[i].id == id){
				return i;
			}
		}
	}
	
	void eraseBtn(Button* btn){
		for(unsigned i = 0; i<= buttons.size(); i++){
			if (buttons[i].id == btn->id){
				buttons.erase(buttons.begin() + i);
			}
		}
	}
	
	void eraseInputBox(InputBox* inptbx){
		for(unsigned i = 0; i<= input_boxes.size(); i++){
			if (input_boxes[i].id == inptbx->id){
				input_boxes.erase(input_boxes.begin() + i);
			}
		}
	}
	
	Allegro* allegro;

	int cursor = -1;
	
	uint16_t keyboardCapture = 0;
	
	std::vector<Cursor> cursors;
	std::vector<Image> images;
	std::vector<Button> buttons;
	std::vector<InputBox> input_boxes;
	std::vector<Message> messages;
};
