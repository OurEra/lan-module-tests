#include <stdio.h>
#include <setjmp.h>
#include "TestJMP.h"

/**
 * (A1)
 * (B1)
 * (A2) r=10001
 * (B2) r=20001
 * (A3) r=10002
 * (B3) r=20002
 * (A4) r=10003
 */

jmp_buf bufferA, bufferB;

void routineB(); // forward declaration

void routineA() {
    int r ;

    printf("(A1)\n");

    r = setjmp(bufferA);
    if (r == 0) routineB();

    printf("(A2) r=%d\n",r);

    r = setjmp(bufferA);
    if (r == 0) longjmp(bufferB, 20001);

    printf("(A3) r=%d\n",r);

    r = setjmp(bufferA);
    if (r == 0) longjmp(bufferB, 20002);

    printf("(A4) r=%d\n",r);
}

void routineB() {
    int r;

    printf("(B1)\n");

    r = setjmp(bufferB);
    if (r == 0) longjmp(bufferA, 10001);

    printf("(B2) r=%d\n", r);

    r = setjmp(bufferB);
    if (r == 0) longjmp(bufferA, 10002);

    printf("(B3) r=%d\n", r);

    r = setjmp(bufferB);
    if (r == 0) longjmp(bufferA, 10003);
}

void TestJMP::setUp() {
    printf("TestJMP::setUp \n");
}

void TestJMP::run() {

	routineA();
}

void TestJMP::tearDown() {
    printf("TestJMP::tearDown \n\n");
}
