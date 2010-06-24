#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "intermediate.h"

struct cpu_state {
	unsigned char irbuf[512];
	unsigned char codebuf[512];
	uint32_t op_param[1][512];
	unsigned char *ir;
	unsigned char *codegen;
	uint32_t *opparam32;
};

void showa(void)
{
	printf("Hello A\n");
}

/* helper function prototype (no argument, no return)
 * void func(void);
 */

typedef void(*MYFUNC)(void);

void gen_op_call0(struct cpu_state *cpu, void (*func)(void))
{
	*(cpu->ir)++ = CALL0;
	*(void(**)(void))(cpu->opparam32)++ = func;
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

void dbt_gen_call0(void*(func)(void))
{
}

void translate(struct cpu_state *cpu)
{
	switch (*cpu->ir++) {
		case CALL0:
			//(void(*func)(void)) = *cpu->op_param[0]++;
			//dbt_gen_call0(func);
			break;
		case OPEOF:
			return;
	}
}

void init_cpu(struct cpu_state *cpu)
{
	memset(cpu->irbuf, 0, sizeof(cpu->irbuf));
	memset(cpu->irbuf, 0, sizeof(cpu->codebuf));
	memset(cpu->op_param[0], 0, sizeof(cpu->op_param[0]));
	cpu->ir = &cpu->irbuf[0];
	cpu->codegen = &cpu->codebuf[0];
	cpu->opparam32 = &cpu->op_param[0][0];
}

int main()
{
	struct cpu_state cpu;
	init_cpu(&cpu);
	return 0;
}

