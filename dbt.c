#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include "intermediate.h"

struct cpu_regs {
	uint32_t r[16];
};

struct cpu_state {
	struct cpu_regs regs;
	unsigned char irbuf[512];
	unsigned char codebuf[512];
	uint32_t op_param[1][512];
	unsigned char *ir;
	unsigned char *codegen;
	uint32_t *opparam32;
};

register struct cpu_state *cpu asm("r12");

void showa(void)
{
	printf("Hello A\n");
}

/* helper function prototype (no argument, no return)
 * void func(void);
 */

typedef void(*MYFUNC)(void);

void gen_op_call0(void(*func)(void))
{
	*(cpu->ir)++ = CALL0;
	*(void(**)(void))(cpu->opparam32)++ = func;
}

void dbt_ret()
{
	*cpu->codegen = 0xc3;
}

void gen_exit_tb()
{
	/* Must do this:
	 * dbt_gen_restore_reg();
	 */

	dbt_ret();
}

static inline uint8_t OFFSET(uint64_t reg)
{
	return (uint64_t)(&cpu->regs.r[reg])-(uint64_t)cpu;
}

void dbt_add_reg_reg(uint32_t opdst, uint32_t op1, uint32_t op2)
{
	/* mov edx, [op1] */
	*(uint32_t*)cpu->codegen = 0x24548b41;
	cpu->codegen += 4;
	*cpu->codegen++ = OFFSET(op1);

	*(uint32_t*)cpu->codegen = 0x24540341;
	cpu->codegen += 4;
	*cpu->codegen++ = OFFSET(op2);

	*(uint32_t*)cpu->codegen = 0x24548941;
	cpu->codegen += 4;
	*cpu->codegen++ = OFFSET(opdst);
}

void dbt_mov_reg_imm32(uint32_t opdst, uint32_t op1, uint32_t imm32)
{
	
}

void translate()
{
	uint32_t opdst, op1, op2;

	switch (*cpu->ir++) {
		case CALL0:
			//(void(*func)(void)) = *cpu->op_param[0]++;
			//dbt_gen_call0(func);
			break;
		case ADD:
			opdst = *cpu->opparam32++;
			op1 = *cpu->opparam32++;
			op2 = *cpu->opparam32++;
			dbt_add_reg_reg(opdst, op1, op2);
			break;
		case OPEOF:
			return;
	}
}

#define EAX 0x0b8
#define ECX 0x0b9
#define EDX 0x0ba
#define EBX 0x0bb
#define ESP 0x0bc
#define EBP 0x0bd
#define ESI 0x0be
#define EDI 0x0bf
#define R8  0x0b8
#define R9  0x0b9
#define R10 0x0ba
#define R11 0x0bb
#define R12 0x0bc
#define R13 0x0bd
#define R14 0x0be
#define R15 0x0bf

#define REGNUM 16

uint32_t alloc_reg()
{
	static unsigned int regmap = 0;
	uint32_t regtbl[REGNUM] = {EAX, ECX, EDX, ESI, EDI, R8, R9, R10,
			R11, EBX, EBP, ESP, R12, R13, R14, R15};

	for (i = 0; i < REGNUM; ++i) {
		y

	}
}

void free_reg()
{
}

void init_cpu()
{
//	cpu = malloc(sizeof(struct cpu_state));
	struct cpu_state *ptr = cpu;
	posix_memalign((void**)&ptr, 4096, sizeof(struct cpu_state));
	cpu = ptr;
	memset(cpu, 0, sizeof(struct cpu_state));

	cpu->ir = &cpu->irbuf[0];
	cpu->codegen = &cpu->codebuf[0];
	cpu->opparam32 = &cpu->op_param[0][0];
}


typedef void(*FUNC)(void);

int main()
{
	int ret;
	init_cpu();

	dbt_add_reg_reg(1, 2, 3);
	gen_exit_tb();

	FUNC func = (FUNC)cpu->codebuf;

	printf("%d\n", cpu->regs.r[1]);
	cpu->regs.r[2] = 7;
	cpu->regs.r[3] = 15;

	printf("cpu: %p\n", cpu);
	ret = mprotect(cpu, 0x1000, PROT_READ|PROT_WRITE|PROT_EXEC);
	if (ret != 0) {
		printf("ret: %d\n", ret);
		perror("mprotect");
		exit(1);
	}
	func();
	printf("%d\n", cpu->regs.r[1]);
	return 0;
}

