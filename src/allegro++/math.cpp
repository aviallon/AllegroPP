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
   
   
   Vec3D::Vec3D(arma::vec3 v){
      vec = v;
   }
   
   Vec3D::Vec3D(double x, double y, double z){
      vec = {x, y, z};
   }
   
   double Vec3D::operator *(const Vec3D &v2){
      return arma::dot(vec, v2.vec);
   }
   
   Vec3D operator *(const Vec3D &v, double k){
      return v*k;
   }
   
   Vec3D operator *(double k, const Vec3D &v){
      return Vec3D(v.vec*k);
   }
   
   Vec3D Vec3D::operator /(double k){
      return Vec3D(vec/k);
   }
   
   Vec3D Vec3D::operator ^(const Vec3D &v2){
      return Vec3D(arma::cross(vec, v2.vec));
   }
   
   double Vec3D::getX(){
      return vec[0];
   }
   
   double Vec3D::getY(){
      return vec[1];
   }
   
   double Vec3D::getZ(){
      return vec[2];
   }
   
   void Vec3D::setX(double x){
      vec[0] = x;
   }
   
   void Vec3D::setY(double y){
      vec[1] = y;
   }
   
   void Vec3D::setZ(double z){
      vec[2] = z;
   }
   
   std::ostream& operator<<(std::ostream& os, const Vec3D &v){
      os << "Vec3D(" << v.vec[0] << "," << v.vec[1] << "," << v.vec[2] << ")";
      return os;
   }
}
