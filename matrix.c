#include "matrix.h"

Matrix *makeHeads(int lines, int columns) {
  Matrix *head = (Matrix *)malloc(sizeof(Matrix));

  head->column = -1;
  head->line = -1;
  head->info = -1;
  head->right = head;
  head->below = head;

  for (int j = lines; j > 0; j--) {
    Matrix *lineHead = (Matrix *)malloc(sizeof(Matrix));
    lineHead->info = j;
    lineHead->line = -1;
    lineHead->column = 0;
    lineHead->right = lineHead;

    lineHead->below = head->below;
    head->below = lineHead;
  }

  for (int j = columns; j > 0; j--) {
    Matrix *columnHead = (Matrix *)malloc(sizeof(Matrix));
    columnHead->info = j;
    columnHead->line = 0;
    columnHead->column = -1;
    columnHead->below = columnHead;

    columnHead->right = head->right;
    head->right = columnHead;
  }
  return head;
}

void insert(Matrix *matrix, int lines, int columns, int currentLine,
            int currentColumn, float value) {
  Matrix *head = (Matrix *)malloc(sizeof(Matrix));
  Matrix *aux = matrix;
  Matrix *previous = matrix;

  head->info = value;
  head->column = currentColumn;
  head->line = currentLine;

  for (int i = 0; i < currentColumn; i++) {
    previous = matrix;
    matrix = matrix->right;
  }

  for (int i = 0; i < lines; i++) {
    if (matrix->below == matrix) {
      matrix->below = head;
      head->below = matrix;
      break;
    }
    if (matrix->line > head->line) {
      head->below = matrix;
      previous->below = head;
      break;
    }
    if (matrix->below->column == -1) {
      previous = matrix;
      matrix = matrix->below;
      head->below = matrix;
      previous->below = head;
      break;
    }

    previous = matrix;
    matrix = matrix->below;
  }

  previous = NULL;
  matrix = aux;

  for (int i = 0; i < currentLine; i++) {
    previous = matrix;
    matrix = matrix->below;
  }

  for (int i = 0; i < columns; i++) {
    if (matrix->right == matrix) {
      matrix->right = head;
      head->right = matrix;
      break;
    }
    if (matrix->column > head->column) {
      previous->right = head;
      head->right = matrix;
      break;
    }
    if (matrix->right->line == -1) {
      previous = matrix;
      matrix = matrix->right;

      previous->right = head;
      head->right = matrix;
      break;
    }

    previous = matrix;
    matrix = matrix->right;
  }

  previous = NULL;
  aux = NULL;
  free(aux);
  free(previous);
}

Matrix *matrix_create() {
  int lines = 0;
  int columns = 0;

  scanf("%d %d", &lines, &columns);

  int line = -1, column = -1, scanCount = 0;
  float value = 0;
  Matrix *head;

  head = makeHeads(lines, columns);

  while (1) {
    scanf("%d", &line);
    if (line == 0)
      return head;
    scanf(" %d %f", &column, &value);

    if (lines < line || columns < column) {
      printf("\nO numero de linhas e colunas deve ser igual ao numero "
             "específicado!\n\n");
      exit(0);
    } else if (value != 0) {
      insert(head, lines, columns, line, column, value);
      scanCount++;

      if (scanCount == (lines * columns)) {
        return head;
      }
    } else {
      scanCount++;
      if (scanCount == (lines * columns)) {
        return head;
      }
    }
  }

  return head;
}

void matrix_print(Matrix *m) {
  if (m == NULL) {
    printf("Nao há nenhuma celula na matriz!\n");
    return;
  } else if (m->line != -1 && m->column != -1) {
    printf("Nao há nenhuma celula na matriz!\n");
    return;
  } else {
    int lines = 0, columns = 0;

    lines = count_lines(m);
    columns = count_columns(m);

    printf("%d %d <=tam\n", lines, columns);
    while (1) {
      m = m->below;
      while (1) {
        m = m->right;
        if (m->column != -1 && m->line != -1) {
          printf("%d, %d, %.2f \n", m->line, m->column, m->info);
        } else if (m->line == -1) {
          break;
        }
      }
      if (m->column == -1 && m->line == -1)
        return;
    }
  }
}

float matrix_getelem(Matrix *m, int x, int y) {
  int lines = 0, columns = 0;

  lines = count_lines(m);
  columns = count_columns(m);

  if (x <= lines && y <= columns) {
    for (int i = 1; i <= y; i++)
      m = m->right;

    for (int i = 0; i <= x; i++) {
      if (m->line == x && m->column == y) {
        return m->info;
      } else if (m->below->column == -1) {
        return 0;
      }
      m = m->below;
    }
  } else {
    printf("Indice invalido!");
    return 0;
  }

  return 0;
}

void matrix_setelem(Matrix *m, int x, int y, float elem) {
  Matrix *aux = m;
  int lines = 0, columns = 0;
  lines = count_lines(m);
  columns = count_columns(m);

  for (int i = 1; i <= y; i++) {
    m = m->right;
  }
  for (int i = 0; i <= x; i++) {
    if (m->line == x && m->column == y) {
      m->info = elem;
    } else if (m->below->column == -1) {
      insert(aux, lines, columns, x, y, elem);
    }

    m = m->below;
  }
}

