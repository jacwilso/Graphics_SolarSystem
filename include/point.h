#ifndef POINT_H
#define POINT_H

class Point{
  private:
    float x,y,z; // x, y, z coordinates

  public:
    Point(); // default constructor
    Point(float x,float y,float z); // paramaterized constructor
    // get the private variables of the point
    float getX();
    float getY();
    float getZ();

    // operator overloading
    void operator=(Point p);
    void operator+=(Point p);
    void operator-=(Point p);
};

Point operator*(Point p,float f);
Point operator*(float f,Point p);
Point operator+(Point p1,Point p2);

#endif
