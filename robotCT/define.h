
// define.h : ��������
//

# pragma once

#ifndef _DEFINE_H
#define _DEFINE_H

#endif

#include <math.h>

/******************����CT���ζ���*******************/

#define DELTA_ANGLE		5						// CTͶӰ�Ƕȼ��
#define NUMBER_ANGLE	(180 / DELTA_ANGLE + 1)	// CTͶӰ�Ƕ���
#define PI				3.14159265359


/**********************����ϵ***********************/

// ��ʼλ��ŷ���Ƕ�������ֵ
#define BCS_X		350.0
#define BCS_Y		0.0
#define BCS_Z		600.0
#define BCS_RX		0.0
#define BCS_RY		(PI / 2)
#define BCS_RZ		0.0

// ��е��ĩ��λ��: ŷ���Ƕ���
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

// ��е��ʵ������λ��(���ⲿ��): ��Ԫ������
typedef struct __POSE_Q__
{
	double x;
	double y;
	double z;
	double q0;
	double q1;
	double q2;
	double q3;
	double w;		// �ⲿ��(����)
	int index;		// ����λ�����(��0��ʼ, -1Ϊ������Ĺ��ɵ�)
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

#define	GUIDE_MAX_SIDE	230.0			// ���쵥��������г���
#define	GUIDE_POS_SIDE	5				// ���쵥�������λ����(���ٵ����˶�����, �Ի�е���˶�����Ƶ���ĵ����˶�)
#define	GUIDE_GAP		GUIDE_MAX_SIDE / GUIDE_POS_SIDE


/*****************������ʽ�Ի���******************/

// �����ֵ
#define EXP_P		10
#define	EXP_ADD		11
#define EXP_SUB		12
#define EXP_MUL		13
#define EXP_DIV		14
#define EXP_LBR		15
#define EXP_RBR		16

// ���Ǻ���ֵ
#define	EXP_THE		21
#define EXP_SIN		22
#define EXP_COS		23
#define EXP_TAN		24


/*******************ͨ���豸���********************/

#define DEVICE_SERVER		101
#define DEVICE_ROBOT_0		102
#define DEVICE_ROBOT_1		103
#define DEVICE_DETECTOR		104
#define DEVICE_SERIAL		105


/*********************ͨ��Э��**********************/

// ����ͨ��Э��


// ����ͨ�Ŷ˿�
#define SERVER_PORT			6000

// ����ͨ��Э��
#define PROTOCAL_CONNECT			"a0"
#define PROTOCAL_ROBOT_MOVE			"RM\r"
#define	PROTOCAL_ROBOT_REACH		"RR\r"
#define	PROTOCAL_DETECTOR_SHOW		"DS\r"
#define	PROTOCAL_DETECTOR_FINISH	"DF\r"

// ����������
#define BUFFER_SERIAL_RECEIVE	7
#define BUFFER_SERIAL_SEND		10
#define BUFFER_SOCKET_RECEIVE	3
#define BUFFER_SOCKET_SEND		256


/*****************����������ò���******************/

#define LEAD			5.0		// ˿���ݾ�(mm)

// ϸ��
#define	DIVIDE_2		2
#define DIVIDE_4		4
#define DIVIDE_8		8
#define DIVIDE_16		16

// �����
#define STEPPR_50		50		// 7.2
#define STEPPR_100		100		// 3.6
#define STEPPR_200		200		// 1.8
#define STEPPR_400		400		// 0.9


/******************�Զ�����Ϣ�б�*******************/

// �����߳���Ϣ
#define WM_ERROR			WM_USER + 0x01
#define WM_CONNECT			WM_USER + 0x02
#define WM_REACH			WM_USER + 0x03
#define	WM_FINISH			WM_USER + 0x04

// �û������߳���Ϣ
#define WM_THREADTIMER		WM_USER + 0x21
#define WM_START			WM_USER + 0x22
#define	WM_SHOW				WM_USER + 0x23


/*********************��������**********************/

// ������Ϣ����
#define	SOCKET_INITIAL		201		// �׽��ֳ�ʼ������

// ���ȶ���(���ڸ�����ת�ַ���)
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