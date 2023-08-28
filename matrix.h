#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct matrix {
  struct matrix *right;
  struct matrix *below;
  int line;
  int column;
  float info;
};
typedef struct matrix Matrix;

Matrix *makeHeads(int lines, int columns);
void insert(Matrix *matrix, int lines, int columns, int currentLine,
            int currentColumn, float value);
int count_lines(Matrix *matrix);
int count_columns(Matrix *matrix);

Matrix *big_matrix_create(unsigned int m);
Matrix *standard_matrix_multiply(Matrix *factorA, Matrix *factorB);

Matrix *matrix_create();
void matrix_destroy(Matrix *m);
void matrix_print(Matrix *m);
Matrix *matrix_add(Matrix *m, Matrix *n);
Matrix *matrix_multiply(Matrix *m, Matrix *n);
Matrix *matrix_transpose(Matrix *m);
float matrix_getelem(Matrix *m, int x, int y);
void matrix_setelem(Matrix *m, int x, int y, float elem);
