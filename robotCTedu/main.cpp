#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include "sender.h"

using namespace std;

#define PI		3.14159265358979323846

// coordinate of the origin of base coordinate system
#define BCS_X	500
#define BCS_Y	0
#define BCS_Z	500

typedef struct __POSE_R__
{
	double x;
	double y;
	double z;
	double Rx;
	double Ry;
	double Rz;
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
} POSE_Q;


POSE_Q* getPoses(double r, double alpha, int number)
{
	POSE_R* poses_r = new POSE_R[number];
	POSE_Q* poses_q = new POSE_Q[number];

	// calculate coordinate of every point during the scanning
	for (int i = 0; i < number; i++)
	{
		poses_r[i].x = 0 - r * sin(i * alpha * PI / 180);
		poses_r[i].y = 0 - r * cos(i * alpha * PI / 180);
		poses_r[i].z = 0;
		poses_r[i].Rx = 0;
		poses_r[i].Ry = 0;
		poses_r[i].Rz = (90 - i * alpha) * PI / 180;
		cout << poses_r[i].x << "	" << poses_r[i].y << "	" << poses_r[i].Rz << endl;
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
		cout << poses_q[i].q0 << "	" << poses_q[i].q1 << "	" << poses_q[i].q2 << "	" << poses_q[i].q3
			<< "	" << poses_q[i].q0 * poses_q[i].q0 + poses_q[i].q1 * poses_q[i].q1 + poses_q[i].q2 * poses_q[i].q2 + poses_q[i].q3 * poses_q[i].q3 << endl;
	}

	return poses_q;
}

int main()
{
	double radium = 100;
	double angle = 15;
	int number = int(180 / angle) + 1;
	bool isFinished = false;

	// calculate the poses moved during the scanning
	POSE_Q* poses = getPoses(radium, angle, number);

	// serial port & socket setup
	Sender server = Sender();
	if (!(server.serialSetup() && server.socketSetup()))
	{
		return 1;
	}

	// waiting & accept the connection from robot
	server.socketAccept();
	// start logic

	// main loop
	for (int i = 0; i < number; i++)
	{
		double data[7];
		data[0] = poses[i].x;
		data[1] = poses[i].y;
		data[2] = poses[i].z;
		data[3] = poses[i].q0;
		data[4] = poses[i].q1;
		data[5] = poses[i].q2;
		data[6] = poses[i].q3;
		server.socketSend(data);
		if (server.isAllReached())
		{
			Sleep(2000);
		}
		
	}
	printf("Scanning complete!\n");

	return 0;
}