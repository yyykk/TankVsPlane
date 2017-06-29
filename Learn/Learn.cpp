// Learn.cpp : 定义控制台应用程序的入口点。
//

#include "object.h"

IMAGE pic, doctor, fire, boom, tank, plane, biu;
HDC PicHDC;
int life = 3;

struct object {
	int time;
	int x;
	int y;
	HDC imghdc;
	int maskcolor;
};

vector<object> vbiu;

object Initobject(int x, int y) {
	DWORD *pmask;
	object Newobject;
	Newobject.time = 7 * 4;
	Newobject.x = x;
	Newobject.y = y;
	Newobject.imghdc = GetImageHDC(&biu);
	pmask = GetImageBuffer(&biu);
	Newobject.maskcolor = BGR(pmask[0]);
	return Newobject;
}

void TranImage(struct object *object) {
	TransparentBlt(PicHDC, object->x, object->y,
		BiuWeight, BiuHigh, object->imghdc,
		0, 0,
		BiuWeight, BiuHigh, object->maskcolor);
}

void Fireobject(vector<object> &vobject) {
	for (int i = 0; i < vobject.size(); ++i) {
		if (vobject[i].y < MaxHigh && vobject[i].time > 0) {
			vobject[i].y += BiuHigh / 6;
			TranImage(&vobject[i]);
		}
	}
}

struct Boom {
	int time;
	int x;
	int y;
	HDC imghdc;
	int maskcolor;
};

vector<Boom> vboom;

Boom InitBoom(int x, int y) {
	DWORD *pmask;
	Boom NewBoom;
	NewBoom.time = 7 * 4;
	NewBoom.x = x;
	NewBoom.y = y;
	NewBoom.imghdc = GetImageHDC(&boom);
	pmask = GetImageBuffer(&boom);
	NewBoom.maskcolor = BGR(pmask[0]);
	return NewBoom;
}

void TranImage(struct Boom *boom) {
	TransparentBlt(PicHDC, boom->x, boom->y,
		PlaneWeight, PlaneHigh, boom->imghdc, 
		0, 0, 
		PlaneWeight, PlaneHigh, boom->maskcolor);
}

void FireBoom() {
	for (int i = 0; i < vboom.size(); ++i) {
		if (vboom[i].y > 0) {
			vboom[i].y -= TankHigh / 4;
			TranImage(&vboom[i]);
		}
	}
}

struct Plane {
	int time;
	int x;
	int y;
	int d;
	HDC imghdc;
	int maskcolor;
};

vector<Plane> vplane;

Plane InitPlane(int x, int y, int d) {
	DWORD *pmask;
	Plane SlefPlane;
	SlefPlane.time = 12 * 4;
	SlefPlane.x = x;
	if (d == 1) {
		SlefPlane.x = 0;
	}
	SlefPlane.y = y;
	SlefPlane.d = d;
	SlefPlane.imghdc = GetImageHDC(&plane);
	pmask = GetImageBuffer(&plane);
	SlefPlane.maskcolor = BGR(pmask[0]);
	return SlefPlane;
}

void TranImage(struct Plane *plane) {
	TransparentBlt(PicHDC, plane->x, plane->y,
		PlaneWeight, PlaneHigh, plane->imghdc,
		0, 0,
		PlaneWeight, PlaneHigh, plane->maskcolor);
}

void FirePlaneRight() {
	for (int i = 0; i < vplane.size(); ++i) {
		if (vplane[i].x >= 0 && vplane[i].x <= MaxWeight && vplane[i].time > 0) {
			if (vplane[i].d == 0) {
				vplane[i].x -= PlaneHigh / 4;
			}else {
				vplane[i].x += PlaneHigh / 4;
			}
			if (rand() % 7 == 0 && rand() % 7 == 0) {
				vbiu.push_back(Initobject(vplane[i].x, vplane[i].y + 50));
			}
			TranImage(&vplane[i]);
		}
	}
}

struct Tank {
	int x;
	int y;
	HDC imghdc;
	int maskcolor;
};

