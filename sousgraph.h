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

#ifndef SOUSGRAPH_H
#define SOUSGRAPH_H

#include <cstdint>
#include <ostream>


#define T_SIZE(i) (i + (31)) / 32



struct SousGraph
{
  int taille;
  int nombreElement;
  int premierElement;
  uint64_t  * sommet;
};
/* Copie de SubGraph */
void sousGraph(SousGraph & sg, const SousGraph & t);
/* Réglage de la taille */
void tailleSousGraph(SousGraph & sg, uint64_t * tab, const int & s);
/* Manipulation des éléments */
void ajouter(SousGraph & sg, const int & v);
void supprimer(SousGraph & sg, const int & v);
bool inter(SousGraph & sg, const SousGraph & t);
void selectSG(SousGraph & sg);
void afficher(const SousGraph & v, std::ostream & out);
bool vide(const SousGraph & sg);
bool contient(const SousGraph & sg, const int & v);

#endif // SOUSGRAPH_H
