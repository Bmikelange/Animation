/*
 * CalculsMSS.cpp :
 * Copyright (C) 2016 Florence Zara, LIRIS
 *               florence.zara@liris.univ-lyon1.fr
 *               http://liris.cnrs.fr/florence.zara/
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file CalculsMSS.cpp
Programme calculant pour chaque particule i d un MSS son etat au pas de temps suivant 
 (methode d 'Euler semi-implicite) : principales fonctions de calculs.
\brief Fonctions de calculs de la methode semi-implicite sur un systeme masses-ressorts.
*/ 

#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "vec.h"
#include "ObjetSimule.h"
#include "ObjetSimuleMSS.h"
#include "Viewer.h"

using namespace std;





/**
* Calcul des forces appliquees sur les particules du systeme masses-ressorts.
 */
void ObjetSimuleMSS::CalculForceSpring()
{
	/// f = somme_i (ki * (l(i,j)-l_0(i,j)) * uij ) + (nuij * (vi - vj) * uij) + (m*g) + force_ext
	
	/// Rq : Les forces dues a la gravite et au vent sont ajoutees lors du calcul de l acceleration
	for(unsigned int i=0;i< _SystemeMasseRessort->GetPartList().size();i++)
	{
		for(unsigned int j=0; j< _SystemeMasseRessort->GetParticule(i)->GetRessortList().size();j++)
		{
			Ressort *R=_SystemeMasseRessort->GetParticule(i)->GetRessortList()[j];
			double r=R->GetRaideur();
			Vector fs;
			if(R->GetParticuleA()->GetId()==(int)i)
			{
				double d=distance(Point(P[R->GetParticuleB()->GetId()]),Point(P[R->GetParticuleA()->GetId()]));
				Vector s=(Point(P[R->GetParticuleB()->GetId()])-Point(P[R->GetParticuleA()->GetId()]))/d;
				Vector fe=r*(d-R->GetLrepos())*s;

				Vector fv=dot(R->GetAmortissement()*(V[R->GetParticuleB()->GetId()]-V[R->GetParticuleA()->GetId()]),s)*s;
				fs=fe+fv;
			}
			else
			{
				double d=distance(Point(P[R->GetParticuleA()->GetId()]),Point(P[R->GetParticuleB()->GetId()]));
				Vector s=(Point(P[R->GetParticuleA()->GetId()])-Point(P[R->GetParticuleB()->GetId()]))/d;
				Vector fe=r*(d-R->GetLrepos())*s;

				Vector fv=dot(R->GetAmortissement()*(V[R->GetParticuleA()->GetId()]-V[R->GetParticuleB()->GetId()]),s)*s;
				fs=fe+fv;
			}
			if(length(fs)>40000)
			{
				_SystemeMasseRessort->GetParticule(i)->GetRessortList().erase(_SystemeMasseRessort->GetParticule(i)->GetRessortList().begin()+j);
			}
			Force[i]=Force[i]+fs;
		}
	}
		
}//void


/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleMSS::Collision()
{
    /// Arret de la vitesse quand touche le plan
    for(int i=0;i<_Nb_Sommets;i++)
	{
		if(P[i].y<=-10)
		{
			V[i]=Vector(0,0,0);
			P[i].y=-10;
		}
		
	}
    
}// void

void ObjetSimuleMSS::CollisionSphere(Point p,float n,Transform T)
{
    /// Arret de la vitesse quand touche le plan
    for(int i=0;i<_Nb_Sommets;i++)
	{
		if(length(P[i]-(Vector)T(p))<n)
		{
			V[i]=Vector(0,0,0);
		}
		
	}
    
}

