CC=gcc
CFLAGS=-g -O2 -Wall -I/usr/local/include -I/usr/include/postgresql
LDFLAGS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu -lh2o -luv -lpq -lwslay -lbrotlidec -lbrotlienc -lbrotlicommon

TARGET=app
SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c $(SRCDIR)/controller/*.c $(SRCDIR)/service/*.c $(SRCDIR)/model/*.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
