#ifndef __MLX90615_H
#define __MLX90615_H

#include "sys.h"

void mlx90615Init(void);

// 获取环境温度。返回值最后一位为小数。
u16 mlxGetTaTemp(void);

// 获取被测目标温度。返回值最后一位为小数。
u16 mlxGetToTemp(void);

#endif
