#include <allegro++/math.h>

namespace AllegroPP {
   namespace Math {
      int clamp(double x, int min, int max){
         return floor((x>max) ? max : ((x<min) ? min : x));
      }

      float clamp(double x, float min, float max){
         return (x>max) ? max : ((x<min) ? min : x);
      }
   }
   
   Vec2D::Vec2D(double x, double y){
      vec = { x, y };
   }
   
   Vec2D::Vec2D(arma::vec2 v){
      vec = v;
   }
   
   double Vec2D::operator *(const Vec2D &v2){
      return arma::dot(vec, v2.vec);
   }
   
   Vec2D operator *(const Vec2D &v, double k){
      return Vec2D(v.vec*k); 
   }
   
   Vec2D operator *(double k, const Vec2D &v){
      return Vec2D(v.vec*k); 
   }
   
   Vec2D Vec2D::operator/(double k){
      return Vec2D(vec/k);
   }
   
   std::ostream& operator<<(std::ostream& os, const Vec2D &v){
      os << "Vec2D(" << v.vec[0] << "," << v.vec[1] << ")";
      return os;
   }
}
