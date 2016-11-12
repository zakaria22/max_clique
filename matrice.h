/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (vect) 2015  <copyright holder> <email>
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

#ifndef MATRICE_H
#define MATRICE_H
#include <cstdio>
#include <vector>

#include "sousgraph.h"

class Matrice
{
private:
  int sommet;
  bool ** matrice;

public:
  Matrice(const char * filename);
  ~Matrice();
  //Methodes needed
  void afficher();
  void adjacence(const int & value, SousGraph & sg) const;
  int taille() const;

};

//faire sortir le numero de sommet edpuis le fichier de données
int numSommet(FILE ** f);

SousGraph cliqueMax(const Matrice & m, SousGraph & maximumC);
//tester si on peut pas faire mieu
bool maxiC(SousGraph nombre, SousGraph nombreFinal, SousGraph & maximumC, const Matrice & m, int value, std::vector<SousGraph> vect);

#endif // MATRICE_H
