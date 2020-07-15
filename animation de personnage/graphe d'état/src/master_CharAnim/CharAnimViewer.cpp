
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>

#include "CharAnimViewer.h"
#include "CharacterController.h"

using namespace std;
using namespace chara;


CharAnimViewer* CharAnimViewer::psingleton = NULL;


CharAnimViewer::CharAnimViewer() : Viewer(), m_frameNumber(0)
{
	psingleton = this;
}


int CharAnimViewer::init()
{
    Viewer::init();
    cout<<"==>master_CharAnim/CharAnimViewer"<<endl;
    m_camera.lookat( Point(0,0,0), 1000 );
	m_camera.rotation(180, 0);
    gl.light( Point(300, 300, 300 ) );

    //b_draw_grid = false;

    m_world.setParticlesCount( 10 );


    init_cylinder();
    init_sphere();

    m_bvh.init("data/bvh/motionGraph_second_life/avatar_stand.bvh" );
    m_bvh1.init("data/bvh/motionGraph_second_life/avatar_walk.bvh" );
	m_bvh2.init("data/bvh/motionGraph_second_life/avatar_run.bvh" );
	m_bvh3.init("data/bvh/motionGraph_second_life/avatar_kick_roundhouse_R.bvh" );
	m_bvh4.init("data/bvh/motionGraph_second_life/avatar_drink.bvh" );
	m_bvh5.init("data/bvh/motionGraph_second_life/avatar_punch_onetwo.bvh" );
	m_bvh6.init("data/bvh/motionGraph_second_life/avatar_clap.bvh" );
	m_bvh7.init("data/bvh/motionGraph_second_life/avatar_jump.bvh" );
	m_bvh8.init("data/bvh/motionGraph_second_life/avatar_backflip.bvh" );
	m_bvh9.init("data/bvh/motionGraph_second_life/avatar_blowkiss.bvh" );
	m_bvh10.init("data/bvh/motionGraph_second_life/avatar_bow.bvh" );
	m_bvh11.init("data/bvh/motionGraph_second_life/avatar_hello.bvh" );

	m_world.setParticlesCount( 10 );
	//m_bvh.init( smart_path("data/bvh/danse.bvh") );

    m_frameNumber = 0;
    cout<<endl<<"========================"<<endl;
    cout<<"BVH decription"<<endl;
    cout<<m_bvh<<endl;
    cout<<endl<<"========================"<<endl;

    m_ske.init( m_bvh );
	m_skePrincipal=&m_ske;


    return 0;
}

void CharAnimViewer::draw_skeleton_transform(const Skeleton& s,Transform T)
{
	for(int i=0;i<s.numberOfJoint();i++)
	{
		int pi=s.getParentId(i);
		Point	p1=T(s.getJointPosition(i));
		if(pi != -1)
		{
			Point p2 =T(s.getJointPosition(pi));
			draw_cylinder(p1,p2,2);
		}
    		draw_sphere(p1,3);
	}
}


void CharAnimViewer::draw_skeleton(const Skeleton& s)
{
	for(int i=0;i<s.numberOfJoint();i++)
	{
		int pi=s.getParentId(i);
		if(pi != -1)
			draw_cylinder(s.getJointPosition(i),s.getJointPosition(pi),1);
    	draw_sphere(s.getJointPosition(i),1);
	}
}



int CharAnimViewer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw_quad( RotationX(-90)*Scale(500,500,1) );

    Viewer::manageCameraLight();
    gl.camera(m_camera);

	// Affiche les particules physiques (Question 3 : interaction personnage sphere/ballon)
    //m_world.draw();

	// Affiche le skeleton � partir de la structure lin�aire (tableau) Skeleton
    //draw_skeleton( m_ske );

	//draw_sphere(cp.getCh2w()*Scale(10,10,10));

	draw_skeleton_transform(*m_skePrincipal,cp.getCh2w());
	m_world.draw();








