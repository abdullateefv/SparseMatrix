/*
 * Abdul Lateef FNU
 * Modified: 5/30/2023
 */

#include <stdio.h>
#include <stdlib.h>

// Element struct holds the data for an individual cell of the matrix
struct Element {
    int value; // value of the element
    int row; // row index of the element
    int column; // column index of the element
    struct Element *rowElement; // link to next element in same row
    struct Element *columnElement; // link to next element in same column
};

// Header struct holds the information about a particular row or column
struct Header {
    int index; // index of the row/column this header refers to
    struct Header *header; // link to next header
    struct Element *element; // link to first element in this row/column
};

// Matrix struct defines a sparse matrix
struct Matrix {
    struct Header *headRowHeader; // head of row headers linked list
    struct Header *headColumnHeader; // head of column headers linked list
    int rows; // total rows in matrix
    int columns; // total columns in matrix
    int fillValue; // value to fill for empty cells in the matrix
};

// setColumnHeader: Function to set column header for a new element in matrix
void setColumnHeader(struct Matrix *m, struct Element *newElement) {
    if (m->headColumnHeader == NULL) {
        struct Header *newColumnHeader = malloc(sizeof(struct Header)); newColumnHeader->index = newElement->column; newColumnHeader->header = NULL; newColumnHeader->element = newElement;
        m->headColumnHeader = newColumnHeader;
        return;
    }
    if (m->headColumnHeader->index > newElement->column) {
        struct Header *newColumnHeader = malloc(sizeof(struct Header));
        newColumnHeader->index = newElement->column; newColumnHeader->header = m->headColumnHeader; newColumnHeader->element = newElement;
        m->headColumnHeader = newColumnHeader;
        return;
    }
    if (m->headColumnHeader-> index == newElement->column) {
        struct Element *currentElement = m->headColumnHeader->element;
        while (currentElement->columnElement != NULL) {
            currentElement = currentElement->columnElement;
        }
        currentElement->columnElement = newElement;
        return;
    }
    if (m->headColumnHeader->index < newElement->column) {
        struct Header *currentHeader = m->headColumnHeader;
        while (currentHeader->header != NULL && currentHeader->index != newElement->column && currentHeader->header->index <= newElement->column) {
            currentHeader = currentHeader->header;
        }

        if (currentHeader->index == newElement->column) {
            struct Element *currentElement = currentHeader->element;
            while (currentElement->columnElement != NULL) {
                currentElement = currentElement->columnElement;
            }
            currentElement->columnElement = newElement;
        } else if (currentHeader->header != NULL && currentHeader->header->index > newElement->column) {
            struct Header *newColumnHeader = malloc(sizeof(struct Header)); newColumnHeader->index = newElement->column; newColumnHeader->header = currentHeader->header; newColumnHeader->element = newElement;
            currentHeader->header = newColumnHeader;
        } else if (currentHeader->header == NULL) {
            struct Header *newColumnHeader = malloc(sizeof(struct Header)); newColumnHeader->index = newElement->column; newColumnHeader->header = NULL; newColumnHeader->element = newElement;
            currentHeader->header = newColumnHeader;
        }
    }
}

// setRowHeader: Function to set row header for a new element in matrix
void setRowHeader(struct Matrix *m, struct Element *newElement) {
    if (m->headRowHeader == NULL) {
        struct Header *newRowHeader = malloc(sizeof(struct Header)); newRowHeader->index = newElement->row; newRowHeader->header = NULL; newRowHeader->element = newElement;
        m->headRowHeader = newRowHeader;
        setColumnHeader(m, newElement);
    } else {
        struct Header *currentHeader = m->headRowHeader;
        while (currentHeader->header != NULL) {
            currentHeader = currentHeader->header;
        }
        if (currentHeader->index == newElement->row) {
            struct Element *currentElement = currentHeader->element;
            while (currentElement->rowElement != NULL) {
                currentElement = currentElement->rowElement;
            }
            currentElement->rowElement = newElement;
            setColumnHeader(m, newElement);
        } else if (currentHeader->index < newElement->row) {
            struct Header *newRowHeader = malloc(sizeof(struct Header)); newRowHeader->index = newElement->row; newRowHeader->header = NULL; newRowHeader->element = newElement;
            currentHeader->header = newRowHeader;
            setColumnHeader(m, newElement);
        }
    }
}

// MTRX_alloc: Function to allocate memory for a matrix and initialize it from a file
struct Matrix MTRX_alloc(char *filename, int fillValue) {
    struct Matrix m;
    FILE* file;
    int ch;
    int nextChar;
    int columnCursor = 0;
    int charCount = 0;
    int rowCursor = 1;
    m.fillValue = fillValue;
    m.headColumnHeader = NULL;
    m.headRowHeader = NULL;

    file = fopen(filename, "r");

    while (fscanf(file, "%d", &ch) == 1) {
        columnCursor++;
        charCount++;
        if (ch != fillValue) {
            struct Element *newElement = malloc(sizeof(struct Element));
            newElement->value = ch; newElement->row = rowCursor; newElement->column = columnCursor; newElement->rowElement = NULL; newElement->columnElement = NULL;
            setRowHeader(&m, newElement);
        }

        nextChar = fgetc(file);
        if (nextChar == '\n') {
            columnCursor = 0;
            rowCursor++;
        } else {
            ungetc(nextChar, file);
        }
    }
    rowCursor--;
    m.rows = rowCursor;
    if (rowCursor != 0) {
        m.columns = charCount / rowCursor;
    } else {
        m.columns = 0;
    }

