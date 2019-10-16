#pragma once

#include "../InterruptController.h"

#define PortIntMasterControl		0x20
#define PortIntMasterControlMask	0x21
#define PortIntServantControl		0xa0
#define PortIntServantControlMask	0xa1

//ICW1
#define WRITE_ICW4	1
#define SINGLE_CHIP 2
#define ICW1_BASE	0x10

//ICW2	为中断号开始号，8对齐
#define MASTER_INT_VEC 0x20
#define SERVANT_INT_VEC 0x28
//ICW3	主片各位为1表示主片上的某个IR连有从片
#define MASTER_ICW3 0x4
//		从片低3位表示连接到主片的IR引脚编号
#define SERVANT_ICW3 0x2
//ICW4
#define ICW4_BASE	0x01

class I82C59AInterruptController :public InterruptController{
	virtual ~I82C59AInterruptController();
	virtual void EnableInterrupt(IntType type);
	virtual void DisableInterrupt(IntType type);
	virtual void init();
};
