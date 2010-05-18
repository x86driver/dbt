TARGET = dbt genop intermediate.h
CFLAGS = -g -Wall

all:$(TARGET)

dbt:dbt.c
	gcc $(CFLAGS) -o $@ $<

genop:genop.c
	gcc $(CFLAGS) -o $@ $<

intermediate.h:genop intermediate.op
	./genop

clean:
	rm -rf $(TARGET)

