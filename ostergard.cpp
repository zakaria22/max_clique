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

#include "ostergard.h"
#include <algorithm>
#include <iostream>

using namespace std;

SVert ostergard(const AdjMat & m, SVert & max){
	vector<SVert> C(m.getSize());
	Tab_TYPE C_tab[m.getSize()][T_SIZE(m.getSize())];
	SVert res;
	Tab_TYPE res_tab[T_SIZE(m.getSize())];
	initSVert(res, res_tab, m.getSize());

	for (int i=0; i<m.getSize(); i++){
		initSVert(C[i], C_tab[i], m.getSize());
		all(C[i]);
	}

	for (int i=m.getSize() - 1; i>=0; i--){
		SVert cur, U;
		Tab_TYPE cur_tab[T_SIZE(m.getSize())], U_tab[T_SIZE(m.getSize())];
		initSVert(cur, cur_tab, m.getSize());
		initSVert(U, U_tab, m.getSize());

		for (int j=i; j < m.getSize(); j++){
			append(U, j);
		}
		// cerr << "¤ Sommet " << i << endl;
		o_clique(U, cur, max, m, i, C);
		C[i] = max;
	}

	copySVert(max, C[0]);

	return(max);
}


bool o_clique(SVert U_old, SVert cur_old, SVert & max, const AdjMat & m, int v, vector<SVert> C){
	int i;

	SVert U, cur;
	Tab_TYPE U_tab[T_SIZE(m.getSize())], cur_tab[T_SIZE(m.getSize())];
	initSVert(U, U_tab, m.getSize());
	initSVert(cur, cur_tab, m.getSize());

	copySVert(U, U_old);
	copySVert(cur, cur_old);

	SVert adj;
	Tab_TYPE adj_tab[T_SIZE(m.getSize())];
	initSVert(adj, adj_tab, m.getSize());
	m.getAdj(v, adj);
	isect(U, adj);
	append(cur, v);


	if (isEmpty(U)){
		if (cur.num > max.num){
			copySVert(max, cur);
			disp(max, cout);
			return(true);
		}
	}

	while (!isEmpty(U)){
		if (cur.num + U.num <= max.num){
			return(false);
		}
		i = U.first;
		if (cur.num + C[i].num <= max.num){
			return(false);
		}
		remove(U, i);
		if (o_clique(U, cur, max, m, i, C)){
			return(true);
		}
	}

	return(false);
}