# Lab 8 Git Some More
Description:
Minimat is an interactive mini-matlab style calculator for 3d vectors, written in c.
At the prompt, you can create named vectors, do basic arithmetics on them, or load/save them as a CSV file.
Each vector has 3 components, (x,y,z). One command per line. It is then parsed, run and the result is printed before returning to promt. If the expression is not assigned a name, it defaults to ans.
Vectors are kepy in memory until cleared or program is quit.
The code is split into layers:
vectorops.c/h | Math layer | add, sub, scalar on vect structs
vectorstore.c/h | Storage layer | dynamic array of stored vectors
vectorfile.c/h | File IO layer | CSV save/load
vectorutil.c/h | Helper layer | trim whitespace, validate names, parse numbers
vectorui.c/h | user interface | console input/output and command parsing
main.c | program entry | handles asking for help and starts the ui

Building:
Requires gcc and make.
make: builds program. exe file is called vect.
make clean: removes object files.
without make: gcc -Wall -Wextra -o vect main.c vectorops.c vectorstore.c vectorfile.c vectorutil.c vectorui.c
Run:
./vect starts program
./vect -h prints the help info

Commands
Put spaces around =, +, - and *.
name = x y z - create or replace a vector. Values may be separated by
spaces or commas, and z may be left out (it becomes 0).
name - display a vector.
a + b - add two vectors.
a - b - subtract two vectors.
a * 2 or 2 * a - multiply a vector by a number.
c = a + b - do an operation and store the result (works with - and * too).
list - show all stored vectors.
clear - delete all stored vectors and free their memory.
load <file> - load vectors from a CSV file.
save <file> - save all vectors to a CSV file.
fill <n> - create n test vectors named v1 to vn (1 to 20000).
quit - exit the program.

CSV files
One vector per line: name,x,y,z
load merges with what is already in memory. A vector with the same
name is overwritten, so loading the same file twice gives the same vectors.
Lines ending in \n or \r\n and blank lines are accepted.
Malformed lines (wrong number of fields, bad name, bad number) are skipped.
The program reports how many were skipped and the first bad line number.
A missing file or a file that cannot be read gives an error message and the
program continues.
save overwrites an existing file and writes only real vectors, so the file
can be loaded again or opened in Excel.
File names cannot contain spaces. Relative and absolute paths both work.

Dynamic Memory:
Vectors are stored in a dynamic array in vectorstore.c.
The array is a vect * allocated on the heap. It starts with room for 4
vectors.
When it is full, realloc doubles its size and copies the old contents. If
realloc fails, the old array is left untouched and the program reports
"out of memory" instead of crashing.
Adding a vector first looks for an existing one with the same name and
replaces it in place. Only a new name uses a new slot.
clear calls free on the whole array and resets the count and capacity to
zero, so the next vector allocates a fresh array.
The program registers atexit(cleanupStorage) in main, so the array is
freed when the program exits.
The array and its counters are static in vectorstore.c. Other files can
only use the storage functions, not the data directly.

Known Limitations:
Vector names start with a letter, use only letters, digits and _, and are
at most 49 characters. quit, list, clear, load, save and fill are
reserved.
Looking up a vector by name is a linear search, so very large sets are
slower. This is why fill is capped at 20000.
Ctrl-C ends the program immediately without freeing memory. Use quit (or
Ctrl-D) for a clean exit.
Dot product and cross product are not implemented.