    fclose(file);
    return m;
}

// MTRX_free: Function to free memory allocated to a matrix
void MTRX_free(struct Matrix* m) {
    struct Header *nextRowHeader;
    struct Header *currentColumnHeader;
    struct Header *currentRowHeader = m->headRowHeader;
    while (currentRowHeader != NULL) {
        struct Element *currentElement = currentRowHeader->element;
        while (currentElement != NULL) {
            struct Element *nextElement = currentElement->rowElement;
            free(currentElement);
            currentElement = nextElement;
        }
        nextRowHeader = currentRowHeader->header;
        free(currentRowHeader);
        currentRowHeader = nextRowHeader;
    }
    currentColumnHeader = m->headColumnHeader;
    while (currentColumnHeader != NULL) {
        struct Header *nextHeader = currentColumnHeader->header;
        free(currentColumnHeader);
        currentColumnHeader = nextHeader;
    }
}

// MTRX_getElement: Function to get value of a cell in a matrix
int MTRX_getElement(struct Matrix *m, int row, int column) {
    struct Header *currentRowHeader;
    if (m->headRowHeader == NULL || m->headColumnHeader == NULL || row > m->rows || column > m->columns) {
        return m->fillValue;
    }
    currentRowHeader = m->headRowHeader;
    while (currentRowHeader->index != row && currentRowHeader->header != NULL) {
        currentRowHeader = currentRowHeader->header;
    }
    if (currentRowHeader->index != row) {
        return m->fillValue;
    }
    if (currentRowHeader->index == row) {
        struct Element *currentElement = currentRowHeader->element;
        while (currentElement->column != column && currentElement->rowElement != NULL) {
            currentElement = currentElement->rowElement;
        }
        if (currentElement->column == column) {
            return currentElement->value;
        } else {
            return m->fillValue;
        }
    }
    return m->fillValue;
}

// MTRX_write: Function to write a matrix to a file
void MTRX_write(struct Matrix *m, char *filename) {
    FILE *file = fopen(filename, "w");
    int row, column;
    for (row = 1; row <= m->rows; row++) {
        for (column = 1; column <= m->columns; column++) {
            fprintf(file, "%d", MTRX_getElement(m, row, column));
            if (column < m->columns) {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

// MTRX_add: Function to add two matrices
struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue) {
    struct Matrix output;
    int row, column;
    output.fillValue = fillValue;
    output.headColumnHeader = NULL;
    output.headRowHeader = NULL;
    output.rows = a->rows;
    output.columns = a->columns;

    for (row = 1; row <= a->rows; row++) {
        for (column = 1; column <= a->columns; column++) {
            int aElement = MTRX_getElement(a, row, column);
            int bElement = MTRX_getElement(b, row, column);
            if (aElement + bElement != output.fillValue) {
                struct Element *newElement = malloc(sizeof(struct Element));
                newElement->value = aElement + bElement; newElement->row = row; newElement->column = column; newElement->rowElement = NULL; newElement->columnElement = NULL;
                setRowHeader(&output, newElement);
            }
        }
    }
    return output;
}

// MTRX_multiply: Function to multiply two matrices
struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue) {
    struct Matrix output;
    int row, column;
    if (a->columns != b->rows) {
        printf("Invalid matrix dimensions for multiplication");
        exit(1);
    }

    output.fillValue = fillValue;
    output.headColumnHeader = NULL;
    output.headRowHeader = NULL;
    output.rows = a->rows;
    output.columns = b->columns;


    for (row = 1; row <= a->rows; row++) {
        for (column = 1; column <= b->columns; column++) {
            int sum = 0;
            int pos;
            for (pos = 1; pos <= a->columns; pos++) {
                sum += MTRX_getElement(a, row, pos) * MTRX_getElement(b, pos, column);
            }
            if (sum != output.fillValue) {
                struct Element *newElement = malloc(sizeof(struct Element));
                newElement->value = sum; newElement->row = row; newElement->column = column; newElement->rowElement = NULL; newElement->columnElement = NULL;
                setRowHeader(&output, newElement);
            }
        }
    }
    return output;
}

// MTRX_transpose: Function to calculate transpose of a matrix
struct Matrix MTRX_transpose(struct Matrix *m) {
    struct Matrix output;
    int row, column;
    output.fillValue = m->fillValue;
    output.headColumnHeader = NULL;
    output.headRowHeader = NULL;
    output.rows = m->columns;
    output.columns = m->rows;

    for (row = 1; row <= output.rows; row++) {
        for (column = 1; column <= output.columns; column++) {
            int element = MTRX_getElement(m, column, row);
            if (element != output.fillValue) {
                struct Element *newElement = malloc(sizeof(struct Element));
                newElement->value = element; newElement->row = row; newElement->column = column; newElement->rowElement = NULL; newElement->columnElement = NULL;
                setRowHeader(&output, newElement);
            }
        }
    }
    return output;
}

// main: Entry point with example matrix transposal use-case
int main() {
    struct Matrix a, t;
    a = MTRX_alloc("matrixA.txt", 0);
    t = MTRX_transpose(&a);
    MTRX_write(&t, "matrixT.txt");
    MTRX_free(&t);
    MTRX_free(&a);
    return 0;
}
