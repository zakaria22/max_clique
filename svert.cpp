/*
 * Copyright 2015 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "svert.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

/**

 * @fn copySVert
 * @brief Opérateur de copie de SVert
 * @param sv l'ensemble de sommets
 * @param t le SVert à copier
 */
void copySVert(SVert & sv, const SVert & t){
	int max_size = (sv.size > t.size)?sv.size:t.size;
	sv.size = t.size;
	sv.first = t.first;
	sv.num = t.num;
	for (int i=sv.first / Tab_SIZE; i< (max_size + (Tab_SIZE - 1)) / Tab_SIZE; i++){
		sv.set[i] = t.set[i];
	}
}


/**
 * @fn initSVert
 * @brief prépare les différents paramètres d'un SVert
 * @param sv l'ensemble de sommets
 * @param s la taille
 */
void initSVert(SVert & sv, Tab_TYPE * tab, const int & s){
	sv.set = tab;
	for (int i = 0; i<((s + (Tab_SIZE - 1)) /Tab_SIZE); i++){
		sv.set[i] = 0x0;
	}
	sv.size = s;
	sv.num = 0;
	sv.first = -1;
}


/**
 * @fn append
 * @brief Ajoute un élément à une liste de sommets
 * @param sv la liste des sommets
 * @param v sommet à ajouter
 */
void append(SVert & sv, const int & v){
	Tab_TYPE mask = MASK_FSTTO1;
	if (v < 0 || v >= sv.size || contains(sv, v)){
		return;
	}
	sv.set[v / Tab_SIZE] |= (mask >> (v % Tab_SIZE));
	sv.num++;
	if (v < sv.first || sv.first == -1){
		sv.first = v;
	}
}


/**
 * @fn remove
 * @brief Retire un élément de la liste de sommets
 * @param v sommet à retirer
 * @param sv la liste des sommets
 */
void remove(SVert & sv, const int & v){
	Tab_TYPE mask = MASK_FSTTO1;
	if (v < 0 || v >= sv.size || !contains(sv, v)){
		return;
	}
	sv.set[v/Tab_SIZE] &= ~(mask >> (v % Tab_SIZE));
	sv.num--;
	if (v == sv.first){
		for (int i = sv.first; i<sv.size; i++){
			if (contains(sv, i)){
				sv.first = i;
				return;
			}
		}
		sv.first = -1;
	}
}


/**
 * @fn isect
 * @brief Effectue l'intersection avec un ensemble de sommets
 * @param sv liste des sommets
 * @param t second ensemble de sommets
 * @return booléen qui indique si l'intersection est possible
 */
bool isect(SVert & sv, const SVert & t){
	if (t.size != sv.size || isEmpty(sv) || isEmpty(t)){
		return(false);
	}

	int i = sv.first / Tab_SIZE;	// Comme ce SVert est forcément non vide, first != -1
	Tab_TYPE mask = MASK_FSTTO1;

	sv.first = -1;
	sv.num = 0;

	// On trouve une valeur correcte à first
	for (; i<= (sv.size - 1) / Tab_SIZE && sv.first == -1; i++){
		sv.set[i] &= t.set[i];
		for (int j = 0; j<Tab_SIZE; j++){
			if (sv.set[i] & (mask >> j)){
				if (sv.first == -1){
					sv.first = Tab_SIZE * i + j;
				}
				sv.num++;
			}
		}
	}
	for (; i<= (sv.size - 1) / Tab_SIZE; i++){
		sv.set[i] &= t.set[i];
		for (int j=0; j<Tab_SIZE; j++){
			if (sv.set[i] & (mask >> j)){
				sv.num++;
			}
		}
	}

	return(true);
}


/**
 * @fn all
 * @brief Met tous les sommets possibles dans SVert
 * @param sv l'ensemble de sommets
 */
void all(SVert & sv){
	Tab_TYPE mask = MASK_FSTTO1;
	for (int i=0; i < (sv.size - 1) / Tab_SIZE; i++){
		sv.set[i] = MASK_ALLTO1;
	}
	for (int i=0; i<= (sv.size - 1) % Tab_SIZE; i++){
		sv.set[(sv.size - 1)  / Tab_SIZE] |= (mask >> i);
	}
	sv.num = sv.size;
	sv.first = 0;
}


/**
 * @fn disp
 * @brief Affiche le contenu de l'ensemble de sommet
 * @param sv l'ensemble de sommets
 * @param out le flux
 */
void disp(const SVert & sv, ostream & out){
	out << "[" << sv.num << " elements] ";
	for (int i=sv.first; i<sv.size; i++){
		if (contains(sv, i)){
		   out << i  << " ";
		}
	}
	out << endl;
}


/**
 * @fn isEmpty
 * @brief Indique si cet ensemble de sommets est vide
 * @param sv l'ensemble de sommets
 * @return true si cet ensemble est vide, false sinon
 */
bool isEmpty(const SVert & sv){
	return(sv.num == 0);	// Marche aussi avec (v.first == -1)
}


/**
 * @fn contains
 * @brief Indique si l'ensemble contient un sommet
 * @param sv l'ensemble de sommets
 * @param v le sommet à rechercher
 */
bool contains(const SVert & sv, const int & v){
	Tab_TYPE mask = MASK_FSTTO1;
	if (v < 0 || v >= sv.size){
		return(false);
	}
	return(sv.set[v / Tab_SIZE] & (mask >> (v % Tab_SIZE)));
}