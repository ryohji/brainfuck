#include <stdio.h>
#include <memory.h>

static void do_brainfuck(FILE *fp, FILE *in, FILE *out);
static int fgetprevc(FILE *fp);

int main(int argn, char **args) {
  if (argn < 2)
    return 0;
  FILE *fp = fopen(args[1], "rb");
  if (fp) {
    do_brainfuck(fp, stdin, stdout);
    fclose(fp);
  }
  return 0;
}

void do_brainfuck(FILE *fp, FILE *in, FILE *out) {
  char mem[30000];
  char *p = mem;
  memset(mem, 0, sizeof(mem));

 LOOP:
  int i = fgetc(fp);
  switch (i) {
  case EOF:
    return; // end brainfuck program normally
  case '>':
    if (p + 1 < mem + sizeof(mem))
      ++p;
    break;
  case '<':
    if (p > mem)
      --p;
    break;
  case '+':
    ++ *p;
    break;
  case '-':
    -- *p;
    break;
  case '.':
    fputc(*p, out);
    break;
  case ',':
    *p = fgetc(in);
    break;
  case '[':
    if (0 == *p) {
      long level = 0;
      const long curip = ftell(fp) - 1;
      for (; !(']' == (i = fgetc(fp)) && 0 == level); ) {
	switch (i) {
	case EOF:
	  fprintf (stderr, "syntax error, no matching ']' found. (%ld)\n", curip);
	  return;
	case '[':
	  ++level;
	  break;
	case ']':
	  --level;
	  break;
	}
      }
    }
    break;
  case ']':
    if (0 != *p) {
      long level = 0;
      const long curip = ftell(fp) - 1;
      for (; !('[' == (i = fgetprevc(fp)) && 0 == level); ) {
	switch (i) {
	case EOF:
	  fprintf (stderr, "syntax error, no matching '[' found. (%ld)\n", curip);
	  return;
	case '[':
	  --level;
	  break;
	case ']':
	  ++level;
	  break;
	}
      }
    }
    break;
  }
  goto LOOP;
}

int fgetprevc(FILE *fp) {
  const long cur = ftell(fp);
  if (2 > cur) {
    return EOF;
  } else {
    fseek(fp, cur - 2, SEEK_SET);
    return fgetc(fp);
  }
}
