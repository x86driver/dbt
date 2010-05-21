#include <stdio.h>
#include "intermediate.h"

struct cpu_state {
	unsigned char codegen[512];
};

void showa(void)
{
	printf("Hello A\n");
}

/* helper function prototype (no argument, no return)
 * void func(void);
 */

void gen_op_call0(struct cpu_state cpu, void *(func)(void))
{
}

void translate()
{
}

int main()
{
	return 0;
}

