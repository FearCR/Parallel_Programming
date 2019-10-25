//compila con g++ ContarHTML.cc
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include <mutex>
using namespace std;
map<string, int> etiquetas;
ifstream file;
std::mutex mymutex;


void contar(){
  std::thread::id * myid = new std::thread::id;
  *myid = std::this_thread::get_id();
  int line=0;
  while(file.good())
  {


    string s;
    string word;
    mymutex.lock();
    getline(file, s);
    line++;
    mymutex.unlock();
    int pos = s.find_first_of('>');
    int ini = s.find_first_of('>');
    if(pos < 0) continue;
    while(s.size() > 0)
    {
      word="";
      ini = s.find_first_of('<');
      pos = s.find_first_of('>');
      if(pos < 0 )
      pos = s.size();
      if(ini>=0){
        word = s.substr(ini, (pos+1)-ini);//substr(posicionInicial, cantidad de caracteres a la derecha)
      }else{
        ini=0;
      }
      if(word != "")
      mymutex.lock();
      etiquetas[word]++;
      s = s.erase(0, (pos+1));//erase(posicionInicial, cantidad de caracteres a la derecha)
      mymutex.unlock();
    }
  }
  mymutex.lock();
  cout<<"hi im thread "<<*myid<< " i read "<<line<<" lines"<<endl;
    mymutex.unlock();
delete myid;
}


void imprimirMapa(){
  for(map<string, int>::iterator wit = etiquetas.begin(); wit != etiquetas.end(); ++wit)
  cout << "etiqueta: " << wit->first << " | Encontro: " << wit->second << endl;
}



int main(int argc, char **argv)
{
  file.open("b");
  if(!file.is_open()) return 1;

    std::thread * threadpointer = new std::thread[5];

    for (int i = 0; i < 5; i++)
      {
        threadpointer[i] = std::thread(contar);
      }
    for (int i = 0; i < 5; i++)
        // Join will block our main thread, and so the program won't exit until
        // everyone comes home.
      {
        threadpointer[i].join();
      }
    delete [] threadpointer;
    file.close();
  imprimirMapa();
  return 0;
}