void InitTank(Tank *SlefTank,int x, int y) {
	DWORD *pmask;
	SlefTank->x = x;
	SlefTank->y = y;
	SlefTank->imghdc = GetImageHDC(&tank);
	pmask = GetImageBuffer(&tank);
	SlefTank->maskcolor = BGR(pmask[0]);
}

void TranImage(struct Tank *tank) {
	TransparentBlt(PicHDC, tank->x, tank->y,
		PlaneWeight, PlaneHigh, tank->imghdc,
		0, 0,
		PlaneWeight, PlaneHigh, tank->maskcolor);
}

void Destory(int x) {
	int i = 0, j = 0, k = 0;
	for (; i < vplane.size(); ++i) {
 		if (vplane[i].time <= 0) continue;
 		for (; j < vboom.size(); ++j) {
			if (vboom[j].time <= 0) continue;
			if ((abs(vplane[i].x - vboom[j].x) <= PlaneHigh) && (abs(vplane[i].y - vboom[j].y) <= PlaneWeight)) {
				vplane[i].time = 0;
				vboom[j].time = 0;
				putimage(vplane[i].x, vplane[i].y, &doctor);
			}
		}
	}
	if (life > 0) {
		for (; k < vbiu.size(); ++k) {
			if (vbiu[k].y > MaxHigh) continue;
			if (MaxHigh - vbiu[k].y < TankHigh && vbiu[k].x - x < TankWeight && vbiu[k].x - x > 0) {
				vbiu[k].time = 0;
				life -= 1;
				if (life <= 0) {
					loadimage(&tank, _T("./source/doctor.jpg"), TankWeight, TankHigh);
				}
			}
		}
	}
}

void InitImage() {
	initgraph(MaxWeight, MaxHigh);

	loadimage(&biu, _T("./source/biu.bmp"), BiuWeight, BiuHigh);

	loadimage(&boom, _T("./source/Boom.bmp"), TankWeight, TankHigh);

	loadimage(&pic, _T("./walle.jpg"), MaxWeight, MaxHigh);

	loadimage(&tank, _T("./source/Tank.bmp"), TankWeight, TankHigh);

	loadimage(&doctor, _T("./source/doctor.jpg"), TankWeight, TankHigh);

	loadimage(&fire, _T("./source/Boom.jpg"), FireWeight, FireHigh);

	loadimage(&plane, _T("./source/Plane.bmp"), TankWeight, TankHigh);
}

int main() {
	int x = MaxWeight / 2, y = MaxHigh - 50;
	
	long int TankStart = clock(), PlaneStart = clock(), BoomStart = clock(), BiuStart = clock();

	Tank selfTank;
	
	srand((unsigned)time(NULL));

	InitImage();

	while (1) {
		loadimage(&pic, _T("./walle.jpg"), MaxWeight, MaxHigh);
		PicHDC = GetImageHDC(&pic);
		InitTank(&selfTank, x, y);
		TranImage(&selfTank);
		if (clock() - PlaneStart >= 100) {
			if (rand() % 3 == 0) {
				vplane.push_back(InitPlane(MaxWeight, rand() % 201, rand() % 2));
			}
			PlaneStart = clock();
		}
		if (clock() - TankStart >= 50) {
			FirePlaneRight();
			TankStart = clock();
		}
		if (clock() - BiuStart >= 50) {
			Fireobject(vbiu);
			BiuStart = clock();
		}
		if (clock() - BoomStart >= 20) {
			if (_kbhit()) {
				if (GetAsyncKeyState(VK_LEFT)) {
					if(x > 0) x -= 20;
				}
				if (GetAsyncKeyState(VK_RIGHT)) {
					if(x < MaxWeight - 20) x += 20;
				}
				if (GetAsyncKeyState(VK_ESCAPE)) {
					life += 1;
					loadimage(&tank, _T("./source/Tank.bmp"), TankWeight, TankHigh);
				}
				if (GetAsyncKeyState(VK_SPACE)) {
					vboom.push_back(InitBoom(x, y));
				}
			}
			FireBoom();
			SetWorkingImage();
			putimage(0, 0, &pic);
			TankStart = clock();
		}
		Destory(x);
	}
	system("pause");
	return 0;
}