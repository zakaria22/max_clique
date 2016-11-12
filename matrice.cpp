/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <algorithm>


#include "matrice.h"
#include "sousgraph.h"

using namespace std;

//constructeur de la matrice a partir du fichier de donnée
Matrice::Matrice(const char * filename)
{
  FILE * datafile = NULL;
  unsigned char buff;
  unsigned char mask = 0x80;
  int i, j, offset;
  if ((datafile = fopen(filename, "r")) == NULL){
    cerr << "! Fichier " << filename << " inexistant ou illisible" << endl;
    exit(EXIT_FAILURE);
    
  }
  sommet = numSommet(&datafile);
  /* On alloue le tableau de tableaux de booleéns */
  matrice = (bool **) calloc(sommet, sizeof(bool *));
  for (i=0; i<sommet; i++){
    matrice[i] = (bool *) calloc(sommet, sizeof(bool));
    
  }
  //operation transitive, a->b est equivalent à b->a
  for (i=0; i<sommet; i++){
    for (j=0; j<= i / 8; j++){
      if (!fread(&buff, 1, 1, datafile)){
	cerr << "! Erreur de lecture au noeud " << i << endl;
	exit(EXIT_FAILURE);
	
      }
      for (offset=0; offset<8 && j * 8 + offset <= i; offset++){
	matrice[i][j * 8 + offset] = ((buff & (mask >> offset)) != 0);
	matrice[j * 8 + offset][i] = matrice[i][j * 8 + offset];		
	
      }
      
    }
    
  }
  fclose(datafile);
}

Matrice::~Matrice()
{
  for (int i=0; i<sommet; i++){
    free(matrice[i]);
    
  }
  free(matrice);
}

void Matrice::afficher(){
	int i, j;

	for (i=0; i<sommet; i++){
		cout << "[" << i + 1 << "] -> ";
		for (j=0; j<=sommet; j++){
			if (matrice[i][j]) cout << j + 1 << " ";
		}
	cout << endl;
	}
}

//ajouter un sommet adjacent a la matrice
void Matrice::adjacence(const int & value, SousGraph & sg) const {
	for (int i=0; i<sommet; i++){
		if (matrice[value][i]) ajouter(sg, i);
	}
}


/**
 le nombre de sommets du graphe
 */
int Matrice::taille() const {
	return(sommet);
}


/**
Retourne le nombre de sommets du graphe d'après le header du fichier
f pointeur vers le pointeur fichier
 */
int numSommet(FILE ** f){
	char buff[256];
	int res;

	// Absorbe la première ligne, ici inutile
	fgets(buff, 256, *f);

	// Absorbe les lignes préfixées par c
	while(fgets(buff, 256, *f) != NULL && buff[0] == 'c');

	if (buff[0] != 'p'){
		cerr << "! Erreur de lecture : fichier incorrect" << endl;
		exit(EXIT_FAILURE);
	}

	// On rembobine la ligne...
	fseek(*f, -strlen(buff), SEEK_CUR);
	// Pour la scanner avec scanf
	fscanf(*f, "%*c %*s %d %*d", &res);
	fgetc(*f); // Pour se débarasser du \n

	return(res);
}

//chercher la clique(en rajouttant un sommet a chaque fois; et tester s'il est maxmal
SousGraph cliqueMax(const Matrice & m, SousGraph & maximumC){
	vector<SousGraph> vect(m.taille());
	uint64_t C_tab[m.taille()][T_SIZE(m.taille())];
	SousGraph res;
	uint64_t res_tab[T_SIZE(m.taille())];
	tailleSousGraph(res, res_tab, m.taille());

	for (int i=0; i<m.taille(); i++){
		tailleSousGraph(vect[i], C_tab[i], m.taille());
		selectSG(vect[i]);
	}

	for (int i=m.taille() - 1; i>=0; i--){
		SousGraph cur, U;
		uint64_t cur_tab[T_SIZE(m.taille())], U_tab[T_SIZE(m.taille())];
		tailleSousGraph(cur, cur_tab, m.taille());
		tailleSousGraph(U, U_tab, m.taille());

		for (int j=i; j < m.taille(); j++){
			ajouter(U, j);
		}
		maxiC(U, cur, maximumC, m, i, vect);
		vect[i] = maximumC;
	}

	sousGraph(maximumC, vect[0]);

	return(maximumC);
}

//tester si une clique est maximal, si on peut pas faire mieux
bool maxiC(SousGraph nombre, SousGraph nombreFinal, SousGraph & maximumC, const Matrice& m, int value, vector<SousGraph> vect){
	int i;

	SousGraph U, cur;
	uint64_t U_tab[T_SIZE(m.taille())], cur_tab[T_SIZE(m.taille())];
	tailleSousGraph(U, U_tab, m.taille());
	tailleSousGraph(cur, cur_tab, m.taille());

	sousGraph(U, nombre);
	sousGraph(cur, nombreFinal);

	SousGraph adj;
	uint64_t adj_tab[T_SIZE(m.taille())];
	tailleSousGraph(adj, adj_tab, m.taille());
	m.adjacence(value, adj);
	inter(U, adj);
	ajouter(cur, value);


	if (vide(U)){
		if (cur.nombreElement > maximumC.nombreElement){
			sousGraph(maximumC, cur);
			afficher(maximumC, cout);
			return(true);
		}
	}

	while (!vide(U)){
		if (cur.nombreElement + U.nombreElement <= maximumC.nombreElement){
			return(false);
		}
		i = U.premierElement;
		if (cur.nombreElement + vect[i].nombreElement <= maximumC.nombreElement){
			return(false);
		}
		supprimer(U, i);
		if (maxiC(U, cur, maximumC, m, i, vect)){
			return(true);
		}
	}

	return(false);
}

