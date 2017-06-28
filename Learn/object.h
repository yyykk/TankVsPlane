#ifndef  OBJECT_H
#define OBJECT_H

#pragma comment(lib, "MSIMG32.LIB")
#include "stdio.h"
#include "string.h"
#include "string"
#include "cstring"
#include "graphics.h"
#include "iostream"
#include "vector"
#include "time.h"
#include "conio.h"
#include "stdlib.h"
#include "math.h"

using namespace std;

#define MaxWeight 600
#define MaxHigh 400
#define TankWeight 50
#define TankHigh 50
#define BiuWeight 50
#define BiuHigh 50
#define PlaneWeight 50
#define PlaneHigh 50
#define FireWeight 50
#define FireHigh 350
#define MaxTime 7

HDC Pic;

struct object {
	int time;
	int x;
	int y;
	HDC imghdc;
	int maskcolor;
};

object Initobject(int x, int y, IMAGE source) {
	DWORD *pmask;
	object Newobject;
	Newobject.time = 7 * 4;
	Newobject.x = x;
	Newobject.y = y;
	Newobject.imghdc = GetImageHDC(&source);
	pmask = GetImageBuffer(&source);
	Newobject.maskcolor = BGR(pmask[0]);
	return Newobject;
}

void TranImage(struct object *boom) {
	TransparentBlt(Pic, boom->x, boom->y,
		PlaneWeight, PlaneHigh, boom->imghdc,
		0, 0,
		PlaneWeight, PlaneHigh, boom->maskcolor);
}

void Fireobject(vector<object> vobject) {
	for (int i = 0; i < vobject.size(); ++i) {
		if (vobject[i].time > 0) {
			vobject[i].time -= 1;
			vobject[i].y -= TankHigh / 4;
			TranImage(&vobject[i]);
		}
	}
}

#endif // ! OBJECT_H

