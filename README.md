# Intro to Parallel Computing

## Matrix Multiplication 

In this exercise, we compare the performance of two implementations of the classic matrix multiplication algorithm, one sequential and one parallel (and slightly optimized).    
Generating 10 random square matrices with dimensions equal to the first 10 powers of 2.
We compare execution times of the sequential and parallel multiplication back to back and print those results on the console, with an output like this, as run on my laptop (R5 4500U, 8GB RAM):

    ----------------------------------
    Size:  Sequential   vs   Parallel 
    ----------------------------------
    2:    0.000002                    0.000014
    4:    0.000002                    0.000007
    8:    0.000008                    0.000010
    16:    0.000051                    0.000021
    32:    0.000397                    0.000076
    64:    0.003130                    0.000571
    128:    0.010018                    0.003503
    256:    0.070621                    0.033993
    512:    0.572225                    0.118816
    1024:    4.985128                    0.807434

In order to compile the C code file in the folder using gcc:

    gcc  parallelMult.c -o parallelMult.exe -fopenmp -lm

If running on Linux, it can be useful to limit the number of threads you want to use before compiling the code:

    export OMP_NUM_THREADS=#threads

Then to run the executable:

    ./parallelMult.exe
