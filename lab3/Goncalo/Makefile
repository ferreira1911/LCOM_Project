# Nome do programa (Minix service)
PROG=lab3

# Ficheiros de código-fonte a compilar
SRCS = lab3.c keyboard.c

# Flags adicionais de compilação
CFLAGS += -pedantic
CPPFLAGS += -DLAB3

# Dependências de bibliotecas
DPADD += ${LIBLCF}
LDADD += -llcf

# Inclui o makefile do LCOM
.include <minix.lcom.mk>
