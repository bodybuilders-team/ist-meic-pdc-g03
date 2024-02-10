# ist-meic-pdc-gXX

> Parallel and Distributed Computing project of group XX - MEIC @ IST 2023/2024.

## Authors

- [110817 André Páscoa](https://github.com/devandrepascoa)
- [110860 André Jesus](https://github.com/andre-j3sus)
- [110893 Nyckollas Brandão](https://github.com/Nyckoka)

@IST<br>
Master in Computer Science and Computer Engineering<br>
Parallel and Distributed Computing - Group XX<br>
Summer  Semester of 2023/2024

## About the Project

The project focuses on gaining experience in parallel programming on shared and distributed memory systems, utilizing OpenMP and MPI, respectively. The task involves implementing sequential and parallel versions of the 3D Game of Life.

## Repository Structure

- `src/`: Contains the source code files for the sequential, OpenMP, and MPI implementations.
- `include/`: Optional directory for header files if needed.
- `data/`: Contains input and output data files.
- `doc/`: Documentation directory containing the report discussing implementation details and results.
- `Makefile`: Contains instructions for compiling the code.
- `README.md`: Instructions and information about the project.

## Instructions

To compile the code, use the provided Makefile. Ensure you have the necessary dependencies installed.

To run the code:
- For the serial implementation: `./life3d <parameters>`
- For the OpenMP implementation: `./life3d-omp <parameters>`
- For the MPI implementation: `mpirun -np <num_processes> ./life3d-mpi <parameters>`

Replace `<parameters>` with the required input parameters.

Refer to the documentation in the `doc/` directory for detailed information on the implementations, including performance results and analysis.
