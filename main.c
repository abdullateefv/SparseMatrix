#include <stdio.h>
#include <stdlib.h>

struct Element {
    int value;
    int row;
    int column;
    struct Element *rowElement;
    struct Element *columnElement;
};

struct Header {
    int index;
    struct Header *header;
    struct Element *element;
};

struct Matrix {
    struct Header *headRowHeader;
    struct Header *headColumnHeader;
    int rows;
    int columns;
    int fillValue;
};

void setColumnHeader(struct Matrix *m, struct Element *newElement) {
        // Set Column Headers
        if (m->headColumnHeader == NULL) {   // If no headColumnHeader exists
            struct Header *newColumnHeader = malloc(sizeof(struct Header)); newColumnHeader->index = newElement->column; newColumnHeader->header = NULL; newColumnHeader->element = newElement;
            m->headColumnHeader = newColumnHeader;
            return;
        }
        if (m->headColumnHeader->index > newElement->column) { // If the headColumnHeader is further up the columns and needs to be shifted down
            struct Header *newColumnHeader = malloc(sizeof(struct Header));
            newColumnHeader->index = newElement->column; newColumnHeader->header = m->headColumnHeader; newColumnHeader->element = newElement;
            m->headColumnHeader = newColumnHeader;
            return;
        }
        if (m->headColumnHeader-> index == newElement->column) { // If the headColumnHeader is in the same position as the new element
            struct Element *currentElement = m->headColumnHeader->element;
            while (currentElement->columnElement != NULL) {
                currentElement = currentElement->columnElement;
            }
            currentElement->columnElement = newElement;
            return;
        }
        if (m->headColumnHeader->index < newElement->column) { // If the headColumnHeader appears before the new element column
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
void setRowHeader(struct Matrix *m, struct Element *newEleme) {

}
struct Matrix MTRX_alloc(char *filename, int fillValue) {
    // Initialize matrix
    struct Matrix m;
    m.fillValue = fillValue;
    m.headColumnHeader = NULL;
    m.headRowHeader = NULL;

    FILE* file = fopen(filename, "r");
    int ch;
    int columnCursor = 0;
    int charCount = 0;
    int rowCursor = 1;

    while (fscanf(file, "%d", &ch) == 1) {
        columnCursor++;
        charCount++;
        if (ch != fillValue) {
            // Process the new element
            struct Element *newElement = malloc(sizeof(struct Element));
            newElement->value = ch; newElement->row = rowCursor; newElement->column = columnCursor; newElement->rowElement = NULL; newElement->columnElement = NULL;
            // Set row headers
            if (m.headRowHeader == NULL) {
                struct Header *newRowHeader = malloc(sizeof(struct Header)); newRowHeader->index = rowCursor; newRowHeader->header = NULL; newRowHeader->element = newElement;
                m.headRowHeader = newRowHeader;
                setColumnHeader(&m, newElement);
            } else {
                struct Header *currentHeader = m.headRowHeader;
                while (currentHeader->header != NULL) {
                    currentHeader = currentHeader->header;
                }
                if (currentHeader->index == rowCursor) {
                    struct Element *currentElement = currentHeader->element;
                    while (currentElement->rowElement != NULL) {
                        currentElement = currentElement->rowElement;
                    }
                    currentElement->rowElement = newElement;
                    setColumnHeader(&m, newElement);
                } else if (currentHeader->index < rowCursor) {
                    struct Header *newRowHeader = malloc(sizeof(struct Header)); newRowHeader->index = rowCursor; newRowHeader->header = NULL; newRowHeader->element = newElement;
                    currentHeader->header = newRowHeader;
                    setColumnHeader(&m, newElement);
                }
            }
        }

        // Check for new line
        int nextChar = fgetc(file);
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
};
void MTRX_free(struct Matrix* m) {
    struct Header *currentRowHeader = m->headRowHeader;
    while (currentRowHeader != NULL) {
        struct Element *currentElement = currentRowHeader->element;
        while (currentElement != NULL) {
            struct Element *nextElement = currentElement->rowElement;
            free(currentElement);
            currentElement = nextElement;
        }
        struct Header *nextHeader = currentRowHeader->header;
        free(currentRowHeader);
        currentRowHeader = nextHeader;
    }
    struct Header *currentColumnHeader = m->headColumnHeader;
    while (currentColumnHeader != NULL) {
        struct Header *nextHeader = currentColumnHeader->header;
        free(currentColumnHeader);
        currentColumnHeader = nextHeader;
    }
}
int MTRX_getElement(struct Matrix *m, int row, int column) {
    if (m->headRowHeader == NULL || m->headColumnHeader == NULL || row > m->rows || column > m->columns) {
        return m->fillValue;
    }
    struct Header *currentRowHeader = m->headRowHeader;
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
}
void MTRX_write(struct Matrix *m, char *filename) {
    FILE *file = fopen(filename, "w");
    int row, column;
    for (row = 1; row <= m->rows; row++) {
        for (column = 1; column <= m->columns; column++) {
            fprintf(file, "%d ", MTRX_getElement(m, row, column));
        }
        fprintf(file, "\n");
    }
}
struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue);
struct Matrix MTRX_transpose(struct Matrix *m);

int main() {
    struct Matrix a, t;
    a = MTRX_alloc("matrixT.txt", 0);
    MTRX_write(&a, "output.txt");
    //t = MTRX_transpose(&a);
    //MTRX_free(&t);
    //MTRX_free(&a);
    return 0;
}