#include "../allegro/allegro.h"

int i = 0;
int i2 = 128;
void fen1redr(Allegro* allegro, float FPS){
	allegro->clearScreen();
	
	i++;
	allegro->draw_text(640/2, 480/2, "Hello, world!", allegro->rgb(i%255, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
}

void fen2redr(Allegro* allegro, float FPS){
	allegro->clearScreen();
	
	i2++;
	allegro->draw_text(200/2, 200/2, "Hello, world!", allegro->rgb(i2%255, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
}

void testBtn(Allegro* allegro, Button* btn){
	std::cout << btn->name << std::endl;
}

int main(){
	Allegro::init(); // Required before doing anything else
	
	Allegro* fen1 = new Allegro();
	Allegro* fen2 = new Allegro();
	
	fen1->createWindow(30, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.
	fen2->createWindow(30, 200, 200);

	fen1->setRedrawFunction(&fen1redr);
	fen2->setRedrawFunction(&fen2redr);
	
	fen1->getGUI()->newBtn("Test", 0, 0, 60, 150, &testBtn);
	fen2->getGUI()->newBtn("Pomme", 100, 0, 60, 150, &testBtn);

	Allegro::startLoop(); // starts the loop

	delete fen1, fen2;

	return 0;
}