void matrix_destroy(Matrix *m) {
  if (m == NULL)
    return;
  int lineCount = count_lines(m), cont = 0;

  Matrix *head;
  Matrix *next;

  while (1) {
    head = m;
    m = m->right;
    while (1) {
      if (m->line != -1) {
        next = m->right;
        free(m);
        m = next;
      } else if (m->line == -1 && m == head) {
        next = m->below;
        free(m);
        m = next;
        cont++;
        break;
      } else {
        m = m->right;
      }
    }

    if (cont == lineCount + 1)
      break;
  }

  next = NULL;
  m = NULL;
}

Matrix *matrix_transpose(Matrix *m) {
  Matrix *transposedMatrix;
  int lines = 0, columns = 0;
  lines = count_columns(m);
  columns = count_lines(m);

  transposedMatrix = makeHeads(lines, columns);

  while (1) {
    m = m->below;
    while (1) {
      m = m->right;
      if (m->line != -1 && m->column != -1) {
        insert(transposedMatrix, lines, columns, m->column, m->line, m->info);
      } else if ((m->line == -1 && m->column == 0) ||
                 (m->line == -1 && m->column == -1)) {
        break;
      }
    }
    if (m->line == -1 && m->column == -1)
      break;
  }

  return transposedMatrix;
}

Matrix *matrix_add(Matrix *m, Matrix *n) {
  Matrix *result;
  int lines_in_m = 0, columns_in_m = 0, lines_in_n = 0, columns_in_n = 0;
  float sum = 0;
  lines_in_m = count_lines(m);
  columns_in_m = count_columns(m);
  lines_in_n = count_lines(n);
  columns_in_n = count_columns(n);

  if ((lines_in_m == lines_in_n) && columns_in_m == columns_in_n) {

    result = makeHeads(lines_in_m, columns_in_m);

    for (int i = 1; i <= lines_in_m; i++) {
      for (int j = 1; j <= columns_in_m; j++) {
        sum = 0;
        sum += matrix_getelem(m, i, j);
        sum += matrix_getelem(n, i, j);

        if (sum != 0) {
          insert(result, lines_in_m, columns_in_m, i, j, sum);
        }
      }
    }
  } else {
    printf("\nNao foi possivel somar\n");
    result = NULL;
  }

  return result;
}

Matrix *matrix_multiply(Matrix *m, Matrix *n) {
  int lines_in_m = count_lines(m), columns_in_m = count_columns(m),
      lines_in_n = count_lines(n), columns_in_n = count_columns(n);

  if (columns_in_m != lines_in_n) {
    printf("\nNao foi possivel multiplicar.\n");
    return NULL;
  }

  Matrix *result = makeHeads(lines_in_m, columns_in_n);

  for (int i = 1; i <= lines_in_m; i++) {
    for (int j = 1; j <= columns_in_n; j++) {
      float sum = 0;

      float a = matrix_getelem(m, i, j);
      float b = matrix_getelem(n, i, j);
      sum += a * b;

      if (sum != 0.0) {
        insert(result, lines_in_m, columns_in_n, i, j, sum);
      }
    }
  }

  return result;
}

Matrix *regular_matrix_multiply(Matrix *m, Matrix *n) {
  int lines_in_m = count_lines(m), columns_in_m = count_columns(m),
      lines_in_n = count_lines(n), columns_in_n = count_columns(n);

  if (columns_in_m != lines_in_n) {
    printf("\nNao foi possivel multiplicar.\n");
    return NULL;
  }

  Matrix *result = makeHeads(lines_in_m, columns_in_n);

  for (int i = 1; i <= lines_in_m; i++) {
    for (int j = 1; j <= columns_in_n; j++) {
      float sum = 0;

      float a = matrix_getelem(m, i, j);
      float b = matrix_getelem(n, i, j);
      sum += a * b;

      insert(result, lines_in_m, columns_in_n, i, j, sum);
    }
  }

  return result;
}

int count_lines(Matrix *matrix) {
  int lines = 0;
  Matrix *aux = matrix;
  while (1) {
    matrix = matrix->below;
    if (matrix->column != -1) {
      lines++;
    } else {
      break;
    }
  }
  matrix = aux;
  aux = NULL;
  return lines;
}

int count_columns(Matrix *matrix) {
  int columns = 0;
  Matrix *aux = matrix;
  while (1) {
    matrix = matrix->right;
    if (matrix->line != -1) {
      columns++;
    } else {
      break;
    }
  }
  matrix = aux;
  aux = NULL;
  return columns;
}

Matrix *big_matrix_create(unsigned int m) {
  Matrix *head;

  head = makeHeads(m, m);

  srand(time(NULL));

  for (unsigned i = 1; i <= m; i++) {
    for (unsigned j = 1; j <= m; j++) {
      insert(head, m, m, i, j, rand() % 2);
    }
  }

  return head;
}