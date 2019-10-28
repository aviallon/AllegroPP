#include <allegro++/allegro.h>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <filesystem>

using namespace AllegroPP;
using namespace std;
namespace fs = std::filesystem;

using cint = const unsigned int;


class Tile {
public:
	Tile(int x, int y, Color color, int size = 5, int shape = CIRCLE) {
		this->x = x;
		this->y = y;
		this->size = size; 
		this->shape = shape;
		this->color = color;
	}
	
	Tile(){
		
	}
	
	bool isBlank() const{
		if(this->size == 0){
			return true;
		}
		return false;
	}
	
	void drawTile(Allegro* allegro){
		if (shape == CIRCLE) {
			allegro->draw_ellipse(x, y, x + size, y + size, color.toAllegro(), 1, true);
		}
		else {
			allegro->draw_rectangle(x, y, x + size, y + size, color.toAllegro(), 1, true);
		}
	}
	
	bool isInTile(int x, int y){
		if(this->x < x && this->x+size > x && this->y < y && this->y+size > y)
			return true;
		return false;
	}
	
	friend ostream& operator<<(ostream& os, const Tile& t){
      if(t.isBlank())
         return os;
		os << t.x << " " << t.y << " " << t.size << " " << t.shape << " " << t.color.toHex();
		return os;
	}
	
	friend istream& operator>>(istream& is, Tile& t){
		string colorHexStr;
		is >> t.x >> t.y >> t.size >> t.shape >> colorHexStr;
      const int colorHex = stoul(colorHexStr, 0, 16);
		t.color = Color(colorHex);
		return is;
	}

	static cint SQUARE = 1 << 0;
	static cint CIRCLE = 1 << 1;

	int x, y;
	int size;
	int shape;
	Color color;
};

class World {
public:
	World() {

	}

	void newTile(int x, int y, Color color) {
		world.push_back(Tile(x, y, color));
	}

	void newTile(int x, int y, int size, Color color, int shape) {
		world.push_back(Tile(x, y, color, size, shape));
	}
	

	Tile get(int i) {
		return world[i];
	}

	int getX(int i) {
		return world[i].x;
	}

	int getY(int i) {
		return world[i].y;
	}

	int getSize(int i) {
		return world[i].size;
	}

	void setSize(int i, int size) {
		world[i].size = size;
	}

	void drawTile(int i, Allegro* allegro) {
		world[i].drawTile(allegro);
	}
	
	void deleteTilesAtPoint(int x, int y){
		for(unsigned int i=0; i<world.size(); i++){
			if(world[i].isInTile(x, y)){
				world.erase(world.begin() + i);
			}
		}
		
	}


	std::vector<Tile> world;
};

int paintSize = 5;
int shape = Tile::CIRCLE;
bool mouseDown = false;
Color tileColor;
int menuSize = 50;

void redraw(Allegro* allegro, float FPS) {
	World* world = (World*)(allegro->getContext());
	allegro->draw_rectangle(0, 0, allegro->getDisplayWidth(), allegro->getDisplayHeight(), allegro->rgb(255, 255, 255), 1, true);

	for (unsigned int i = 0; i < world->world.size(); i++) {
		world->drawTile(i, allegro);
	}

	allegro->draw_rectangle(0, 0, allegro->getDisplayWidth(), menuSize, allegro->rgb(200, 200, 200), 1, true);

	
	stringstream ss;
	ss << "Taille : " << paintSize;
	allegro->draw_text(allegro->getDisplayWidth()-paintSize-5-allegro->getTextWidth(ss.str(), allegro->getDefaultFont()), 3, ss.str(), allegro->rgb(0, 0, 0));
	Tile demo(allegro->getDisplayWidth()-paintSize-5, 3, tileColor, paintSize, shape);
	demo.drawTile(allegro);
	
	//allegro->getGUI()->drawAllImages();
}

void click(Allegro* allegro, void* cont, uint16_t ev, int x, int y) {
	World* world = (World*)cont;
	if(ev & Allegro::MOUSE_L_CLICKED && y >= menuSize){
		mouseDown = (ev & Allegro::MOUSE_DOWN) ? true : false;
		world->newTile(x - paintSize/2, y - paintSize/2, paintSize, tileColor, shape);
	} else if(ev & Allegro::MOUSE_R_CLICKED && y >= menuSize){
		world->deleteTilesAtPoint(x, y);
	}
}

void move(Allegro* allegro, void* cont, uint16_t ev, int x, int y) {
	World* world = (World*)cont;
	switch (ev) {
		case Allegro::MOUSE_MOVED:
			if (mouseDown && y >= menuSize) {
				world->newTile(x - paintSize/2, y - paintSize/2, paintSize, tileColor, shape);
			}
			break;
		case Allegro::MOUSE_WHEELED:
		if(paintSize+x>1 && paintSize+x<(menuSize-6)){
			paintSize += x;
			break;
		}
	}
}

