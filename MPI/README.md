# Guia de funciones y uso de MPI
* **Compilacion y ejecucion:**  

```Bash
mpicc -g -Wall -o mpi_hola mpi_hola.c  
mpic++  

mpiexec -n numeroDeProcesos ejecutable
```
* **Lineas basicas que tiene todo programa de MPI:**     
```C++
#include <mpi.h>


MPI_Init(NULL,NULL) //inicia MPI  
comm=MPI_COMM_WORLD //comunicador  

MPI_Comm_size(MPI_COMM_WORLD,&variable_int)//para tener el numero total de procesos  
MPI_Comm_rank(MPI_COMM_WORLD, &variable_int)//para tener el id de cada proceso de 0 a numProcesos  


MPI_Send(msg_buffer*, size, type,Process_destination,tag,MPI_COMM_WORLD)    
MPI_Recv(msg_buffer*, size, type, Process_source, tag, MPI_COMM_WORLD, status ) //MPI_STATUS_IGNORE  

/*
  Tipos:
  MPI_CHAR
  MPI_INT  
  MPI_LONG
  MPI_FLOAT
  MPI_DOUBLE
  etc
*/

if(my_rank==0){//proceso 0 hace algo diferente}  


MPIFinalize() //termina MPI  
```  

Ver diapositiva 39 para un ejemplo con sends y recieves multiples con un input de usuario.  


* **Comunicacion Colectiva de procesos:**

```C++
MPI_Reduce(input_data*,output_data*,count,data_type,MPI_operator,dest_process,MPI_COMM_WORLD)
//realiza una operacion "tree_structured" de todos los procesos hacia un proceso destino en una variable o variables especificas
  /*
  MPI_MAX//maximo  
  MPI_MIN//minimo  
  MPI_SUM//suma
  MPI_PROD//multiplicacion
  MPI_LAND//and logico
  MPI_BAND//and bitwise
  MPI_LOR//or logico
  MPI_BOR//or bitwise
  MPI_LXOR//xor logico
  MPI_BXOR//xor bitwise
  MPI_MAXLOC//maximo y location
  MPI_MINLOC//minimo y location
  */


//si se hacen multiples llamados a reduce, se toma en cuenta el orden en que se hacen
//NO el nombre de la memoria, ver filminas 50, 51 y 52 del cap 3
```
**NO MEZCLAR FUNCIONES COLECTIVAS**  
**NO MEZCLAR DESTINOS, LOS ARGUMENTOS DEBEN SER COMPATIBLES**


```C++
MPI_Allreduce(input_data*, output_data*, count, data_type, MPI_operator, MPI_COMM_WORLD)
//Se usa cuando se necesita que todos los procesos tengan el resultado global
```   

```C++
MPI_Bcast(data*, count,data_type, process_source, MPI_COMM_WORLD)
// envia a todos los procesos datos perteneciente a un solo proceso (Broadcast)
```  
un ejemplo de usos multiples de bcast con inputs de usuario se puede ver en la diapositiva 58. 
  
```C++
MPI_Scatter(send_buffer*,send_count,data_type, recieve_buffer*,recieve_count,data_type,process_source, MPI_COMM_WORLD)
//distribuye y envia un vector a otros procesos

double* send_buffer=malloc(size*sizeof(double))//se hace antes del scatter al definir la variable, es solo un recordatorio
send_buffer[i]= ...
//ejemplo en filmina 65
```  
  
```C++
MPI_Gather(send_buffer*,send_count,data_type,recieve_buffer,recieve_count,data_type,Process_destination,MPI_COMM_WORLD)
//proceso inverso a scatter, recoge las partes distribuidas de un vector y las une en un proceso determinado(proceso 0)
double* recieve_buffer=malloc(size*sizeof(double))//se hace antes del gather al definir la variable, es solo un recordatorio
prinf(recieve_buffer[i])...
//ejemplo en filmina 68
```  
  
```C++
MPI_Allgather(send_buffer*,send_count,data_type,recieve_buffer,recieve_count,data_type, MPI_COMM_WORLD)
//concatena los contenidos de cada procesos(send_buffer) y los guarda en cada proceso (recieve_buffer)
```  
  
```C++
MPI_Barrier(MPI_COMM_WORLD)
//espera a que todos los demas procesos lleguen a ese punto, usado mas que todo para que un procesos no finalice el programa
//antes de que los demas terminen.
```
