#include <stdio.h>
#include <string.h>

#define R0 0
#define R1 1
#define R2 2
#define R3 3

#define NUM_REG 4

unsigned int regmap[NUM_REG] = {0};

struct TB {
	unsigned char _prebuf[512];
	unsigned char _codebuf[512];
	unsigned char _postbuf[512];
	unsigned char *prebuf, *codebuf, *postbuf;
} tb;

// 代表從 env 拿值到 reg, 通常用在 tb 的 prepare buffer
static inline void env_to_reg(struct TB *tb, int regn)
{
	*tb->prebuf++ = (unsigned char)regn;
}

// 從 reg 把值放回 env, 通常用在 tb 結束時的 post buffer
static inline void reg_to_env(struct TB *tb, int regn)
{
	*tb->postbuf++ = (unsigned char)regn;
}

// ret: 1 代表之前分配過了
int test_and_set(int regn)
{
	int ret = regmap[regn];
	regmap[regn] = 1;
	return ret;
}

int reg_alloc(int target_reg)
{
	if (test_and_set(target_reg) == 1) {
	} else {
		env_to_reg(&tb, target_reg);
		reg_to_env(&tb, target_reg);
	}
}

void init_tb(void)
{
	memset((void*)&tb, 0, sizeof(tb));
	tb.prebuf = &tb._prebuf;
	tb.codebuf = &tb._codebuf;
	tb.postbuf = &tb._postbuf;
}

void show_tb(void)
{
	unsigned char *ptr = &tb._prebuf[0];
	do {
		printf("把 R%d 從 env 拿到 host register\n", ptr++);
	} while (*ptr != NULL);
}

int main()
{
	init_tb();

	//預計的介面
	reg_alloc(R0);
	reg_alloc(R1);

	show_tb();
	return 0;
}