void saveTheWorld(Allegro* allegro, Button* btn){
	World* world = (World*)(allegro->getContext());
	for (unsigned i=0; i<world->world.size(); i++){
		if(world->world[i].isBlank()){
			world->world.erase(world->world.begin()+i);
		}
	}
	try {
		const char* filename = allegro->askFile("./", "Sauvegarder", "*.twrld", ALLEGRO_FILECHOOSER_SAVE);
		stringstream fnss;
		fnss << filename;
		if(fnss.str().compare("") != 0){
			
			remove(filename);
			ofstream save_file(filename);
			save_file.write("\377\037\n", 3); // Magic number to declare file as application/octet-stream
			ostream_iterator<Tile> output_iterator(save_file, "\n");
			std::copy(world->world.begin(), world->world.end(), output_iterator);
			save_file.close();
			cout << "Saved the world" << endl;
		} else {
			cout << "Aborted by user." << endl;
		}
	} catch (exception e){
		cerr << e.what() << endl;
	}
}

void loadTheWorld(World* world, const char* filename){
	try {
		int counter = 0;
		ifstream open_file(filename);
		world->world = vector<Tile>(10000);
		string line;
		getline(open_file, line);
		while(getline(open_file, line)){
			if(line.compare("") == 0)
				continue;
			stringstream lstr;
			lstr << line;
         Tile temp;
         lstr >> temp;
         if(!temp.isBlank())
            world->world.at(counter) = temp;
			counter++;
		}
		open_file.close();
	} catch (exception e){
		//if(e == std::runtime_error)
		cerr << e.what() << endl;
	}
}

void loadTheWorldBtn(Allegro* allegro, Button* btn){
	World* world = (World*)(allegro->getContext());
	const char* filename = allegro->askFile("./", "Ouvrir", "*.twrld", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	loadTheWorld(world, filename);
}

unsigned savebtn_id;

void key(Allegro* allegro, void* cont, uint16_t ev, uint8_t keycode) {
   Button* savebtn = allegro->getGUI()->getBtn(savebtn_id);
	switch (keycode) {
      case ALLEGRO_KEY_S:
         shape = Tile::SQUARE;
         break;
      case ALLEGRO_KEY_R:
         shape = Tile::CIRCLE;
         break;
      case ALLEGRO_KEY_LSHIFT:
         if(ev & Allegro::KEY_DOWN){
            savebtn->name = "Ouvrir";
            savebtn->btn_clicked = &loadTheWorldBtn;
         } else {
            savebtn->name = "Sauvegarder";
            savebtn->btn_clicked = &saveTheWorld;
         }
   }
}

void exitBtn(Allegro* allegro, Button* btn){
	allegro->screenshot("screen.png", 0, menuSize, allegro->getDisplayWidth(), allegro->getDisplayHeight());
	allegro->quit();
}

void changeColorBtn(Allegro* allegro, Button* btn){
	if(btn->name.compare("Bleu") == 0){
		btn->name = "Rouge";
		tileColor = Color(0, 0, 255);
	} else if(btn->name.compare("Rouge") == 0){
		btn->name = "Noir";
		tileColor = Color(255, 0, 0);
	} else {
		btn->name = "Bleu";
		tileColor = Color(0, 0, 0);
	}
}

void changeShape(Allegro* allegro, Button* btn){
	if(shape == Tile::CIRCLE){
		shape = Tile::SQUARE;
		btn->name = "Cercle";
	} else {
		shape = Tile::CIRCLE;
		btn->name = "Carré";
	}
}

bool getArg(int argc, const char** args, const char* arg){
	for(int i = 0; i<argc; i++){
		if(strcmp(args[i], arg) == 0){
			return true;
		}
	}
	return false;
}

int main(int argc, const char** args) {
   Allegro::init();
   
	Allegro* allegro = new Allegro();
	World* world = new World();
   
	tileColor = Color(0, 0, 0);
	allegro->createWindow(60, 500, 500);
	
	allegro->setContext((void *)world);

	allegro->bindMouseClick(&click);
	allegro->bindMouseMove(&move);
	allegro->bindKeyDown(&key);
	allegro->bindKeyUp(&key);
	
	allegro->getGUI()->newBtn("Exit", 3, 3, 40, 70, &exitBtn);
	
	allegro->getGUI()->newBtn("Bleu", 70+20, 3, 40, 80, &changeColorBtn);
	allegro->getGUI()->newBtn("Carré", 70+20+80+20, 3, 40, 60, &changeShape);
	savebtn_id = allegro->getGUI()->newBtn("Sauvegarder", 70+20+80+20+60+20, 3, 40, 80, &saveTheWorld);
	//allegro->getGUI()->newImage("./linux.png", 50, 50, 0, 0);
	
	if(argc > 1){
      cout << "Loading world " << args[argc-1] << " !" << endl;
		loadTheWorld(world, args[argc-1]);
	}
	
	allegro->setRedrawFunction(&redraw);

	Allegro::startLoop();

	return 0;
}