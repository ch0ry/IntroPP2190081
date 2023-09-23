# Intro to Parallel Computing

## Matrix Multiplication 

In order to compile the C code file in the folder using gcc:

    gcc  parallelMult.c -o parallelMult.exe -fopenmp -lm

If running on Linux, it can be useful to limit the number of threads you want to use before compiling the code:

    export OMP_NUM_THREADS=#threads

Then to run the executable:

    ./parallelMult.exe
