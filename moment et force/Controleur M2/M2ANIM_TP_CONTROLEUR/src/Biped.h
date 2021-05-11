#ifndef BIPED_H
#define BIPED_H

#include "Box2D/Box2D.h"
#include "PDController.h"
#include "FSM.h"
#include <vector>


enum {PIED_GAUCHE,PIED_DROIT,JAMBE_GAUCHE,JAMBE_DROIT,CUISSE_GAUCHE,CUISSE_DROIT,TRONC,NB_CORPS};               // Les corps rigides
enum {CHEVILLE_GAUCHE,CHEVILLE_DROIT,GENOU_GAUCHE,GENOU_DROIT,HANCHE_GAUCHE,HANCHE_DROIT,NB_ARTICULATIONS};     // Les articulations

class Biped {

public:

	Biped (b2World* world);                     // Constructeur
	virtual	~Biped();                           // Destructeur

	void update(double Dt);                     // Mise � jour du bipede (controleur)

	bool hasFallen();                                               // Vrai si le bipede est tomb�

	b2Vec2 getCOM() const {return m_positionCOM;}                   // Retourne la position du CdM
	FSM* getStateMachine(){return m_stateMachine;}                  // Retourne la machine � �tats finis des poses cl�s
	PDController ** getPDControllers (){return m_PDControllers;}    // Retourne le tableau des r�gulateurs PD

	float sumTorque() const;                                        // Retourne la somme des carr�s des moments articulaires appliqu�s (pour l'optimisation)
	float sumAngleVelocity() const;                                 // Retourne la somme des vitesses angulaires absolues (pour l'optimisation)

	void setMachine(int i) { m_stateMachine = m_stateMachines[i];};

protected:

    b2World             *	m_world;                            // Le monde physique
	bool	                m_hasFallen;                        // Vrai si le bipede est tombe
	b2Vec2                  m_positionCOM;                      // La position du CdM du bipede
	b2Vec2                  m_velocityCOM;                      // La vitesse du CdM du bipede
	b2Body			    *	m_bodies[NB_CORPS];                 // Le tableau de corps rigides
	b2RevoluteJoint     *	m_joints[NB_ARTICULATIONS];		    // Le tableau d'articulations
	std::vector<float>      m_currentAnglesLocal;               // Les angles courants des articulations (locaux)
	std::vector<float>      m_currentAnglesGlobal;              // Les orientations courantes des corps rigides (globaux)
	float                   m_motorTarget[NB_ARTICULATIONS+1];  // Le tableau de moments articulaires � appliquer (+1 pour tronc)
	PDController		*	m_PDControllers[NB_ARTICULATIONS+1];// Le tableau des regulateurs PD (un par articulation +1 pour tronc)
	std::vector<FSM *> m_stateMachines;
	FSM * m_stateMachine;

    void computeCenterOfMass();                         // Calcul de la position et de la vitesse du CdM du bipede dans le repere du monde
    void KeyPoseTracking ();                            // Calcul des moments n�cessaires au suivi des poses cl�s

};

#endif
