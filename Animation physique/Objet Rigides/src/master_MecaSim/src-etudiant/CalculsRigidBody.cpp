/*
 * CalculsRigidBody.cpp :
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
#include "ObjetSimuleRigidBody.h"
#include "Viewer.h"

using namespace std;




/*
 * Calcul de la masse de l objet rigide
 */
void ObjetSimuleRigidBody::CalculMasse()
{
    _Mass = 0.f;
    _BaryCentre = Vector(0,0,0);
    for(int i = 0 ; i < _Nb_Sommets; i++)
        _Mass += M[i];
    for(int i = 0 ; i < _Nb_Sommets; i++)  
        _BaryCentre = _BaryCentre + P[i] * M[i];
    _BaryCentre = _BaryCentre / _Mass;

}


/*
 * Calcul du tenseur d inertie de l objet rigide - - partie constante Ibody
 * et remplissage tableau roi (position particules dans repere objet)
 */
void ObjetSimuleRigidBody::CalculIBody()
{
    for (int i = 0; i < _Nb_Sommets; i++)
    {
        _ROi.push_back(P[i] - _BaryCentre);
    }  

    for( int i = 0; i < _Nb_Sommets; i++)
    {
        double rtr = _ROi[i].x * _ROi[i].x + _ROi[i].y * _ROi[i].y + _ROi[i].z * _ROi[i].z;
        _Ibody += M[i] * ( rtr * Matrix::UnitMatrix() -  MultiplyTransposedAndOriginal(_ROi[i]) );
    }
    _IbodyInv = _Ibody;
    _IbodyInv.Inverse();
    
}


/*
 * Calcul de l etat de l objet rigide.
 */
void ObjetSimuleRigidBody::CalculStateX()
{
    _InertieTenseurInv    = _Rotation * _IbodyInv * _Rotation.TransposeConst();

    _VitesseAngulaire = _InertieTenseurInv * _MomentCinetique; 
}



/*
 * Calcul de la derivee de l etat : d/dt X(t).
 */
void ObjetSimuleRigidBody::CalculDeriveeStateX(Vector gravite)
{
    _Vitesse = _QuantiteMouvement / _Mass;
    _RotationDerivee = StarMatrix(_VitesseAngulaire) * _Rotation;
    _Force = gravite * _Mass;
    _Torque = Vector();
    for(int i = 0; i < _Nb_Sommets; i++)
    {
        Vector ri = _Rotation * _ROi[i] + _BaryCentre;
        _Torque = _Torque + cross((ri - _BaryCentre), _Force);
    }


}


/**
 * Schema integration pour obbtenir X(t+dt) en fct de X(t) et d/dt X(t)
 */
void ObjetSimuleRigidBody::Solve(float visco)
{
    _Position          = _Position + _delta_t * _Vitesse;
    _Rotation          = _Rotation + _delta_t*_RotationDerivee;
    _QuantiteMouvement = _QuantiteMouvement + _delta_t*_Force;
    _MomentCinetique   = _MomentCinetique + _delta_t*_Torque;    
    
    for( int i = 0 ; i < _Nb_Sommets; i++)
        P[i] = _Position + _BaryCentre + (_Rotation * _ROi[i]);
}



/**
 * Gestion des collisions avec le sol.
 */
void ObjetSimuleRigidBody::Collision()
{
    double height = 0.0;
    bool coll = false;
    double dist = 0.0;
    int sommetc = -1;

    for(int i = 0; i < _Nb_Sommets; i++)
    {
        if (P[i].y < height)
        {
            coll = true;
            double tmp = distance2(Point(P[i].x,P[i].y,P[i].z),
                                  Point(P[i].x,height,P[i].z));

            if(dist < tmp)
            {
                dist = tmp;
                sommetc = i;
            }
        }
    }

    if(coll)
    {
        _Position.y += dist;
        Vector ri = _Rotation * _ROi[sommetc] + _BaryCentre;
        _MomentCinetique = cross(ri ,-_Force) + _MomentCinetique;
        _QuantiteMouvement = -0.7 * _QuantiteMouvement;
    }

}// void

bool Inside(const Vector& p, const Vector& c, const float & r)
{
  Vector q = c - p;
  return (dot(q,q) < r * r) ;
}

int inferior(const Vector& u, const Vector& v)
{
  return ((u.x < v.x) && (u.y < v.y) && (u.z < v.z));
}

int superior(const Vector& u, const Vector& v)
{
  return ((u.x > v.x) && (u.y > v.y) && (u.z > v.z));
}

bool Inside(const Vector& p,const Vector& pmin, const Vector& pmax) 
{
  return (inferior(pmin,p) && superior(pmax,p));
}

void ObjetSimuleRigidBody::CollisionSphere(const Point & c,float n,Transform T)
{
    bool coll = false;
    double dist = 0.0;
    int sommetc = -1;
    Point Tc=T(c);
    Vector dir = Vector(0,0,0);
    for(int i=0;i<_Nb_Sommets;i++)
	{
		if(Inside(P[i],(Vector)Tc,n))
		{
			coll = true;
            double tmp = n*n-distance2(Point(P[i].x,P[i].y,P[i].z),Tc);
            if(tmp<0.01)
                coll=false;
            if(dist < tmp)
            {
                dir = P[i]-(Vector)Tc;
                dist = tmp;
                sommetc = i;
            }
		}
		
	}
    if(coll)
    {
        Vector ri = _Rotation * _ROi[sommetc] + _BaryCentre;
        _MomentCinetique = cross(ri,normalize(dir)) + _MomentCinetique;
        _Position = _Position+normalize(dir)*dist;
        _QuantiteMouvement = -0.7 * _QuantiteMouvement;
    }
    
}

void ObjetSimuleRigidBody::CollisionCube(const Point & pmin,const Point & pmax,Transform T)
{
    bool coll = false;
    double dist = 0.0;
    int sommetc = -1;
    int dirIndex=0;
    Vector dir[6]={Vector(-1,0,0),Vector(0,-1,0),Vector(0,0,-1),Vector(1,0,0),Vector(0,1,0),Vector(0,0,1)};
    Point Tpmin=T(pmin);
    Point Tpmax=T(pmax);
    for(int i=0;i<_Nb_Sommets;i++)
	{
		if(Inside(P[i],(Vector)Tpmin,(Vector)Tpmax))
		{
			coll = true;
            double val[6]={(P[i].x-Tpmin.x)*(P[i].x-Tpmin.x),(P[i].y-Tpmin.y)*(P[i].y-Tpmin.y),
                           (P[i].z-Tpmin.z)*(P[i].z-Tpmin.z),(P[i].x-Tpmax.x)*(P[i].x-Tpmax.x),
                           (P[i].y-Tpmax.y)*(P[i].y-Tpmax.y),(P[i].z-Tpmax.z)*(P[i].z-Tpmax.z)};
            double tmp=100000000;
            int indice=0;
            for(int j=0;j<6;j++)
            {
                if(val[j]<tmp)
                {
                    tmp=val[j];
                    indice=j;
                }
            }
            //évites de détecter la collision si elle est trop faible
            if(tmp<0.0001)
                coll=false;
            if(dist < tmp)
            {
                dirIndex=indice;
                dist = tmp;
                sommetc = i;
            }
		}
		
	}
    if(coll)
    {
        Vector ri = _Rotation * _ROi[sommetc] + _BaryCentre;
        _MomentCinetique = cross(ri,normalize(dir[dirIndex]))+_MomentCinetique;
        _Position = _Position+normalize(dir[dirIndex])* dist;
        _QuantiteMouvement = -0.7 * _QuantiteMouvement;
    }
    
}