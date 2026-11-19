#pragma once
#include "Types.h"

typedef struct{
	uint8_t NumberOfParameter;	// 有效参数个数
	uint64_t Par[6];			// 参数，用数组储存
} Parameter;

typedef uint64_t(*func0)();
typedef uint64_t(*func1)(uint64_t p1);
typedef uint64_t(*func2)(uint64_t p1, uint64_t p2);
typedef uint64_t(*func3)(uint64_t p1, uint64_t p2, uint64_t p3);
typedef uint64_t(*func4)(uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4);
typedef uint64_t(*func5)(uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5);
typedef uint64_t(*func6)(uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5, uint64_t p6);

uint64_t ShadowCall(uint8_t* func, Parameter* para);