# Sparse Matrix Library in C
This repository contains an example of a implementation of sparse matrices in C language, where a matrix is represented using Linked Lists. A sparse matrix is a matrix in which most of the elements are zero. By only storing non-zero elements, we can save memory and processing time for operations.

## Files
1. 'sparseMatrix.c' : The main C file containing implementation of various operations on Sparse Matrices

## Structures
The data structures used are as follows:

* 'struct Element :' This structure represents a cell in the matrix with its value, row and column index, along with links to the next element in the same row and column.
* 'struct Header :' This structure holds information about a particular row or column.
* 'struct Matrix :' This structure represents a sparse matrix with links to the head of row and column headers, and information about the total rows and columns in the matrix, and a fill value for empty cells.

## Functions
Here is a brief overview of the functions available in the Sparse Matrix library:

* 'void setColumnHeader(struct Matrix *m, struct Element *newElement) :' Sets column header for a new element in the matrix.
* 'void setRowHeader(struct Matrix *m, struct Element *newElement) :' Sets row header for a new element in the matrix.
* 'struct Matrix MTRX_alloc(char *filename, int fillValue) :' Allocates memory for a matrix and initializes it from a file.
* 'void MTRX_free(struct Matrix* m) :' Frees memory allocated to a matrix.
* 'int MTRX_getElement(struct Matrix *m, int row, int column) :' Returns the value of a cell in a matrix.
* 'void MTRX_write(struct Matrix *m, char *filename) :' Writes a matrix to a file.
* 'struct Matrix MTRX_add(struct Matrix *a, struct Matrix *b, int fillValue) :' Adds two matrices and returns the resulting matrix.
* 'struct Matrix MTRX_multiply(struct Matrix *a, struct Matrix *b, int fillValue) :' Multiplies two matrices and returns the resulting matrix.
* 'struct Matrix MTRX_transpose(struct Matrix *m) :' Returns the transpose of a matrix.
* The main function contains a simple use-case of these functions where it reads a matrix from a file, calculates its transpose and writes the result to another file.

## Usage
1. Clone the repository.
2. Open the terminal and navigate to the directory where the code is saved.
3. Compile the code using a C compiler like gcc, with the command gcc -o sparseMatrix sparseMatrix.c.
4. Run the code using the command ./sparseMatrix.

## How to Run

```bash
gcc main.c -o main
./main
```

## License
This project is open source and freely available.

## Contact
Please raise an issue in this GitHub repository for any queries or suggestions.

## Author
Abdul Lateef FNU
