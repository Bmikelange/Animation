/****************************************************************************
Copyright (C) 2010-2020 Alexandre Meyer

This file is part of Simea.

Simea is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Simea is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Simea.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include <vector>

#include "vec.h"

class Particle
{
public:

	Particle()
	{
		m_mass = 1.0;      // 1kg

		m_radius = 10 + rand() % 5;

		m_p.x = rand() % 400 - 200;
		m_p.y = m_radius + 5 + rand() % 100;
		m_p.z = rand() % 400 - 200;

	}

	void update(const float dt = 0.1f)		// advect
	{
		if (m_mass>0)
		{
			Vector temp=m_f;
			Vector a = m_f/m_mass;
			m_v = m_v + 0.01* a;
        	m_p = m_p + 0.01 * m_v;
			
			m_f=Vector(0,0,0);
			shooted=false;
		}
	}

	//! Collision with the ground (y=0)
	void groundCollision()
	{
		//if (m_radius < 0) return;
		if (m_p.y <= m_radius && !shooted)
		{
			m_v.y=0;
		}
	}

	//! Collision with any point p of radius radius (this will be used for kicking with the character's bones)
	void collision(const Point& p, const float radius)
	{
		float l=length(p-m_p);
		if (l<m_radius+radius)
		{
			Vector v= m_p-p;
			v.y+=(m_mass*9.81f);

			addForce(5*v);
			shooted=true;
		}
	}

	//! add force to the particles
	void addForce(const Vector& force)
	{
		m_f = m_f + force;
	}


	//! Apply gravity
	void addEarthGravity()
	{
		// apply gravity, call addForce
		addForce( Vector(0.f, -m_mass * 9.81f,0.f) );
	}

	const Point& position() const { return m_p; }
	float radius() const { return m_radius; }

	friend std::ostream& operator<<(std::ostream& o, const Particle& p)
	{
		o << " p=(" << p.m_p.x << "," << p.m_p.y << ") v=(" << p.m_v.x << "," << p.m_p.y << ") m=" << p.m_mass << std::endl;
		return o;
	}

	bool shooted=false;

protected:
	Point m_p;				//!< position
	float m_radius;			//!< radius
	Vector m_v;				//!< velocity m/s
	Vector m_f;				//!< force in N
	float m_mass;			//!< mass in kg
};


#endif
