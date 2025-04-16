# Nome do programa (Minix service)
PROG=lab5

# Ficheiros de código-fonte a compilar
SRCS = lab5.c video.c

# Flags adicionais de compilação
CFLAGS += -pedantic
CPPFLAGS += -DLAB5

# Dependências de bibliotecas
DPADD += ${LIBLCF}
LDADD += -llcf

# Inclui o makefile do LCOM
.include <minix.lcom.mk>
