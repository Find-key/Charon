#include "StackSpoof.h"


/**
 * @brief 通过覆盖当前函数的为零来阻止栈回溯
 *
 * @param[in]  func  要调用的函数地址
 * @param[in]  para  参数结构体
 *
 * @return uint64_t  成功返回调用函数的返回值，失败返回 0
 */
uint64_t ShadowCall(uint8_t* func, Parameter* para) {
	void* ReturnAddress = _ReturnAddress();
	*(void**)_AddressOfReturnAddress() = 0;
	uint64_t result = 0;
	switch (para->NumberOfParameter) {
		case 0:
			result = ((func0)func)();
			break;
		case 1:
			result = ((func1)func)(para->Par[0]);
			break;
		case 2:
			result = ((func2)func)(para->Par[0], para->Par[1]);
			break;
		case 3:
			result = ((func3)func)(para->Par[0], para->Par[1], para->Par[2]);
			break;
		case 4:
			result = ((func4)func)(para->Par[0], para->Par[1], para->Par[2], para->Par[3]);
			break;
		case 5:
			result = ((func5)func)(para->Par[0], para->Par[1], para->Par[2], para->Par[3], para->Par[4]);
			break;
		case 6:
			result = ((func6)func)(para->Par[0], para->Par[1], para->Par[2], para->Par[3], para->Par[4], para->Par[5]);
			break;
		default:
			break;
	}

	*(void**)_AddressOfReturnAddress() = ReturnAddress;
	return result;
}