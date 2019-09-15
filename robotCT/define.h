
// define.h : 参数定义
//

# pragma once

#ifndef _DEFINE_H
#define _DEFINE_H

#endif

#include <math.h>

/******************基本CT几何定义*******************/

#define DELTA_ANGLE		5						// CT投影角度间隔
#define NUMBER_ANGLE	(180 / DELTA_ANGLE + 1)	// CT投影角度数
#define PI				3.14159265359


/**********************坐标系***********************/

// 初始位置欧拉角定义坐标值
#define BCS_X		350.0
#define BCS_Y		0.0
#define BCS_Z		600.0
#define BCS_RX		0.0
#define BCS_RY		(PI / 2)
#define BCS_RZ		0.0

// 机械臂末端位置: 欧拉角定义
typedef struct __POSE_R__
{
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
	__POSE_R__(double x, double y, double z, double rx, double ry, double rz)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->rx = rx;
		this->ry = ry;
		this->rz = rz;
	}
} POSE_R;

// 机械臂实际运行位置(含外部轴): 四元数定义
typedef struct __POSE_Q__
{
	double x;
	double y;
	double z;
	double q0;
	double q1;
	double q2;
	double q3;
	double w;		// 外部轴(导轨)
	int index;		// 成像位置序号(从0开始, -1为不成像的过渡点)
	__POSE_Q__(double x, double y, double z, double q0, double q1, double q2, double q3, double w, int index)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->q0 = q0;
		this->q1 = q1;
		this->q2 = q2;
		this->q3 = q3;
		this->w = w;
		this->index = index;
	}
	__POSE_Q__(POSE_R pose_r, double w = 0.0, int index = -1)
	{
		x = pose_r.x;
		y = pose_r.y - w;
		z = pose_r.z;
		q0 = cos(pose_r.rx / 2) * cos(pose_r.ry / 2) * cos(pose_r.rz / 2)
			+ sin(pose_r.rx / 2) * sin(pose_r.ry / 2) * sin(pose_r.rz / 2);
		q1 = sin(pose_r.rx / 2) * cos(pose_r.ry / 2) * cos(pose_r.rz / 2)
			- cos(pose_r.rx / 2) * sin(pose_r.ry / 2) * sin(pose_r.rz / 2);
		q2 = cos(pose_r.rx / 2) * sin(pose_r.ry / 2) * cos(pose_r.rz / 2)
			+ sin(pose_r.rx / 2) * cos(pose_r.ry / 2) * sin(pose_r.rz / 2);
		q3 = cos(pose_r.rx / 2) * cos(pose_r.ry / 2) * sin(pose_r.rz / 2)
			- sin(pose_r.rx / 2) * sin(pose_r.ry / 2) * cos(pose_r.rz / 2);
		this->w = w;
		this->index = index;
	}
} POSE_Q;

#define	GUIDE_MAX_SIDE	230.0			// 导轨单侧最大运行长度
#define	GUIDE_POS_SIDE	5				// 导轨单侧可运行位置数(减少导轨运动次数, 以机械臂运动代替频繁的导轨运动)
#define	GUIDE_GAP		GUIDE_MAX_SIDE / GUIDE_POS_SIDE


/*****************输入表达式对话框******************/

// 运算符值
#define EXP_P		10
#define	EXP_ADD		11
#define EXP_SUB		12
#define EXP_MUL		13
#define EXP_DIV		14
#define EXP_LBR		15
#define EXP_RBR		16

// 三角函数值
#define	EXP_THE		21
#define EXP_SIN		22
#define EXP_COS		23
#define EXP_TAN		24


/*******************通信设备编号********************/

#define DEVICE_SERVER		101
#define DEVICE_ROBOT_0		102
#define DEVICE_ROBOT_1		103
#define DEVICE_DETECTOR		104
#define DEVICE_SERIAL		105


/*********************通信协议**********************/

// 串口通信协议


// 网络通信端口
#define SERVER_PORT			6000

// 网络通信协议
#define PROTOCAL_CONNECT			"a0"
#define PROTOCAL_ROBOT_MOVE			"RM\r"
#define	PROTOCAL_ROBOT_REACH		"RR\r"
#define	PROTOCAL_DETECTOR_SHOW		"DS\r"
#define	PROTOCAL_DETECTOR_FINISH	"DF\r"

// 缓冲区长度
#define BUFFER_SERIAL_RECEIVE	7
#define BUFFER_SERIAL_SEND		10
#define BUFFER_SOCKET_RECEIVE	3
#define BUFFER_SOCKET_SEND		256


/*****************步进电机设置参数******************/

#define LEAD			5.0		// 丝杠螺距(mm)

// 细分
#define	DIVIDE_2		2
#define DIVIDE_4		4
#define DIVIDE_8		8
#define DIVIDE_16		16

// 步距角
#define STEPPR_50		50		// 7.2
#define STEPPR_100		100		// 3.6
#define STEPPR_200		200		// 1.8
#define STEPPR_400		400		// 0.9


/******************自定义消息列表*******************/

// 工作线程消息
#define WM_ERROR			WM_USER + 0x01
#define WM_CONNECT			WM_USER + 0x02
#define WM_REACH			WM_USER + 0x03
#define	WM_FINISH			WM_USER + 0x04

// 用户界面线程消息
#define WM_THREADTIMER		WM_USER + 0x21
#define WM_START			WM_USER + 0x22
#define	WM_SHOW				WM_USER + 0x23


/*********************其他定义**********************/

// 错误消息参数
#define	SOCKET_INITIAL		201		// 套接字初始化错误

// 精度定义(用于浮点数转字符串)
#define	PRECISION_1			1
#define	PRECISION_2			2
#define	PRECISION_3			3
#define	PRECISION_4			4
#define	PRECISION_5			5
#define	PRECISION_6			6
#define	PRECISION_7			7
#define	PRECISION_8			8
#define	PRECISION_9			9
#define	PRECISION_10		10