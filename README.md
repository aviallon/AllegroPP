# Use

Just add the files in an "allegro" folder in your project
You'll also have to add these compiler options (gcc) :
`-lallegro -lallegro_image -lallegro_primitives -lallegro_memfile -lallegro_ttf -lallegro_font -Wl,--format=binary -Wl,allegro/arial.ttf -Wl,--format=default`.
The first part is just here to link against allegro libs, and the second part (with `-Wl`) is here in order to _include_ arial.ttf file _into_ the final executable. The advantage is to avoid having to share a font file with your project as a separate file.

If you want a comparison, this wrapper works _a lot_ like Tkinter in Python :
you instantiate an `Allegro` object and everything uses it.

```c++
#include "allegro/allegro.h"

int main(){
	  Allegro* allegro = new Allegro();
	  allegro->init(); // Required before doing anything else
	  allegro->createWindow(30, 640, 480); // Create a window with a width of 640 and a height of 480, refreshing at 30 fps.
	
	  allegro->draw_text(640/2, 480/2, "Hello, world!", allegro->rgb(0, 0, 0)); // Draw "Hello, world!" in the middle of the screen (in black).
  
	  allegro->gameLoop(); // starts the loop
	  
	  delete allegro;
	  return 0;
}
```

I'll give more detailed examples in a dedicated folder.
