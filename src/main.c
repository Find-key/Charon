#include <windows.h>
#include <stdio.h>

int main() {
    // 验证能否直接读取 x64 的 PEB 地址 (GS 寄存器 0x60 偏移)
    PVOID pPeb = (PVOID)__readgsqword(0x60);
    printf("[+] Successfully started! PEB address: 0x%p\n", pPeb);

    system("pause");
    return 0;
}