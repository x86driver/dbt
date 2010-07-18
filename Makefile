TARGET = genop intermediate.h reg
CFLAGS = -g -Wall

all:$(TARGET)

dbt:dbt.c intermediate.h
	gcc $(CFLAGS) -o $@ $<

genop:genop.c
	gcc $(CFLAGS) -o $@ $<

intermediate.h:genop intermediate.op
	./genop

reg:reg.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)

