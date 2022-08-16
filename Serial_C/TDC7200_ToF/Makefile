TARGET = run

SOURCES = main_ToF.c hex2dec.c TDC7200_ToF.c

CFLAGS = -Wall -g -O0

DEPS = $(SOURCES:.c=.d)
OBJS = $(SOURCES:.c=.o)


${TARGET}: ${OBJS} 
	${CC} ${CFLAGS} ${OBJS} -o "$@"
#	${CC} ${CFLAGS} ${OBJS} -o "$@"

%.d : %.c
	${CC} -MM "$<" > "$@"

include ${DEPS}

clean:
	@echo cleaning project ${TARGET}
	@rm -f ${OBJS} *.d ${TARGET}

