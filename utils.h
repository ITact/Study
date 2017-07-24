#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//������㷨

//����x,y֮��
inline int RandInt(int x,int y) {return rand()%(y-x+1)+x;};

inline double RandFloat() {return (rand())/(RAND_MAX+1.0);}

//�������bool
inline bool RandBool()
{
	if (RandInt(0,1)) return true;
	else return false;
}

//���-1 -- 1
inline double RandomClamped()	   {return RandFloat() - RandFloat();}

string itos(int arg);

string ftos (float arg);


void Clamp(double &arg, double min, double max);

struct SPoint
{
	float x, y;
	
	SPoint(){}
	SPoint(float a, float b):x(a),y(b){}
};

#endif