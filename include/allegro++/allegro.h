#pragma once

#define __ENABLE_GUI__

#include <cmath>
#include <cctype>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <memory>

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/file.h>
//#include <allegro5/primitives.h>
#if __cplusplus >= 201703L
   #include <filesystem>
#else
   #include <experimental/filesystem>
   namespace std {
      namespace filesystem = std::experimental::filesystem::v1;
   }
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <armadillo>

/*
   ARMADILLO LIB CITES :

   Conrad Sanderson and Ryan Curtin.
   Armadillo: a template-based C++ library for linear algebra.
   Journal of Open Source Software, Vol. 1, pp. 26, 2016.

   Conrad Sanderson and Ryan Curtin.
   Practical sparse matrices in C++ with hybrid storage and template-based expression optimisation.
   Mathematical and Computational Applications, Vol. 24, No. 3, 2019. 
*/

namespace AllegroPP {
   using timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
   using duration = std::chrono::duration<double>;
   using clock = std::chrono::high_resolution_clock;
}

#define MAX(a, b) (((a > b))?(a):(b))
#define MIN(a, b) (((a < b))?(a):(b))

#define FRAME_SKIPPED_THRESHOLD 5

#include <allegro++/math.h>
#include <allegro++/mouse.h>
#include <allegro++/color.h>
#include <allegro++/primitives.h>
#include <allegro++/gui.h>