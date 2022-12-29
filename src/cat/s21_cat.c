#include "s21_cat.h"

int main(const int argc, char **const argv) {
  getFlags(argc, argv);
  return 0;
}

void getFlags(const int argc, char **const argv) {
  struct FlagsStructure flags;
  flags.b_flag = 0;
  flags.e_flag = 0;
  flags.E_flag = 0;
  flags.n_flag = 0;
  flags.s_flag = 0;
  flags.t_flag = 0;
  flags.T_flag = 0;
  flags.v_flag = 0;
  flags.err_flag = 0;

  int isFile = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-' && isFile == 0) {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        switch (argv[i][j]) {
          case 'b':
            flags.b_flag = 1;
            break;
          case 'e':
            flags.e_flag = 1;
            break;
          case 'n':
            flags.n_flag = 1;
            break;
          case 's':
            flags.s_flag = 1;
            break;
          case 't':
            flags.t_flag = 1;
            break;
          case 'v':
            flags.v_flag = 1;
            break;
          case 'E':
            flags.E_flag = 1;
            break;
          case 'T':
            flags.T_flag = 1;
            break;
          default:
            flags.err_flag = 1;
            break;
        }
      }
    } else if (argv[i][0] == '-' && argv[i][1] == '-' && isFile == 0) {
      if (strcmp(argv[i], "--number-nonblank"))
        flags.b_flag = 1;
      else if (strcmp(argv[i], "--number"))
        flags.n_flag = 1;
      else if (strcmp(argv[i], "--squeeze-blank"))
        flags.s_flag = 1;
      else
        flags.err_flag = 1;
    } else {
      isFile++;
      FILE *readFile;
      // printf("%s", argv[i]);
      output(flags, argv[i], &readFile);
    }
  }
}

void output(const struct FlagsStructure flags, char *fileName, FILE **file) {
  *file = fopen(fileName, "r");
  int ch;
  int prev;
  int blank_lines = 0;
  char *symbols[33] = {"^@", "^A", "^B", "^C", "^D",  "^E", "^F", "^G",
                       "^H", "^I", "$",  "^K", "^L",  "^M", "^N", "^O",
                       "^P", "^Q", "^R", "^S", "^T",  "^U", "^V", "^W",
                       "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"};
  int is_first_ch = 1;

  if (!flags.err_flag) {
    if (*file != NULL) {
      int counter = 1;
      while (!feof(*file) && !ferror(*file)) {
        ch = getc(*file);
        if (ch != EOF) {
          if ((ch == '\n' && prev == '\n') ||
              (is_first_ch == 1 && ch == '\n')) {
            blank_lines++;
          } else {
            blank_lines = 0;
          }
          if (blank_lines < 2 || !flags.s_flag) {
            if ((ch != '\n' && flags.b_flag) || (is_first_ch && flags.n_flag)) {
              if (prev == '\n') {
                printf("%6d\t", counter);
                counter++;
              } else if (is_first_ch && (flags.n_flag || flags.b_flag)) {
                if (ch == '\n' && (flags.e_flag || flags.E_flag)) {
                  if (!flags.b_flag) {
                    printf("%6d\t", counter);
                    counter++;
                  }
                } else if (is_first_ch && (flags.n_flag || flags.b_flag)) {
                  if (flags.b_flag && ch == '\n') {
                  } else {
                    printf("%6d\t", counter);
                    counter++;
                  }
                }
              }
            }
            if (ch == '\n' && (flags.E_flag || flags.e_flag)) {
              printf("$\n");
            } else if (ch == '\t' && (flags.T_flag || flags.t_flag)) {
              printf("^I");
            } else {
              if (((ch >= 0 && ch <= 31 && ch != 9 && ch != 10) || ch == 127) &&
                  (flags.v_flag || flags.e_flag || flags.t_flag))
                if (ch == 127)
                  printf("^?");
                else
                  printf("%s", symbols[ch]);
              else {
                putchar(ch);
              }
            }

            if (ch == '\n' && flags.n_flag && !flags.b_flag &&
                (!flags.E_flag || !flags.e_flag) && !flags.s_flag) {
              ch = getc(*file);
              if (ch == EOF) {
              } else {
                fseek(*file, -1, SEEK_CUR);
                printf("%6d\t", counter);
                counter++;
              }
            }

            if (ch == '\n' && flags.n_flag && !flags.b_flag &&
                (!flags.E_flag || !flags.e_flag) && flags.s_flag &&
                blank_lines < 2) {
              // ch = getc(*file);
              // if (ch == EOF) {
              // } else {
              //   fseek(*file, -1, SEEK_CUR);
              //   printf("%6d\t", counter);
              //   counter++;
              // }
              int seek_count = 0;
              int is_end = 0;
              while (ch != EOF) {
                ch = getc(*file);
                if (ch != '\n' && ch != EOF) {
                  is_end = 1;
                }
                seek_count++;
              }
              if (!is_end) {
                printf("%6d\t", counter);
                if (flags.e_flag) printf("$\n");
              } else {
                fseek(*file, -seek_count, SEEK_CUR);
                ch = getc(*file);
                printf("%6d\t", counter);
                counter++;
              }
            }
          }
        }
        prev = ch;
        is_first_ch = 0;
      }
      fclose(*file);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", fileName);
    }

  } else {
    fprintf(stderr, "usage: cat [-benstv] [file ...]\n");
  }
}