#include <iostream>
#include <Windows.h>

using namespace std;

#define PI 3.14159265358979323846

typedef struct __POSE__
{
	double x;
	double y;
	double z;
	double q1;
	double q2;
	double q3;
	double q4;
} POSE;


POSE* getPoses(double r, double alpha)
{
	int number = int(180 / alpha);
	POSE* poses = new POSE[number + 1];
	for (int i = 0; i <= number; i++)
	{
		poses[i].x = 0 - r * sin(i * alpha * PI / 180);
		poses[i].y = r * cos(i * alpha * PI / 180);
		poses[i].z = 0;
		// q1, q2, q3, q4...
	}

	return poses;
}

int main()
{
	double radium = 100;
	double angle = 15;

	POSE* poses = getPoses(radium, angle);
	for (int i = 0; i <= 180 / angle; i++)
	{
		cout << poses[i].x << "	" << poses[i].y << endl;
	}


	return 0;
}