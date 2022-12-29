#ifndef SRC_S21_GREP_H_
#define SRC_S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct struct_parameters {
  int err_flag;
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;  // -f file
  int o_flag;
  int files;
  int e_count;
  int reg_files;
  char pattern[1024];
  char *files_list[1024];
  char *reg_files_list[1024];
};

void get_parameters(const int argc, char **const argv);
void read_files(const struct struct_parameters parameters, int mode);
void file_processing(const struct struct_parameters flags, FILE **file,
                     char filename[1024]);
void show_error(int err, char text[1024], int s_flag);

#endif  // SRC_S21_GREP_H_