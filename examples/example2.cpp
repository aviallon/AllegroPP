#include "../allegro/allegro.h"

void mouseMove(Allegro* allegro, void* context, unsigned char event, int x, int y){
	allegro->draw_rectangle(0, 0, 640, 480, allegro->rgb(255, 255, 255), 1, true); // we erase the screen
	allegro->draw_ellipse_r(x, y, 8, 8, allegro->rgb(0, 0, 0), 2, false); //... and draw an circle at the mouse coordinates.
}

int main(){
	Allegro* allegro = new Allegro();
	allegro->init(); // Required before doing anything else
	allegro->createWindow(30, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.
	//allegro->draw_text(640/2, 480/2, "Hello, world!", allegro->rgb(0, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).

	allegro->bindMouseMove(mouseMove); // bind mouse moved events to mouseMove function
	allegro->gameLoop(); // starts the loop

	delete allegro;

	return 0;
}
