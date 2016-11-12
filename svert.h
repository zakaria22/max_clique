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

#ifndef SVERT_H
#define SVERT_H

#include <cstdint>
#include <ostream>


/* reconnaissance de l'architecture */
#if _WIN64
#define ARCH_64
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ARCH_64
#endif
#endif

/* definition de la taille des "cellules" en fonction de l'architecture */
#if defined(ARCH_64)
  #define Tab_SIZE 64
  #define Tab_TYPE uint64_t
  #define MASK_ALLTO1 0xffffffffffffffff
  #define MASK_FSTTO1 0x8000000000000000

#else
  #warning "Cannot guess architecture... Using 8-bit"
  #define Tab_SIZE 8
  #define Tab_TYPE uint8_t
  #define MASK_ALLTO1 0xff
  #define MASK_FSTTO1 0x80
#endif

#define T_SIZE(i) (i + (Tab_SIZE - 1)) / Tab_SIZE

typedef struct{
  int size;// Nombre maximal d'éléments de cet ensemble
  int num;// Nombre d'éléments dans l'ensemble
  int first;// Le premier élément de l'ensemble
  Tab_TYPE  * set;// set[i] correspond aux sommets de 8i à 8i + 7
}SVert;

/* Copie de SVert */
void copySVert(SVert & sv, const SVert & t);
/* Réglage de la taille */
void initSVert(SVert & sv, Tab_TYPE * tab, const int & s);
/* Manipulation des éléments */
void append(SVert & sv, const int & v);
void remove(SVert & sv, const int & v);
bool isect(SVert & sv, const SVert & t);
void all(SVert & sv);
void disp(const SVert & v, std::ostream & out);
bool isEmpty(const SVert & sv);
bool contains(const SVert & sv, const int & v);

#endif // SVERT_H
