# Guia de funciones y uso de MPI
* Compilacion y ejecucion:  
  
```Bash
mpicc -g -Wall -o mpi_hola mpi_hola.c  
mpic++  

mpiexec -n numeroDeProcesos ejecutable
``` 
* Lineas basicas que tiene todo progrma de MPI:     
```C++
#include <mpi.h>
  
  
MPI_Init(NULL,NULL) //inicia MPI  
comm=MPI_COMM_WORLD //comunicador  

MPI_Comm_size(MPI_COMM_WORLD,&variable_int)//para tener el numero total de procesos  
MPI_Comm_rank(MPI_COMM_WORLD, &variable_int)//para tener el id de cada proceso de 0 a numProcesos  
  

MPI_Send(msg_buffer*, size, type,Process_destination,tag,MPI_COMM_WORLD)    
MPI_Recv(msg_buffer*, size, type, Process_source, tag, MPI_COMM_WORLD, status ) //MPI_STATUS_IGNORE  

  Tipos:
  MPI_CHAR
  MPI_INT  
  MPI_LONG
  MPI_FLOAT
  MPI_DOUBLE
  etc

if(my_rank==0){//proceso 0 hace algo diferente}  
  
 
MPIFinalize() //termina MPI  
```  

ver diapositiva 39 para un ejemplo con sends y recieves multiples con un input de usuario  
