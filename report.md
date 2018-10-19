# Assignment report on random typing (hpcgpXXX)

## Program layout

* Includes
* Defines
* Point struct
* str2point(char * str, Point * p)
* index2str(char * str, short s)
* size_t i2str(char * str, int i)
* int read_points(File * input_file, Point * start_points,
                  Point * end_points, size_t * start_buffer_size,
                  size_t * end_buffer_size)
* short point_index(Point p1, Point p2)
* calc_block(Point * start_points, Point * end_points,
             size_t start_length, size_t end_length, int * output)
* calc_triangle(Point * start_points, Point * end_points
                size_t length, int * output)
* main

Blablabla meta text here. Introduction and overview etc. The functions are described now wow...

### main
- Takes input -tX, where X is the number of threads to be used.
- Opens file containing cell information
- Allocates memory for the two buffers start_points and end_points. In order to comply with memory constraints one must make sure that 2 * POINTS_PER_BUFFER * sizeof(Point) < maximum memory usage. (Since output_occurance is of order X MB we can disregard it.)
- do while loop: Reads two sequences of cells, for which the distances are to be computed. return_value will indicate the properties of the acquired sequences. A value of 1 means that the sequences are equal and that they represent a block on the diagonal in the cell matrix. A value of 0 means that the sequences represent a block of cells not on the diagonal and 2 indicates that we are done (all cell combinations have been accounted for). The while loop breaks when the value 2 is obtained. Depending on if return_value is 0 or 1 two different calculation functions are called, that calculate the cell distances for a rectangular or triangular block in the cell matrix.
- A loop then converts the computed distances and their count to a character array, which is printed to stdout.

### Point
~~~~ c
typedef struct{
	short x, y, z;
} Point;
~~~~
- Contains the x, y, and z positions of a cell.

### read_points
- Description of function: read_points tries to read a new block of entries in the cell matrix. First the top left block is obtained and after that the function attempts to find a new block on the right side of the old one. If the end of a row is reached the next block will be on the diagonal underneath the previous level. When a block of entries has been read the values are stored in the pointer provided in the arguments.

- Input arguments
1. FILE * restrict input_file, reference to the file containing cells.
2. Point start_points[], array to store first sequence of cells in.
3. Point end_points[], array to store second sequence of cells in.
4. size_t * restrict start_buffer_size, pointer used to store the size of the first sequence of cells.
5. size_t * restrict end_buffer_size pointer used to store the size of the second sequence of cells.
- Return value: the function output indicates whether the two sequences of cells are identical (1) or not (0), or if there are no more cells to read (2).
- Variables
1. bytes_read is used to track how many cells were read from the file.
2. position_in_file tracks the position in the file of the first sequence of cells.
3. file_content is used to temporary store information that is being read.
- First the function checks if position_in_file equals zero or if we are at the end of the file. If the first case is true, it means that no sequence of start points has been read. If so the first block of the matrix must be obtained. Using fread the sequence of cells is stored in file_content. Then, str2point extracts the cells and saves the positions in start points. Since we are at a diagonal, the end points will be identical to the start points and they are copied from start_points using memcpy. Now, consider the second case in the if statement to be true. This means that we have reached the right side of the matrix. Then we must find a new sequence of start points so we seek in the file to find the next diagonal block to read. Then the sequences are stored as before. However, being at the end of the file can also mean that the whole matrix has been traversed. If bytes_read is 0 it would indicate that the end of the matrix has indeed been reached and the function can return 2 to signal this.

If position_in_file is non zero and the end of the file has not been reached, then a new block of entries can be found immediately to the right in the cell matrix. Hence the sequence of start points must not be updated, only the end points. In this case, it is certain that a rectangular block of cells has been obtained in contrast to within the if statement, where only the elements in the upper triangular block are relevant.

### calc_block
- Finds the distance between all points in a rectangular block of the cell matrix.
- Input arguments:
1. const Point start_points[], array of points representing the column values of a block in the cell matrix.
2. const Point end_points[], array of points representing the row values of a block in the cell matrix.
3. const size_t start_length, the height of the block.
4. const size_t end_length, the width of the block.
5. unsigned int output[], the array used to count the frequency of distances.
- A double for loop traverses all positions of the block in the cell matrix. For each position the distance between the current points is calculated using point_index. Two points are calculated in every loop iteration.

### calc_triangle
- Finds the distance between all points in a triangular block of the cell matrix.
- Input arguments:
1. const Point start_points[], array of points representing the column values of a block in the cell matrix.
2. const Point end_points[], array of points representing the row values of a block in the cell matrix.
3. const size_t length, the height and width of the block.
5. unsigned int output[], the array used to count the frequency of distances.
- A double for loop traverses all positions of the upper triangular block. For each position the distance between the current points is calculated using point_index. Two points are calculated in every loop iteration.

### point_index
- Performs the calculation of the distance between two points.
- Input arguments:
1. const Point p1, p1 represents the first point.
2. const Point p2, p2 represents the second point.
- Returns a short containing the Euclidian distance between p1 and p2.

### str2point
- Converts three positions from a char array to a Point.
- Input arguments:
1. char * restrict constant str, represents a three dimensional point. Must be formatted as "x_position y_position z_position", where each position is formatted as "SAB.CDE", S = + or -, A-E are numbers.
2. Point * restrict p, pointer used to store the obtained Point.

### index2str
- Converts the cell distance from short to string for output.
- Input arguments:
1. char * str, pointer used to store the results.
2. short s, a cell distance expressed as a four digit integer.

### i2str
- Converts an int to char[]. Is used to convert the count of how many times a distance appears.
- Input arguments:
1. char * str, pointer to store the output.
2. int i, the integer to be converted.


## Performance

Experimentation reveals that the limiting factor to performance is the user input. Indeed, rand, printf, and scanf can be expected to be reasonably optimized, being part of the standard library. The user is the sole nonstandard component, which requires further performance evaluation. To this end, we have benchmarked several course participants. Results are summarized in the following table; Times are given in milliseconds.

| User name | N=10 | N=100 | N=1000 |
|:----:|----:|----:|----:|
| hpcuser573 | 1603 | 17680 | 135520 |
| hpcuser614 | 1663 | 18180 | 136570 |
| hpcuser637 | 1279 | 10550 |  92190 |
| hpcuser789 | 1386 | 14640 | 119840 |

Benchmarks reveal a consistent pattern: hpcuser637 yields the fasted input. Consulting the course material we, however, could not find a theoretical explanation for the observed differences.
