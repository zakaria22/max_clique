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
#include <cstring>

#include "sousgraph.h"

using namespace std;


void afficher(const SousGraph& v, std::ostream& out)
{
//affiche le nombre des élements dans chaque sous graphe complet
  out << "[" << v.nombreElement << " elements] ";
  for (int i=v.premierElement; i<v.taille; i++){
    if (contient(v, i)){
      out << i  << " ";
      
    }
    
  }
  out << endl;
  
}


void ajouter(SousGraph& sg, const int& v)
{
//ajouter un noeuds adjacent
  uint64_t mask = 0x80000000;
  if (v < 0 || v >= sg.taille || contient(sg, v)){
    return;
  }
  
  sg.sommet[v / 64] |= (mask >> (v % 64));
  sg.nombreElement++;
 
  if (v < sg.premierElement || sg.premierElement == -1){
    sg.premierElement = v;
  }
}


bool contient(const SousGraph& sg, const int& v)
{
//tester si l'ensemble contient déja le noeud
  uint64_t mask = 0x80000000;
  if (v < 0 || v >= sg.taille){
    return(false);
    
  }
  return(sg.sommet[v / 64] & (mask >> (v % 64)));
}

bool inter(SousGraph& sg, const SousGraph& t)
{
  if (t.taille != sg.taille || vide(sg) || vide(t)){
    return(false);
    
  }
  
  int i = sg.premierElement / 64;	// Comme ce SubGraph est forcément non vide, premierElement != -1
  uint64_t mask = 0x80000000;
  sg.premierElement = -1;
  sg.nombreElement = 0;

  // On trouve une valeur correcte à premierElement
  for (; i<= (sg.taille - 1) / 64 && sg.premierElement == -1; i++){
    sg.sommet[i] &= t.sommet[i];
    for (int j = 0; j<64; j++){
      if (sg.sommet[i] & (mask >> j)){
	
	if (sg.premierElement == -1){
	  sg.premierElement = 64 * i + j;
	  
	}
	sg.nombreElement++;
	
      }
      
    }
    
  }
  for (; i<= (sg.taille - 1) / 64; i++){
    sg.sommet[i] &= t.sommet[i];
    for (int j=0; j<64; j++){
      if (sg.sommet[i] & (mask >> j)){
	sg.nombreElement++;
	
      }
      
    }
    
  }
  
  return(true);
}

void selectSG(SousGraph& sg)
{
//selectioner un sous  graphe
  uint64_t mask = 0x80000000;
  for (int i=0; i < (sg.taille - 1) / 64; i++){
    sg.sommet[i] = 0xffffffffffffffff;
    
  }
  for (int i=0; i<= (sg.taille - 1) % 64; i++){
    sg.sommet[(sg.taille - 1)  / 64] |= (mask >> i);
    
  }
  sg.nombreElement = sg.taille;
  sg.premierElement = 0;

}

/*
 * copier le sous graphe 
 * sg est l'ensemble des sommets , t est le sous graphe à copier
 */
 
void sousGraph(SousGraph& sg, const SousGraph& t)
{
  int max_taille = (sg.taille > t.taille)?sg.taille:t.taille;
  sg.taille = t.taille;
  sg.premierElement = t.premierElement;
  sg.nombreElement = t.nombreElement;
  
  for (int i=sg.premierElement / 64; i< (max_taille + (64 - 1)) / 64; i++){
    sg.sommet[i] = t.sommet[i];
  }

}

void supprimer(SousGraph& sg, const int& v)
{
  uint64_t mask = 0x80000000;
  
  if (v < 0 || v >= sg.taille || !contient(sg, v)){
    return;
  }
  
  sg.sommet[v/64] &= ~(mask >> (v % 64));
  sg.nombreElement--;
  
  if (v == sg.premierElement){
    for (int i = sg.premierElement; i<sg.taille; i++){
      if (contient(sg, i)){
	sg.premierElement = i;
	return;
	}
      }
     
     sg.premierElement = -1;
  }
}

/**
 * prépare les paramètres d'un SubGraph, sg est l'ensemble de sommets de sous graph, s est la taille
 */
void tailleSousGraph(SousGraph& sg, uint64_t* tab, const int& s)
{
  sg.sommet = tab;
  for (int i = 0; i<((s + (64 - 1)) /64); i++){
  sg.sommet[i] = 0x0;
  }
  sg.taille = s;
  sg.nombreElement = 0;
  sg.premierElement = -1;
}

//tester si lensemble est vide
bool vide(const SousGraph& sg)
{
// return(v.premierElement ==0);
  return(sg.nombreElement == 0);

}

