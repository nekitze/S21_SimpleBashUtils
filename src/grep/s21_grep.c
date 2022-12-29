#include "s21_grep.h"

int main(const int argc, char **const argv) {
  if (argc < 3)
    show_error(4, 0, 0);
  else
    get_parameters(argc, argv);
  return 0;
}
// test
void get_parameters(const int argc, char **const argv) {
  struct struct_parameters parameters;
  parameters.err_flag = 0;
  parameters.e_flag = 0;
  parameters.i_flag = 0;
  parameters.v_flag = 0;
  parameters.c_flag = 0;
  parameters.l_flag = 0;
  parameters.n_flag = 0;
  parameters.h_flag = 0;
  parameters.s_flag = 0;
  parameters.f_flag = 0;  // -f file
  parameters.o_flag = 0;
  parameters.files = 0;
  parameters.pattern[0] = '\0';
  parameters.e_count = 0;
  parameters.reg_files = 0;

  char prev_flag = 'x';

  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-' && argv[i][1] != '-') || argv[i][0] != '-' ||
        ((prev_flag == 'e' || prev_flag == 'f') && argv[i][0] == '-')) {
      if (argv[i][0] == '-' && argv[i][1] != '\0' && prev_flag != 'e' &&
          prev_flag != 'f') {
        if (argv[i][1] == 'e' && argv[i][2] != '\0') {
          if (!parameters.e_count == 0) strcat(parameters.pattern, "|");
          for (int k = 2; k < (int)strlen(argv[i]); k++) {
            char c_to_str[2];
            c_to_str[1] = '\0';
            c_to_str[0] = argv[i][k];
            strcat(parameters.pattern, c_to_str);
          }
          parameters.e_count++;
        } else if (argv[i][1] == 'f' && argv[i][2] != '\0') {
          char reg_filename[1024];
          reg_filename[0] = '\0';
          for (int k = 2; k < (int)strlen(argv[i]); k++) {
            char c_to_str[2];
            c_to_str[1] = '\0';
            c_to_str[0] = argv[i][k];
            strcat(reg_filename, c_to_str);
          }
          parameters.reg_files_list[parameters.reg_files] = reg_filename;
          parameters.reg_files++;
        } else {
          for (int j = 1; j < (int)strlen(argv[i]); j++) {
            switch (argv[i][j]) {
              case 'e':
                parameters.e_flag = 1;
                prev_flag = 'e';
                break;
              case 'i':
                parameters.i_flag = 1;
                break;
              case 'v':
                parameters.v_flag = 1;
                break;
              case 'c':
                parameters.c_flag = 1;
                break;
              case 'l':
                parameters.l_flag = 1;
                break;
              case 'n':
                parameters.n_flag = 1;
                break;
              case 'h':
                parameters.h_flag = 1;
                break;
              case 's':
                parameters.s_flag = 1;
                break;
              case 'f':
                parameters.f_flag = 1;
                prev_flag = 'f';
                break;
              case 'o':
                parameters.o_flag = 1;
                break;
              default:
                parameters.err_flag = 1;
                break;
            }
          }
        }
      } else if (prev_flag == 'f') {
        parameters.reg_files_list[parameters.reg_files] = argv[i];
        parameters.reg_files++;
        prev_flag = 'x';
      } else if (prev_flag == 'e' && parameters.e_count > 0) {
        parameters.e_count++;
        strcat(parameters.pattern, "|");
        strcat(parameters.pattern, argv[i]);
        prev_flag = 'x';
      } else if (prev_flag == 'e' && parameters.e_count == 0) {
        strcat(parameters.pattern, argv[i]);
        parameters.e_count++;
        prev_flag = 'x';
      } else {
        parameters.files_list[parameters.files] = argv[i];
        parameters.files++;
      }
    } else {
      show_error(4, 0, parameters.s_flag);
      exit(1);
    }
  }
  if (!parameters.reg_files) {
    read_files(parameters, 1);
  } else {
    read_files(parameters, 2);
  }
}

void read_files(struct struct_parameters parameters, int mode) {
  if (mode == 1) {
    int i = 0;
    if (parameters.e_count == 0) {
      strcat(parameters.pattern, parameters.files_list[0]);
      i = 1;
    }
    for (; i < parameters.files; i++) {
      FILE *file;
      file = fopen(parameters.files_list[i], "r");
      if (file != NULL) {
        if (!parameters.err_flag) {
          file_processing(parameters, &file, parameters.files_list[i]);
        } else {
          show_error(5, parameters.files_list[i], parameters.s_flag);
        }
        fclose(file);
      } else {
        show_error(1, parameters.files_list[i], parameters.s_flag);
      }
    }
  }

  if (mode == 2) {
    char string[1024];
    for (int i = 0; i < parameters.reg_files; i++) {
      FILE *reg_file;
      reg_file = fopen(parameters.reg_files_list[i], "r");
      if (reg_file != NULL) {
        if (!parameters.err_flag) {
          while (!feof(reg_file) && !ferror(reg_file)) {
            fgets(string, 1024, reg_file);
            string[strcspn(string, "\n")] = 0;
            if (!parameters.e_count) {
              strcat(parameters.pattern, string);
              parameters.e_count++;
            } else {
              strcat(parameters.pattern, "|");
              strcat(parameters.pattern, string);
              parameters.e_count++;
            }
          }
        } else {
          show_error(5, parameters.files_list[i], parameters.s_flag);
        }
        fclose(reg_file);
      } else {
        show_error(1, parameters.reg_files_list[i], parameters.s_flag);
      }
    }
    read_files(parameters, 1);
  }
}

