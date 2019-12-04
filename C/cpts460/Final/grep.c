#include "ucode.c"

int matchstar();
int matchhere();

int matchstar(int c, char *regexp, char *text) {
  do {
    if (matchhere(regexp, text))
      return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}

int matchhere(char *regexp, char *text) {
  if (regexp[0] == '\0')
    return 1;
  if (regexp[1] == '*')
    return matchstar(regexp[0], regexp + 2, text);
  if (regexp[0] == '$' && regexp[1] == '\0')
    return *text == '\0';
  if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
    return matchhere(regexp + 1, text + 1);
  return 0;
}

int match(char *regexp, char *text) {
  if (regexp[0] == '^')
    return matchhere(regexp + 1, text);
  do {
    if (matchhere(regexp, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

int main(int argc, char *argv[]) {
  char *pattern;
  int fd;
  if (argc == 1) {
    printf("GREP: not enough arguments\n");
    exit(1);
  } else if (argc == 2) {
    pattern = argv[1];
    fd = 0;
  } else if (argc = 3) {
    pattern = argv[1];
    fd = open(argv[2], O_RDONLY);
  } else {
    printf("GREP: too many arguments\n");
    exit(1);
  }
  if (fd < 0) {
    printf("GREP: fail to open %s\n", argv[2]);
    exit(1);
  }
  char line[256];
  int matches = 0, n, lineno = 0;
  do {
    lineno++;
    n = readline(fd, line);
    if (match(pattern, line)) {
      printf("line %d: %s", lineno, line);
      matches++;
    }
  } while (*line && n);
  printf("%d matches found\n", matches);
  close(fd);
  exit(0);
}