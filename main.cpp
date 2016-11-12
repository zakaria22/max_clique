#include <iostream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <ctime>
#include <cstdint>
#include <ostream>

#include "sousgraph.h"
#include "sousgraph.cpp"
#include "matrice.h"
#include "matrice.cpp"

using namespace std;
void counter(int t){
	
  int secondes = t % 60;
  int minutes = (t / 60) % 60;
  int heures = (t / (60 * 60));

  cout << heures << " h " << minutes << " min " << secondes <<" s " << endl;
}

int main(int argc, char **argv) {
  time_t tbegin,tend;
  double t;
  SousGraph g;
  string s;
  cout<<"Entrer le nom de fichier " << endl;
  cin >> s;
  string filename =  "/home/etudiant/projects/graph/Base/"+s;
  
  Matrice graph(filename.c_str());
  
  uint64_t tab[T_SIZE(graph.taille())];
  tailleSousGraph(g,tab,graph.taille());
  
  tbegin=time(NULL);              // get the current calendar time
  cliqueMax(graph,g);
  
  tend=time(NULL);
  if (!vide(g)){
    cout << "\n\nLa clique maximale de ce graphe contient" << endl;
    afficher(g, cout);
  }
  t=difftime(tend,tbegin);  
  cout << endl;
  counter((int)t);  
  return 0;
}
