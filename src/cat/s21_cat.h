#ifndef SRC_S21_CAT_H_
#define SRC_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FlagsStructure {
  int err_flag;
  int b_flag;
  int e_flag;
  int E_flag;
  int n_flag;
  int s_flag;
  int t_flag;
  int T_flag;
  int v_flag;
};

void output(const struct FlagsStructure flags, char *fileName, FILE **file);
void getFlags(const int argc, char **const argv);

#endif  // SRC_S21_CAT_H_