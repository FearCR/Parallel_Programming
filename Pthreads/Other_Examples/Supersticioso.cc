// Compile with g++ Supersticioso.cc -lpthread


/*
Casos a considerar:
NO pueden haber 13 hilos procesando.

1-Si hay menos de 12 hilos, pueden entrar y salir hasta llegar a 12.
2-Si hay 12 hilos, tienen que entrar 2 a la vez.

3-Si hay mas de 14 hilos, pueden entrar y salir hasta llegar a 14.
4-Si hay 14 hilos solo pueden salir 2 a la vez.
5-si hay 12 o 14 pueden entrar y salir al mismo tiempo, en caso de que
a)alguien quiera salir y hay uno esperando fuera para entrar
b)alguien quiera entrar y hay uno esperando dentro para salir
*/
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
#define NUMTHRDS 100

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"

int status;
int n = 256;
pthread_t thds[NUMTHRDS];

pthread_mutex_t mutex;
sem_t sT;

int adentro = 0;
int esperandoDentro =0;
int esperandoFuera=0;
int main ( int argc, char *argv[] );
void *T ( void *arg );


int main ( int argc, char *argv[] ) {

  long i;

  sem_init( &sT, 0, 0 );


  pthread_attr_t attr;
  pthread_mutex_init ( &mutex, NULL );
  pthread_attr_init ( &attr );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

  for ( i = 0; i < NUMTHRDS; i++ ) {
    pthread_create ( &thds[i], &attr, T, ( void * ) i );
  }

  pthread_attr_destroy ( &attr );

  for ( i = 0; i < NUMTHRDS; i++ ) {
    pthread_join ( thds[i], ( void ** ) &status );
  }

  pthread_mutex_destroy ( &mutex );
  //  pthread_exit ( NULL );
  cout << "\n";
  cout << "  Normal end of execution.\n";

  return 0;
}

void *T ( void *arg ) {
  long i;
  int tiempo=rand()%100;
  i = (long) arg;
  pthread_mutex_lock ( &mutex );
  //emtrar y procesar
  if(adentro<12){//caso1
    adentro++;
    cout<< "soy "<< i<<","<<BLUE" entré"<<BLACK" al cuarto, ahora en el cuarto habemos "<< adentro<<endl;
    pthread_mutex_unlock ( &mutex );
    sleep_for(seconds(tiempo));
  }else if(adentro>=14){//caso 3
    adentro++;
    cout<< "soy "<< i<<","<<BLUE" entré"<<BLACK" al cuarto, ahora en el cuarto habemos "<< adentro<<endl;
    pthread_mutex_unlock ( &mutex );
    sleep_for(seconds(tiempo));
  }else if(adentro==12){//caso 2
    if(esperandoFuera>0){//si hay alguno esperando afuera entran 2 a la vez
      esperandoFuera--;
      adentro+=2;
      cout<< "soy "<< i<<","<<BLUE" entré"<<BLACK" al cuarto con alguien esperando, ahora en el cuarto habemos "<< adentro<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_post(&sT);
      sleep_for(seconds(tiempo));

    }else if(esperandoDentro>0){
      esperandoDentro--;
      cout<< "soy "<< i<<","<<BLUE" entré"<<BLACK" y "<<RED"salio "<<BLACK"del cuarto al mismo tiempo, ahora en el cuarto habemos "<< adentro<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_post(&sT);
      sleep_for(seconds(tiempo));

    }else {//si no hay nadie esperando , espera
      esperandoFuera++;
      cout<< GREEN"soy "<< i<<", si entro quedarian "<< adentro+1<<", entonces espero"<<BLACK""<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_wait(&sT);
      sleep_for(seconds(tiempo));
    }
  }


  pthread_mutex_lock ( &mutex );
  //salir
  if(adentro<=12||adentro>14){ //casos 1 y 3
    adentro--;
      cout<< "soy "<< i<<","<<RED" salí"<<BLACK" del cuarto, ahora en el cuarto habemos "<< adentro<<endl;
    pthread_mutex_unlock ( &mutex );
  }else if( adentro ==14){//caso 4
    if(esperandoDentro>0){//sihay alfuno esperando dentro salen 2 a la vez
      adentro-=2;
      cout<< "soy "<< i<<","<<RED" salí"<<BLACK" sali del cuarto con alguien esperando, ahora en el cuarto habemos "<< adentro<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_post(&sT);
    }else if(esperandoFuera>0){
      esperandoFuera--;
      cout<< "soy "<< i<<","<<RED" sali"<<BLACK" y "<<BLUE"entro "<<BLACK"del cuarto al mismo tiempo, ahora en el cuarto habemos "<< adentro<<endl;
      pthread_mutex_unlock ( &mutex );
      sem_post(&sT);
    }else{
        cout<< CYAN"soy "<< i<<", si salgo quedarian "<< adentro-1<<", entonces espero"<<BLACK""<<endl;
      esperandoDentro++;
      pthread_mutex_unlock ( &mutex );
      sem_wait(&sT);

    }

  }
  pthread_exit ( ( void * ) 0 );
}
