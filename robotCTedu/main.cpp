#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include "controller.h"

using namespace std;

#define PI		3.14159265358979323846

// coordinate of the origin of base coordinate system
#define BCS_X	520
#define BCS_Y	0
#define BCS_Z	600

typedef struct __POSE_R__
{
	double x;
	double y;
	double z;
	double Rx;
	double Ry;
	double Rz;
	double w;		// external axis
} POSE_R;

typedef struct __POSE_Q__
{
	double x;
	double y;
	double z;
	double q0;
	double q1;
	double q2;
	double q3;
	double w;		// external axis
} POSE_Q;


int main()
{
	double r = 200;
	double delta = 5;
	int number = 36;

	POSE_R poses_r[36];
	POSE_Q poses_q[36];
	
	// calculate the poses moved during the scanning
	for (int i = 0; i < number; i++)
	{
		poses_r[i].x = 0 - r * sin(i * delta * PI / 180);
		poses_r[i].y = 0.2 * r * cos(i * delta * PI / 180);
		poses_r[i].z = 0;
		poses_r[i].Rx = 0;
		poses_r[i].Ry = 0;
		poses_r[i].Rz = (90 - i * delta) * PI / 180;
		poses_r[i].w = 0 - 1.2 * r * cos(i * delta * PI / 180);
	}

	// transfer pose variable from Ruler angle to Quaternion
	for (int i = 0; i < number; i++)
	{
		poses_q[i].x = BCS_X + poses_r[i].x;
		poses_q[i].y = BCS_Y + poses_r[i].y;
		poses_q[i].z = BCS_Z + poses_r[i].z;
		poses_q[i].q0 = cos(poses_r[i].Rx / 2) * cos(poses_r[i].Ry / 2) * cos(poses_r[i].Rz / 2)
			+ sin(poses_r[i].Rx / 2) * sin(poses_r[i].Ry / 2) * sin(poses_r[i].Rz / 2);
		poses_q[i].q1 = sin(poses_r[i].Rx / 2) * cos(poses_r[i].Ry / 2) * cos(poses_r[i].Rz / 2)
			- cos(poses_r[i].Rx / 2) * sin(poses_r[i].Ry / 2) * sin(poses_r[i].Rz / 2);
		poses_q[i].q2 = cos(poses_r[i].Rx / 2) * sin(poses_r[i].Ry / 2) * cos(poses_r[i].Rz / 2)
			+ sin(poses_r[i].Rx / 2) * cos(poses_r[i].Ry / 2) * sin(poses_r[i].Rz / 2);
		poses_q[i].q3 = cos(poses_r[i].Rx / 2) * cos(poses_r[i].Ry / 2) * sin(poses_r[i].Rz / 2)
			- sin(poses_r[i].Rx / 2) * sin(poses_r[i].Ry / 2) * cos(poses_r[i].Rz / 2);
		poses_q[i].w = poses_r[i].w;
	}

	// serial port & socket setup
	Controller server = Controller();
	if (!(server.serialSetup() && server.socketSetup()))
	{
		return 1;
	}

	// waiting & accept the connection from robot
	server.socketAccept();

	// main loop
	double data[7];
	for (int i = 0; i < number; i++)
	{
		printf("Moving to pose[%d]...\n", i);
		data[0] = poses_q[i].x;
		data[1] = poses_q[i].y;
		data[2] = poses_q[i].z;
		data[3] = poses_q[i].q0;
		data[4] = poses_q[i].q1;
		data[5] = poses_q[i].q2;
		data[6] = poses_q[i].q3;
		server.serialSend(poses_q[i].w);
		server.serialReached();
		server.socketSend(data);
		server.socketReached();
		printf("Scanning...\n");
		Sleep(2000);
	}

	printf("Moving to the start pose...\n");
	data[0] = poses_q[number / 2].x;
	data[1] = poses_q[number / 2].y;
	data[2] = poses_q[number / 2].z;
	data[3] = poses_q[number / 2].q0;
	data[4] = poses_q[number / 2].q1;
	data[5] = poses_q[number / 2].q2;
	data[6] = poses_q[number / 2].q3;
	server.socketSend(data);
	server.socketReached();
	server.serialSend(poses_q[number / 2].w);
	server.serialReached();
	printf("Scanning complete!\n");

	return 0;
}