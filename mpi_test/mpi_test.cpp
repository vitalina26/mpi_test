#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// size of array
#define n 10000



// Temporary array for slave process
int a2[1000];

int main(int argc, char* argv[])
{
    int a[n];
    srand(time(NULL));
    for (size_t i = 0; i < n; i++)
    {
        a[i] = rand() % 100;

    }

    int rank, size,
        elements_per_process,
        n_elements_recieved;
    double t, start, end;
    // np -> no. of processes
    // rank -> process 

    MPI_Status status;
   
    // Creation of parallel processes
    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    // find out process rank,
    // and how many processes 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
  
    // master process
    if (rank == 0) {
        int index, i;
        elements_per_process = n / size;
        // check if more than 1 processes are run
        if (size > 1) {
            // distributes the portion of array
            // to child processes to calculate
            // their partial sums
            for (i = 1; i < size - 1; i++) {
                index = i * elements_per_process;

                MPI_Send(&elements_per_process,
                    1, MPI_INT, i, 0,
                    MPI_COMM_WORLD);
                MPI_Send(&a[index],
                    elements_per_process,
                    MPI_INT, i, 0,
                    MPI_COMM_WORLD);
            }

            // last process adds remaining elements
            index = i * elements_per_process;
            int elements_left = n - index;

            MPI_Send(&elements_left,
                1, MPI_INT,
                i, 0,
                MPI_COMM_WORLD);
            MPI_Send(&a[index],
                elements_left,
                MPI_INT, i, 0,
                MPI_COMM_WORLD);
        }

        // master process add its own sub array
        int sum = 0;
        for (i = 0; i < elements_per_process; i++)
            sum += a[i];

        // collects partial sums from other processes
        int tmp;
        for (i = 1; i < size; i++) {
            MPI_Recv(&tmp, 1, MPI_INT,
                MPI_ANY_SOURCE, 0,
                MPI_COMM_WORLD,
                &status);
            int sender = status.MPI_SOURCE;

            sum += tmp;
        }

        // prints the final sum of array
        printf("avg of array is : %d\n", sum / n);
        end = MPI_Wtime();
        t = end - start;
        std::cout << "time : " << t << std::endl;
    }
    
    else {
        MPI_Recv(&n_elements_recieved,
            1, MPI_INT, 0, 0,
            MPI_COMM_WORLD,
            &status);

        // stores the received array segment
        // in local array a2
        MPI_Recv(&a2, n_elements_recieved,
            MPI_INT, 0, 0,
            MPI_COMM_WORLD,
            &status);

        // calculates its partial sum
        int partial_sum = 0;
        for (int i = 0; i < n_elements_recieved; i++)
            partial_sum += a2[i];

        // sends the partial sum to the root process
        MPI_Send(&partial_sum, 1, MPI_INT,
            0, 0, MPI_COMM_WORLD);
    }
  
    // cleans up all MPI state before exit of process
    MPI_Finalize();
   
    return 0;
}