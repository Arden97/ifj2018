#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char *print_buf;

int bprintf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int r = vsprintf(print_buf + strlen(print_buf), format, ap);
  va_end(ap);
  return r;
}

/*
 * Test parser.
 */

static void _test_scanner(const char *source_path, const char *out_path) {
  // init parser here

  char *source = file_read(source_path);
  assert(source != NULL);
  char *expected = file_read(out_path);
  assert(expected != NULL);

  // run scanner
  char buf[1024] = {0};
  print_buf = buf;
  // ifj_set_prettyprint_func(bprintf);
  // ifj_prettyprint(scanner output);

  assert(strcmp(expected, print_buf) == 0);
}

static void test_assign() {
  _test_scanner("test/scanner/assign.rb", "test/scanner/assign.out");
}

/*
 * Test the given `fn`.
 */

#define test(fn)                                                               \
  printf("    \e[92m✓ \e[90m%s\e[0m\n", #fn);                                  \
  test_##fn();

/*
 * Test suite title.
 */

#define suite(title) printf("\n  \e[36m%s\e[0m\n", title)

/*
 * Report sizeof.
 */

#define size(type)                                                             \
  printf("\n  \e[90m%s: %ld bytes\e[0m\n", #type, sizeof(type));

/*
 * Run all test suites.
 */

int main(int argc, const char **argv) {
  clock_t start = clock();

  // size(some ifj18 obj);

  suite("parser");
  // test(assign);

  printf("\n");
  printf("  \e[90mcompleted in \e[32m%.5fs\e[0m\n",
         (float)(clock() - start) / CLOCKS_PER_SEC);
  printf("\n");
  return 0;
}
