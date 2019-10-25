

// Compile with g++ h20.cc -lpthread
//
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define NUMTHRDS 100

#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"

int status;
int n = 256;
pthread_t thds[NUMTHRDS];
pthread_mutex_t mutex;

sem_t sC, sM;

int cC = 0, cM = 0;

int main ( int argc, char *argv[] );
void *C ( void *arg );
void *M ( void *arg );


int main ( int argc, char *argv[] ) {

  long i;

  sem_init( &sC, 0, 0 );
  sem_init( &sM, 0, 0 );

  pthread_attr_t attr;
  pthread_mutex_init ( &mutex, NULL );
  pthread_attr_init ( &attr );
  pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

  for ( i = 0; i < NUMTHRDS; i++ ) {
    if ( 0 == ( rand() % 2 ) )
       pthread_create ( &thds[i], &attr, C, ( void * ) i );
    else
       pthread_create ( &thds[i], &attr, M, ( void * ) i );
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


void *C ( void *arg ) {

   long i;

   i = (long) arg;

   pthread_mutex_lock ( &mutex );	// Lock access to cH and cO variables

   if ( (cC > 1) ) {	//soy un C, si hay otros 2 C me subo
      cC-=2;	// Using a C
      cout << "Nos subimos 3"<<RED" Canibales"<<BLACK" al bote " << i << endl;
      pthread_mutex_unlock ( &mutex );	//Free the mutex
      sem_post( &sC );		// Free the waiting thread
      sem_post( &sC );		// Free the waiting thread
   } else if( cM>1) {//soy un C, si hay otros 2 M, me subo
     cM-=2;
     cout << "Nos subimos 2"<<BLUE" Misioneros"<<BLACK<<" y 1 " <<RED"Canibal"<<BLACK" al bote " << i << endl;
     pthread_mutex_unlock ( &mutex );	//Free the mutex
     sem_post( &sM );		// Free the waiting thread
     sem_post( &sM );
   }else{// si no hay 2 M o 2 C, me espero
     cC++;			// Increment the C count
     cout << "Soy un "<<RED"Canibal"<<BLACK", estoy esperando: " << i << endl;
     pthread_mutex_unlock ( &mutex );	// Free the mutex
     sem_wait( &sC );		// Wait for the others
   }

   pthread_exit ( ( void * ) 0 );
}


void *M ( void *arg ) {

   long i;

   i = (long) arg;

   pthread_mutex_lock ( &mutex );  // Lock access to cH and cO variables

   if ( cM > 1 ) {	//soy un M, si hay otros 2 M me subo
      cM-=2;		// We use two H's
      cout << "Nos subimos 3 "<<BLUE"Misioneros"<<BLACK" al bote " << i << endl;
      pthread_mutex_unlock ( &mutex );	//Free mutex
      sem_post( &sM );	// Free the waiting threads
      sem_post( &sM );
   } else if(cM>0 && cC>0){//Soy un M, si hay otro M y un C me subo
     cM--;	// Using a M
     cC--;	// Using a C
     cout << "Nos subimos 2"<<BLUE" Misioneros"<<BLACK" y 1"<<RED" Canibal"<<BLACK" al bote " << i << endl;
     pthread_mutex_unlock ( &mutex );	//Free the mutex
     sem_post( &sC );		// Free the waiting thread
     sem_post( &sM );		// Free the waiting thread
   }else{//Soy un M, si no hay 2 M, o 1M y 1 C me espero
     cM++;		// Increment M count, then wait
     cout << "Soy un "<<BLUE"Misionero"<<BLACK", estoy esperando: " << i << endl;
     pthread_mutex_unlock ( &mutex );	// Free mutex
     sem_wait( &sM );	// Wait for other threads
   }

   pthread_exit ( ( void * ) 0 );	// Finish this thread
}