#if 0			// exercice du cours
	float A[] = { 0, -10, -20, -30, -40 };
	float B[] = { 0, -10, -20, -30, -40 };
	float C[] = { 0, -10, -20, -30, -40 };
	float D[] = { 0, 10, 20, 30, 40 };
	float a, b, c, d;

	static int t = 3;
	//if (t == 4) t = 0; else t++;

	a = A[t];
	b = B[t];
	c = C[t];
	d = D[t];

	Transform scaleS = Scale(12, 12, 12);
	Transform scaleA = Scale(20, 20, 20);
	Transform scale = Scale(10, 100, 10);
	Transform A2W = RotationZ(a);
	draw_cylinder(A2W*scale);
	draw_sphere(A2W*scaleS);
	draw_axe(A2W*scaleA);

	Transform B2A = Translation(0, 100, 0) * RotationZ(b);
	Transform B2W = A2W * B2A;
	draw_cylinder(B2W*scale);
	draw_sphere(B2W*scaleS);
	draw_axe(B2W*scaleA);

	Transform scaleP = Scale(5, 50, 5);
	Transform C2B = Translation(0, 100, 0) * RotationZ(c);
	Transform C2W = B2W * C2B;
	draw_cylinder(C2W*scaleP);
	draw_sphere(C2W*scaleS);
	draw_axe(C2W*scaleA);

	Transform D2B = Translation(0, 100, 0) * RotationZ(d);
	Transform D2W = B2W * D2B;
	draw_cylinder(D2W*scaleP);
	draw_axe(D2W*scaleA);
	draw_sphere(D2W*scaleS);
#endif


    return 1;
}


int CharAnimViewer::update( const float time, const float delta )
{
    // time est le temps ecoule depuis le demarrage de l'application, en millisecondes,
    // delta est le temps ecoule depuis l'affichage de la derniere image / le dernier appel a draw(), en millisecondes.

	//if (key_state('n')) { m_frameNumber++; cout << m_frameNumber << endl; clear_key_state('n');}
	//if (key_state('b')) { m_frameNumber--; cout << m_frameNumber << endl; clear_key_state('b');}

	//m_ske.setPose( m_bvh, m_frameNumber );
	static int i=-1;
	static float ds=0;
	ds=delta+ds;

	for(int n=0;n<m_skePrincipal->numberOfJoint();n++)
	{
		m_world.collision(cp.getCh2w()(m_skePrincipal->getJointPosition(n)),5);
	}
	if(key_state('x'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh3.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh3, i);
		}

	}
	else if(key_state('i'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh8.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh8, i);

		}
	}
	else if(key_state('u'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh9.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh9, i);

		}
	}
	else if(key_state('m'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh10.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh10, i);

		}
	}
	else if(key_state('l'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh11.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh11, i);

		}
	}
	else if(key_state('o'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh7.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh7, i);

		}
	}
	else if(key_state('p'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh6.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh6, i);

		}
	}
	else if(key_state('t'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh5.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh5, i);

		}
	}
	else if(key_state('r'))
	{
		if(ds>40)
		{
			ds=0;
			i++;
			if(i>=m_bvh4.getNumberOfFrame())
			{
				i=0;
			}
			m_skePrincipal->setPose( m_bvh4, i);

		}
	}
    else if(key_state('z') && !key_state('s'))
    {
		if(ds>60)
		{
			ds=0;
			i++;
			if(key_state('f'))
			{
				if(i>=m_bvh2.getNumberOfFrame())
				{
					i=0;
				}
				m_skePrincipal->setPose( m_bvh2, i);
			}
			else
			{
				if(i>=m_bvh1.getNumberOfFrame())
				{
					i=0;
				}
				m_skePrincipal->setPose( m_bvh1, i);
			}
		}
    }
    else if(key_state('s') && !key_state('z'))
    {
		if(ds>60)
		{
			ds=0;
			i++;
			if(key_state('f'))
			{
				if(i>=m_bvh2.getNumberOfFrame())
				{
					i=0;
				}
				m_skePrincipal->setPose( m_bvh2, i);
			}
			else
			{
				if(i>=m_bvh1.getNumberOfFrame())
				{
					i=0;
				}
				m_skePrincipal->setPose( m_bvh1, i);
			}
		}
        
    }
    else
    {
		i=0;
		m_skePrincipal->setPose( m_bvh, i);
    }


    m_world.update(0.1f);

	cp.update(m_bvh.getFrameTime());

    return 0;
}



