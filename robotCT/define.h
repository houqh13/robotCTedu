# pragma once

// define.h : 参数定义


// 基本CT几何定义

#define DELTA_ANGLE		5						// CT投影角度间隔
#define NUMBER_ANGLE	(180 / DELTA_ANGLE + 1)	// CT投影角度数


// 输入表达式对话框

// 类型
#define	EXP_OPR		1
#define EXP_TRI		2
#define EXP_NUM		3

// 运算符值

#define	EXP_ADD		11
#define EXP_SUB		12
#define EXP_MUL		13
#define EXP_DIV		14
#define EXP_LBR		15
#define EXP_RBR		16

// 三角函数值
#define EXP_SIN		21
#define EXP_COS		22
#define EXP_TAN		23
//#define	EXP_P		8