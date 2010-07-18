/*
 * TODO:
 * 	1. 要有 host <-> target register 對照
 *	2. 查詢的方向應該是傳入 target register, 查出 host register
 * NOTE:
 *	1. 似乎應該假設分配順序都是從頭開始 不會從中間跳著分配
 *	   否則 alloc_host 會出錯
 */

#include <stdio.h>
#include <string.h>

#define R0 0
#define R1 1
#define R2 2
#define R3 3

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3

#define NUM_REG 4
#define NUM_HOST_REG 4

unsigned int regmap[NUM_REG] = {0};	// 紀錄 Rn 是否已經分配了, 0 代表未分配 =\= 0 代表分配到的 host register
unsigned int reghost[NUM_HOST_REG] = {0};	// 紀錄 host register 是否被分配了, 只要不是 0 就代表紀錄了 target register
						// 這裡的順序是按照 priority, 由 regprio 可知順序
unsigned int regprio[NUM_HOST_REG] = {EAX, ECX, EDX, EBX};

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
static inline int test_and_set(int regn)
{
	int ret = regmap[regn];
	regmap[regn] = 1;
	return ret;
}

// 跟 test_and_set() 很像 只是不做 set, 用來 debug 用 (也就是可用來查表)
static inline int test_no_set(int regn)
{
	return regmap[regn];
}

// 傳回值是分配到的 host register
static inline int alloc_host(int target_reg)
{
	int i;
	for (i = target_reg; i < NUM_HOST_REG; ++i) {
		if (reghost[i] == 0) {	// 找到空的 host reg 可以分配
			reghost[i] = regprio[i];	// 絕妙的分配方法 要細想
			return regprio[i];
		}
	}

	return -1;	// 應該永遠不可能跑到這一步
}

int reg_alloc(int target_reg)
{
	int reg = test_and_set(target_reg);
	if (reg == 0) {	// 代表尚未分配
		int hostreg = alloc_host(target_reg);
		*tb.codebuf++ = (unsigned char)hostreg;
		env_to_reg(&tb, target_reg);
		reg_to_env(&tb, target_reg);
		return hostreg;
	} else {	// 代表已經分配過了 要做的事情有: 直接回傳之前分配過的 host reg 就好了
		*tb.codebuf++ = (unsigned char)reg;
		return reg;
	}

	return -1;	// 應該永遠不可能跑到這一步
}

void init_tb(void)
{
	memset((void*)&tb, 0, sizeof(tb));
	tb.prebuf = &tb._prebuf[0];
	tb.codebuf = &tb._codebuf[0];
	tb.postbuf = &tb._postbuf[0];
}

void show_tb(void)
{
	unsigned char *ptr = &tb._prebuf[0];
	do {
		printf("把 R%d 從 env 拿到 host R%d\n", *ptr, test_no_set(*ptr));
		++ptr;
	} while (*ptr != '\0');

	printf("\n");
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
