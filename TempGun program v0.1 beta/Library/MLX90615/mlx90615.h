#ifndef __MLX90615_H
#define __MLX90615_H

#include "N76E003_BSP.h"

void mlx90615Init(void);

// ��ȡ�����¶ȡ�����ֵ���һλΪС����
u16 mlxGetTaTemp(void);

// ��ȡ����Ŀ���¶ȡ�����ֵ���һλΪС����
u16 mlxGetToTemp(void);

#endif
