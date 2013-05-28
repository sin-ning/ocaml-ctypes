#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


typedef int intfun(int, int);

static int add(int x, int y) { return x + y; }
static int times(int x, int y) { return x * y; }

int higher_order_1(intfun *callback, int x, int y)
{
  return callback(x, y) == x + y;
}

typedef int acceptor(intfun *, int, int);

int higher_order_3(acceptor *callback, intfun *fn, int x, int y)
{
  return callback(fn, x, y);
}

typedef int vintfun(int);

int higher_order_simplest(vintfun *callback)
{
  return callback(22);
}

intfun *returning_funptr(int v)
{
  switch (v)
  {
  case 0: return add;
  case 1: return times;
  default: return NULL;
  }
}

int accepting_possibly_null_funptr(intfun *f, int x, int y)
{
  return f != NULL ? f(x, y) : -1;
}

int global = 100;

int *return_global_address(void)
{
  return &global;
}

double float_pointer_callback(void (*f)(double *), double v)
{
  f(&v);
  return v * 2.0;
}

int write_through_callback(int (*f)(int *))
{
  int x = 42;
  return f(&x) + x;
}

int write_through_callback_pointer_pointer(int (*f)(int **, int *))
{
  int x = 10, y = 20;
  int *p =&x;
  fprintf(stderr, "[before] x = %d, y = %d, &x = %p, &y = %p, p = %p, &p = %p\n",
          x, y, &x, &y, p, &p);

  fprintf(stderr, "calling f(%p, %p)\n", &p, &y);
          
  fprintf(stderr, "[after] x = %d, y = %d, p = %p, *p = %d\n",
          x, y, p, *p);
  return f(&p, &y) + *p + x + y;
}

int is_null(void *p)
{
  return p == NULL;
}

int callback_returns_funptr(vintfun *(*callback)(int), int x)
{
  vintfun *v1 = callback(x);
  vintfun *v2 = callback(x + 1);

  return v1(10) + v2(20);
}


int *pass_pointer_through(int *a, int *b, int i)
{
  return (i >= 0) ? a : b;
}

struct simple {
  int i;
  double f;
  struct simple *self;
};

int accept_struct(struct simple simple)
{
  return simple.i + (int)simple.f + (simple.self == NULL ? 1 : 0);
}

struct simple return_struct(void)
{
  struct simple *t = malloc(sizeof *t);
  t->i = 10;
  t->f = 12.5;
  t->self = t;

  struct simple s = {
    20,
    35.0,
    t
  };

  return s;
}

union padded {
  int64_t i;
  char    a[sizeof(int64_t) + 1];
};

int64_t sum_union_components(union padded *padded, size_t len)
{
  size_t i;
  int64_t acc = 0;
  for (i = 0; i < len; i++) {
    acc += padded[i].i;
  }
  return acc;
}

void concat_strings(const char **sv, int sc, char *buffer)
{
  int i = 0;
  for (; i < sc; i++) {
    const char *s = sv[i];
    while (*s) {
      *buffer++ = *s++;
    }
  }
  *buffer = '\0';
}

union number {
  int i;
  double d;
};

struct tagged {
  char tag;
  union number num;
};

double accepts_pointer_to_array_of_structs(struct tagged(*arr)[5])
{
  double sum = 0.0;
  int i = 0;
  struct tagged *s = &(*arr[0]);
  for (; i < 5; i++) {
    switch (s[i].tag) {
    case 'i': {
      sum += s[i].num.i;
      break;
    }
    case 'd': {
      sum += s[i].num.d;
      break;
    }
    default: assert(0);
    }
  }
  return sum;
}

const char *global_string = "global string";


/* OO-style example */
struct animal_methods;
struct animal {
  struct animal_methods *vtable;
};
struct animal_methods {
  char *(*say)(struct animal *);
  char *(*identify)(struct animal *);
};

int check_name(struct animal *a, char *name)
{
  return strcmp(a->vtable->identify(a), name) == 0;
}

enum colour { white, red, black, pale };

struct chorse_methods;

struct chorse {
  struct chorse_methods *vtable;
  enum colour colour;
};

struct chorse_methods {
  struct animal_methods base;
  char *(*colour)(struct chorse *);
};

char *chorse_colour(struct chorse *chorse)
{
  switch (chorse->colour) {
  case white : return "white";
  case red   : return "red";
  case black : return "black";
  case pale  : return "pale";
  default: assert(0);
  }
}

char *chorse_say(struct animal *c)
{
  return "neigh";
}

char *chorse_identify(struct animal *a)
{
  static char buffer[30]; /* static allocation is adequate for the test */
  sprintf(buffer, "%s horse", chorse_colour((struct chorse *)a));
  return buffer;
}

static struct chorse_methods chorse_vtable = {
  {
    chorse_say,
    chorse_identify,
  },
  chorse_colour,
};

struct chorse *new_chorse(int colour)
{
  struct chorse *h = malloc(sizeof *h);
  h->vtable = &chorse_vtable;
  h->colour = (enum colour)colour;
  return h;
}
/* (End of OO-style example) */

int accept_pointers(float *float_p,
                    double *double_p,
                    short *short_p,
                    int *int_p,
                    long *long_p,
                    long long *llong_p,
                    int *nativeint_p,
                    int8_t *int8_t_p,
                    int16_t *int16_t_p,
                    int32_t *int32_t_p,
                    int64_t *int64_t_p,
                    uint8_t *uint8_t_p,
                    uint16_t *uint16_t_p,
                    uint32_t *uint32_t_p,
                    uint64_t *uint64_t_p,
                    size_t *size_t_p,
                    unsigned short *ushort_p,
                    unsigned *uint_p,
                    unsigned long *ulong_p,
                    unsigned long long *ullong_p)
{
  return (*float_p
          + *double_p
          + *short_p
          + *int_p
          + *long_p
          + *llong_p
          + *nativeint_p
          + *int8_t_p
          + *int16_t_p
          + *int32_t_p
          + *int64_t_p
          + *uint8_t_p
          + *uint16_t_p
          + *uint32_t_p
          + *uint64_t_p
          + *size_t_p
          + *ushort_p
          + *uint_p
          + *ulong_p
          + *ullong_p);
}

int accept_pointers_to_pointers(int *p, int **pp, int ***ppp, int ****pppp)
{
  return *p + **pp + ***ppp + ****pppp;
}

intfun **returning_pointer_to_function_pointer(void)
{
  static intfun *f = times;
  return &f;
}

int accepting_pointer_to_function_pointer(intfun **pfp)
{
  return (*pfp)(20, 4);
}