void file_processing(const struct struct_parameters parameters, FILE **file,
                     char filename[1024]) {
  regex_t regex;
  int cflags = REG_EXTENDED;
  int reg_ans;
  char string[1024];
  int match_count = 0;
  int line_num = 0;
  regmatch_t pmatch[2];
  size_t nmatch = 2;

  if (parameters.i_flag) cflags += REG_ICASE;
  if (!parameters.o_flag) cflags += REG_NOSUB;
  if (!regcomp(&regex, parameters.pattern, cflags)) {
    while (!feof(*file) && !ferror(*file)) {
      fgets(string, 1024, *file);
      line_num++;
      reg_ans = regexec(&regex, string, nmatch, pmatch, 0);
      if (!reg_ans && !parameters.v_flag) {
        int breaker = 0;
        if (string[0] != '\n') match_count++;
        while (!reg_ans && !parameters.v_flag && !breaker) {
          if (!parameters.l_flag) {
            if (!parameters.c_flag) {
              if (parameters.files > 1 && !parameters.h_flag &&
                  !(parameters.e_count == 0 && parameters.files == 2)) {
                printf("%s:", filename);
                if (parameters.n_flag) printf("%d:", line_num);
                if (!parameters.o_flag) printf("%s", string);
                if (parameters.o_flag && !parameters.v_flag)
                  printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                         &string[pmatch[0].rm_so]);
                if (string[strlen(string) - 1] != '\n') printf("\n");
              } else {
                if (parameters.n_flag) printf("%d:", line_num);
                if (!parameters.o_flag) printf("%s", string);
                if (parameters.o_flag && !parameters.v_flag) {
                  printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                         &string[pmatch[0].rm_so]);
                }

                if (string[strlen(string) - 1] != '\n' && !parameters.o_flag &&
                    string[0] != '\0')
                  printf("\n");
              }
            }
          }
          if (parameters.o_flag)
            reg_ans = regexec(
                &regex,
                memmove(string, string + (pmatch[0].rm_eo), strlen(string)),
                nmatch, pmatch, 0);
          else
            breaker = 1;
        }
      } else if (reg_ans == REG_NOMATCH && parameters.v_flag &&
                 !parameters.o_flag && string[0] != '\0') {
        match_count++;
        if (!parameters.l_flag) {
          if (!parameters.c_flag) {
            if (parameters.files > 1 && !parameters.h_flag &&
                !(parameters.e_count == 0 && parameters.files == 2)) {
              printf("%s:", filename);
              if (parameters.n_flag) printf("%d:", line_num);
              if (!parameters.o_flag) printf("%s", string);
              if (string[strlen(string) - 1] != '\n') printf("\n");
            } else {
              if (parameters.n_flag) printf("%d:", line_num);
              if (!parameters.o_flag) printf("%s", string);
              if (string[strlen(string) - 1] != '\n') printf("\n");
            }
          }
        }
      } else if ((reg_ans == REG_NOMATCH && !parameters.v_flag) ||
                 (reg_ans == REG_NOMATCH && parameters.v_flag &&
                  parameters.o_flag)) {
        if (parameters.v_flag) match_count++;
      } else if (!reg_ans && parameters.v_flag) {
      } else {
        // show_error(2, 0, parameters.s_flag);
      }
      string[0] = '\0';
    }
    if (parameters.c_flag && !parameters.l_flag && parameters.files > 1 &&
        !parameters.h_flag && parameters.e_flag)
      printf("%s:%d\n", filename, match_count);
    if (parameters.c_flag && !parameters.l_flag && parameters.files == 2 &&
        !parameters.h_flag && !parameters.e_flag)
      printf("%d\n", match_count);
    if (parameters.c_flag && !parameters.l_flag && parameters.files > 2 &&
        !parameters.h_flag && !parameters.e_flag)
      printf("%s:%d\n", filename, match_count);
    if (parameters.c_flag && !parameters.l_flag && parameters.files > 1 &&
        parameters.h_flag)
      printf("%d\n", match_count);
    if (parameters.c_flag && !parameters.l_flag && parameters.files == 1)
      printf("%d\n", match_count);
    if (parameters.l_flag && match_count > 0) printf("%s\n", filename);
  } else {
    // show_error(3, 0, parameters.s_flag);
  }
  regfree(&regex);
}

void show_error(int err, char text[1024], int s_flag) {
  switch (err) {
    case 1:
      if (!s_flag)
        fprintf(stderr, "grep: %s: No such file or directory\n", text);
      break;
    case 2:
      if (!s_flag) fprintf(stderr, "Regex match failed\n");
      break;
    case 3:
      if (!s_flag) fprintf(stderr, "Could not compile regex\n");
      break;
    case 4:
      fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
      break;
    case 5:
      fprintf(stderr, "Wrong flags\n");
      break;
  }
}