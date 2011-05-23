#include <stdio.h>
#include <memory.h>

static void do_brainfuck(FILE *fp, FILE *in, FILE *out);

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

static int fgetprevc(FILE *fp) {
  const long cur = ftell(fp);
  if (2 > cur)
    return EOF;
  fseek(fp, cur - 2, SEEK_SET);
  const int c = fgetc(fp);
  printf ("get prev ch = %c, cur = %ld\n", c, cur);
  return c;
}

#include <unistd.h>
void do_brainfuck(FILE *fp, FILE *in, FILE *out) {
  long nest = 0;
  char mem[30000];
  char *p = mem;
  memset(mem, 0, sizeof(mem));


 LOOP:
  int i = fgetc(fp);
  if (EOF == i)
    return;
  printf("%c ip = %3ld, p = %08d, nest = %d\n", i, ftell(fp), p - mem, nest);
  sleep(1);
  switch (i) {
  case '>':
    if (p + 1 < mem + sizeof(mem))
      ++p;
    break;
  case '<':
    if (p > mem)
      --p;
    break;
  case '+': printf("+, val = %d at %08d\n", 1 + *p, p);
    ++ *p;
    break;
  case '-': printf("-, val = %d at %08d\n", *p - 1, p);
    -- *p;
    break;
  case '.':
    fputc(*p, out);
    break;
  case ',':
    *p = fgetc(in);
    break;
  case '[':
    ++nest;
    if (0 == *p) {
      const long curnest = nest;
      const long curip = ftell(fp);
      for (; !(']' == (i = fgetc(fp)) && curnest == nest); ) {
	if (EOF == i) {
	  printf ("syntax error, no matching ']' found. (%d)\n", curip);
	  return;
	}
	if ('[' == i)
	  nest++;
	if (']' == i)
	  --nest;
      }
    }
    break;
  case ']':
    if (0 != *p) {
      const long curnest = nest;
      const long curip = ftell(fp);
      for (; !('[' == (i = fgetprevc(fp)) && curnest == nest); ) {
	if (EOF == i) {
	  printf ("syntax error, no matching '[' found. (%d)\n", curip);
	  return;
	}
	if ('[' == i)
	  nest++;
	if (']' == i)
	  --nest;
      }
      printf ("] broken by %c\n", i);
    }
    --nest;
    break;
  }
  goto LOOP;
}
