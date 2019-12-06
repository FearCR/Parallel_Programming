# Guia de funciones y uso de OpenMP
* **Compilacion y ejecucion:**  
#include <omp.h>
```Bash
g++ -fopenmp file.cpp
```
  
**Region Paralela**
```C++
# pragma omp parallel num_threads(thread_count)
//metodo

# pragma omp parallel num_threads(thread_count)
{
//codigo paralelo
}
```
**rank y thread_count**
```C++
int my_rank=omp_get_thread_num();
int thread_count=omp_get_num_threads();
```
**region crtica para prints y variables globales 
```C++
# pragma omp critical
//linea critica
```
**Reduction**
```C++
# pragma omp parallel num_threads(thread_count) reduction(+:resultado_global)
```
Se puede usar +,*,-,&,|,^,&&,||  
  
**Parallel For**
```C++
# pragma omp parallel for num_threads(thread_count) reduction(+:variable)
for(...){
  //se trabaja con la variable y al final el reduction suma todos los hilos en el principal
}
```
**Private y shared**
Define variables independientes para cada proceso y compartidas por todos los procesos  
```C++
# pragma omp parallel for num_threads(thread_count) private(i,j), shared(A,x,y,n,m)
for(){
//codigo
}
```
