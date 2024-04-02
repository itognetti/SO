RM := rm -f
GCC := gcc
GCCFLAGS := -Wall -std=c99 -g -lrt -phtread
SOURCES_APP := slave.c lib.c
SOURCES_VIEW := lib.c 

all: app view

app: app.c
		$(GCC) app.c $(GCCFLAGS) $(SOURCES_APP) -o md5

view: view.c
		$(GCC) view.c $(GCCFLAGS) $(SOURCES_VIEW) -o view

clean:
		$(RM) md5
		$(RM) view
		$(RM) result.txt