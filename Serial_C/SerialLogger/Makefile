TARGET = run

SOURCES = mainSerialLogger_EFM32.c fileManagement.c serialConfig.c

CFLAGS = -Wall -g -O0 -lpthread

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
