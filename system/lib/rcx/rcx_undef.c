
/*
    Name:   mDelay
    Input:  int mtime, number of milisecond to delay
    Output:     -
    Calls:  -
    Descr:  This routine takes mtime milisecond to execute.
                Note: It is NOT any kind of system call
*/
void mDelay(int mtime)
{
    int i,j;

    //1 sek = 5*350000

    for(i=0;i<mtime;i++)
        for(j=0;j<672;j++);
}

__asm__(
    ".section .text\n\t"
    ".global ___rcall0\n"
    "___rcall0:\n\t"
    "push r6\n\t"
    "jsr @r0\n\t"
    "mov.w r6,r0\n\t"
    "pop r6\n\t"
    "rts"
);

void reset(void)
{
    *(char *)0xFFCC = 1;
    (*(void (**)(void))0x0000)();
}

#if 0
short
__rcall0 (short a)
{
    __asm__ (
    "jsr @%0\n\t"
    "mov.w r6,%0"
    : "=r" (a)
    : "0" (a)
    : "r6", "cc", "memory"
    );
    return a;
}
#endif

__asm__ (
    ".section .text\n\t"
    ".global ___rcall1\n"
    "___rcall1:\n\t"
    "push r6\n\t"
    "mov.w r1,r6\n\t"
    "jsr @r0\n\t"
    "mov.w r6,r0\n\t"
    "pop r6\n\t"
    "rts"
);

#if 0
short
__rcall1 (short a, short p0)
{
    __asm__ (
    "mov.w %2,r6\n\t"
    "jsr @%0\n\t"
    "mov.w r6,%0"
    : "=r" (a)
    : "0" (a), "g" (p0)
    : "r6", "cc", "memory"
    );
    return a;
}
#endif

__asm__ (
    ".section .text\n\t"
    ".global ___rcall2\n"
    "___rcall2:\n\t"
    "push r6\n\t"
    "push r2\n\t"
    "mov.w r1,r6\n\t"
    "jsr @r0\n\t"
    "mov.w r6,r0\n\t"
    "adds #0x2,r7\n\t"
    "pop r6\n\t"
    "rts"
);

#if 0
short
__rcall2 (short a, short p0, short p1)
{
    __asm__ (
    "push %3\n\t"
    "mov.w %2,r6\n\t"
    "jsr @%0\n\t"
    "mov.w r6,%0\n\t"
    "adds #0x2,r7"
    : "=r" (a)
    : "0" (a), "g" (p0), "r" (p1)
    : "r6", "r7", "cc", "memory"
    );
    return a;
}
#endif

__asm__ (
    ".section .text\n\t"
    ".global ___rcall3\n"
    "___rcall3:\n\t"
    "push r6\n\t"
    "mov.w @(0x4,r7),r3\n\t"
    "push r3\n\t"
    "push r2\n\t"
    "mov.w r1,r6\n\t"
    "jsr @r0\n\t"
    "mov.w r6,r0\n\t"
    "adds #0x2,r7\n\t"
    "adds #0x2,r7\n\t"
    "pop r6\n\t"
    "rts"
);

#if 0
short
__rcall3 (short a, short p0, short p1, short p2)
{
    __asm__ (
    "push %4\n\t"
    "push %3\n\t"
    "mov.w %2,r6\n\t"
    "jsr @%0\n\t"
    "mov.w r6,%0\n\t"
    "adds #0x2,r7\n\t"
    "adds #0x2,r7"
    : "=r" (a)
    : "0" (a), "g" (p0), "r" (p1), "r" (p2)
    : "r6", "r7", "cc", "memory"
    );
    return a;
}
#endif

__asm__ (
    ".section .text\n\t"
    ".global ___rcall4\n"
    "___rcall4:\n\t"
    "push r6\n\t"
    "mov.w @(0x6,r7),r3\n\t"
    "push r3\n\t"
    "mov.w @(0x6,r7),r3\n\t"
    "push r3\n\t"
    "push r2\n\t"
    "mov.w r1,r6\n\t"
    "jsr @r0\n\t"
    "mov.w r6,r0\n\t"
    "add.b #0x6,r7l\n\t"
    "addx #0x0,r7h\n\t"
    "pop r6\n\t"
    "rts"
);

#if 0
short
__rcall4 (short a, short p0, short p1, short p2, short p3)
{
    __asm__ (
    "push %5\n\t"
    "push %4\n\t"
    "push %3\n\t"
    "mov.w %2,r6\n\t"
    "jsr @%0\n\t"
    "mov.w r6,%0\n\t"
    "adds #0x2,r7\n\t"
    "adds #0x2,r7\n\t"
    "adds #0x2,r7"
    : "=r" (a)
    : "0" (a), "g" (p0), "r" (p1), "r" (p2), "r" (p3)
    : "r6", "r7", "cc", "memory"
    );
    return a;
}
#endif
