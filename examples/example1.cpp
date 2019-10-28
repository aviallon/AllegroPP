#include <allegro++/allegro.h>

int main(){
 	Allegro* allegro = new Allegro();
	allegro->init(); // Required before doing anything else
	allegro->createWindow(30, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.

	allegro->draw_text(640/2, 480/2, "Hello, world!", allegro->rgb(0, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).

	allegro->gameLoop(); // starts the loop

	delete allegro;

	return 0;
}
