//==================================================
// MYSEH - Matt Pietrek 1997
// Microsoft Systems Journal, January 1997
// FILE: MYSEH.CPP
// To compile: CL MYSEH.CPP
//==================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

DWORD  scratch;

/*
 * _cdecl 是C Declaration的缩写（declaration，声明），
 * 表示C语言默认的函数调用方法：
 * 所有参数从右到左依次入栈，这些参数由调用者清除，称为手动清栈。
 * 被调用函数不会要求调用者传递多少参数，调用者传递过多或者过少的参数，
 * 甚至完全不同的参数都不会产生编译阶段的错误
 */

EXCEPTION_DISPOSITION
__cdecl
_except_handler(
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext )
{
    unsigned i;

    // Indicate that we made it to our exception handler
    printf( "Hello from an exception handler\n" );

    // Change EAX in the context record so that it points to someplace
    // where we can successfully write
    ContextRecord->Eax = (DWORD)&scratch;
    /* ContextRecord是_CONTEXT数据结构，包含了所有的寄存器信息
     *
     * typedef struct _CONTEXT
     * {
     *     DWORD ContextFlags;
     *     DWORD   Dr0;
     *     DWORD   Dr1;
     *     DWORD   Dr2;
     *     DWORD   Dr3;
     *     DWORD   Dr6;
     *     DWORD   Dr7;
     *     FLOATING_SAVE_AREA FloatSave;
     *     DWORD   SegGs;
     *     DWORD   SegFs;
     *     DWORD   SegEs;
     *     DWORD   SegDs;
     *     DWORD   Edi;
     *     DWORD   Esi;
     *     DWORD   Ebx;
     *     DWORD   Edx;
     *     DWORD   Ecx;
     *     DWORD   Eax;
     *     DWORD   Ebp;
     *     DWORD   Eip;
     *     DWORD   SegCs;
     *     DWORD   EFlags;
     *     DWORD   Esp;
     *     DWORD   SegSs;
     * } CONTEXT;
     */

    // Tell the OS to restart the faulting instruction
    return ExceptionContinueExecution;
    // ExceptionContinueExecution;表示同意处理该异常
    // ExceptionContinueSearch;则表示不能处理这个异常，继续往前搜索可处理该异常的函数
}

int main()
{
    DWORD handler = (DWORD)_except_handler;
    //将 handler 直接指向  _except_handler

    __asm
    {                           // Build EXCEPTION_REGISTRATION record:
        push    handler         // Address of handler function
        push    FS:[0]          // Address of previous handler
        /* 在这两个命令之后，低位32位指向原FS:[0]，高位32位指向现在的handler
         * 这是在模拟_EXCEPTION_REGISTRATION的结构
         * _EXCEPTION_REGISTRATION结构如下：
         *
         * _EXCEPTION_REGISTRATION struct
         *   prev    dd      ?
         *   handler dd      ?
         * _EXCEPTION_REGISTRATION ends
         */


        mov     FS:[0],ESP      // Install new EXECEPTION_REGISTRATION
        /* 这一句之后，将原来的指向_EXCEPTION_REGISTRATION的FS:[0]指针
         * 变更为现在的ESP，也就是用栈来模拟真正的_EXCEPTION_REGISTRATION
         * 至此，FS[0] -> _EXCEPTION_REGISTRATION.handler -> _except_handler()
         * 的结构已经顺利实现了。
         *
         * 事实上，对于编译器而言，创建_EXCEPTION_REGISTRATION结构也是在栈上进行的，
         * 栈天然地保证了|prePointertoOlderExceptionRegistration|handler|的结构顺序。
         */
    }

    __asm
    {
        mov     eax,0           // Zero out EAX
        mov     [eax], 1        // Write to EAX to deliberately cause a fault
        /*人为制造一个错误*/
    }

    printf( "After writing!\n" );

    __asm
    {                           // Remove our EXECEPTION_REGISTRATION record
        mov     eax,[ESP]       // Get pointer to previous record
        mov     FS:[0], EAX     // Install previous record
        /*
         * 这一句归还真正的_EXCEPTION_REGISTRATION给FS[0]
         */

        add     esp, 8          // Clean our EXECEPTION_REGISTRATION off stack
    }

    return 0;
}

