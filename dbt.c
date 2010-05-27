#include <stdio.h>
#include "intermediate.h"

struct cpu_state {
	unsigned char ircode[512];
	unsigned char codegen[512];
};

void showa(void)
{
	printf("Hello A\n");
}

void dbt_gen_call0(cpu, func)
{
}

/* helper function prototype (no argument, no return)
 * void func(void);
 */

void gen_op_call0(struct cpu_state cpu, void *(func)(void))
{
	dbt_gen_call0(cpu, func);
}

void dbt_gen_ret()
{
}

void gen_exit_tb(struct cpu_state cpu)
{
	/* Must do this:
	 * dbt_gen_restore_reg();
	 */

	dbt_gen_ret();
}

void translate()
{
}

int main()
{
	return 0;
}

