#include <allegro++/allegro.h>

using namespace AllegroPP;

void mouseMove(Allegro* allegro, void* context, uint16_t event, int x, int y){
	if(event & Allegro::MOUSE_MOVED && !(event & Allegro::MOUSE_WHEELED)) {
		allegro->draw_rectangle(0, 0, 640, 480, allegro->rgb(255, 255, 255), 1, true); // we erase the screen
		allegro->draw_ellipse_r(x, y, 8, 8, allegro->rgb(0, 0, 0), 2, false); //... and draw an circle at the mouse coordinates.
	}

	// The context pointer is used to share any kind of information between different functions bound to events. I'll make an example about that later.
}

int main(){
   Allegro::init();
	Allegro allegro = Allegro();
	allegro.createWindow(30, 640, 480);


	allegro.bindMouseMove(&mouseMove); // bind mouse moved events to mouseMove function
   
   // We have to stop automatic redraw at each frame.
   // Note : This is NOT recomended, and used here for the sake of simplicity.
   allegro.stopRedraw();
   
	Allegro::startLoop();

	return 0;
}
