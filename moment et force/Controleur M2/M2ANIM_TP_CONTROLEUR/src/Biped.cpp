#include "Biped.h"
#include <iostream>
using namespace std;

Biped::Biped (b2World* world) : m_world (world), m_hasFallen(false) { // Constructeur

		// Creation des corps rigides
		// ==========================

		// Proprietes communes
		b2BodyDef bodyDef;
		bodyDef.fixedRotation = false;
		bodyDef.allowSleep = false;
		bodyDef.awake = true;
		bodyDef.type = b2_dynamicBody;
		bodyDef.linearDamping = 0.01f;
		bodyDef.angularDamping = 0.01f;
        b2PolygonShape shape;
		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.filter.groupIndex = -1; // same group and don't collide

		// PIED GAUCHE
		bodyDef.position.Set(0.05f,0.05f); // 5cm au dessus du sol
		m_bodies[PIED_GAUCHE] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.1f, 0.05f); // boite de 20cm x 10cm
        fixture.density = 5.0f;
        fixture.friction = 0.999;
        fixture.userData = (void*)PIED_GAUCHE;
		m_bodies[PIED_GAUCHE]->CreateFixture(&fixture);

		// PIED DROIT
		bodyDef.position.Set(0.05f,0.05f); // 5cm au dessus du sol
		m_bodies[PIED_DROIT] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.1f, 0.05f); // boite de 20cm x 10cm
        fixture.density = 5.0f;
        fixture.userData = (void*)PIED_DROIT;
		m_bodies[PIED_DROIT]->CreateFixture(&fixture);

		// JAMBE GAUCHE
		bodyDef.position.Set(0.0f,0.25f); // 15 cm au dessus de cheville
		m_bodies[JAMBE_GAUCHE] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.05f, 0.15f); // boite de 10cm x 30cm
        fixture.density = 5.0f;
        fixture.userData = (void*)JAMBE_GAUCHE;
		m_bodies[JAMBE_GAUCHE]->CreateFixture(&fixture);

		// JAMBE DROIT
		bodyDef.position.Set(0.0f,0.25f); // 15 cm au dessus de cheville
		m_bodies[JAMBE_DROIT] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.05f, 0.15f); // boite de 10cm x 30cm
        fixture.density = 5.0f;
        fixture.userData = (void*)JAMBE_DROIT;
		m_bodies[JAMBE_DROIT]->CreateFixture(&fixture);

		// CUISSE GAUCHE
		bodyDef.position.Set(0.0f,0.55f); // 15 cm au dessus de genou
		m_bodies[CUISSE_GAUCHE] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.05f, 0.15f); // boite de 10cm x 30cm
        fixture.density = 5.0f;
        fixture.userData = (void*)CUISSE_GAUCHE;
		m_bodies[CUISSE_GAUCHE]->CreateFixture(&fixture);

		// CUISSE DROIT
		bodyDef.position.Set(0.0f,0.55f); // 15 cm au dessus de genou
		m_bodies[CUISSE_DROIT] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.05f, 0.15f); // boite de 10cm x 30cm
        fixture.density = 5.0f;
        fixture.userData = (void*)CUISSE_DROIT;
		m_bodies[CUISSE_DROIT]->CreateFixture(&fixture);

		// TRONC
		bodyDef.position.Set(0.0f,1.0f); // 30 cm au dessus de hanche
		m_bodies[TRONC] = m_world->CreateBody(&bodyDef);
        shape.SetAsBox(0.1f, 0.3f); // boite de 20cm x 60cm
        fixture.density = 4.0f;
        fixture.userData = (void*)TRONC;
		m_bodies[TRONC]->CreateFixture(&fixture);

		// Creation des articulations
		// ==========================

		// Proprietes communes
		b2RevoluteJointDef jointDef;
		jointDef.lowerAngle = -0.5f * b2_pi;
		jointDef.upperAngle = 0.5f * b2_pi;
		jointDef.enableLimit = true;

		// CHEVILLE GAUCHE
		jointDef.Initialize(m_bodies[PIED_GAUCHE],m_bodies[JAMBE_GAUCHE],m_bodies[JAMBE_GAUCHE]->GetWorldCenter()+b2Vec2(0.0f,-0.15f));
		m_joints[CHEVILLE_GAUCHE] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);

		// CHEVILLE DROIT
		jointDef.Initialize(m_bodies[PIED_DROIT],m_bodies[JAMBE_DROIT],m_bodies[JAMBE_DROIT]->GetWorldCenter()+b2Vec2(0.0f,-0.15f));
		m_joints[CHEVILLE_DROIT] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);

		// GENOU GAUCHE
		jointDef.Initialize(m_bodies[JAMBE_GAUCHE],m_bodies[CUISSE_GAUCHE],m_bodies[JAMBE_GAUCHE]->GetWorldCenter()+b2Vec2(0.0f,0.15f));
		jointDef.lowerAngle = 0.0;
		m_joints[GENOU_GAUCHE] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);

		// GENOU DROIT
		jointDef.Initialize(m_bodies[JAMBE_DROIT],m_bodies[CUISSE_DROIT],m_bodies[JAMBE_DROIT]->GetWorldCenter()+b2Vec2(0.0f,0.15f));
		m_joints[GENOU_DROIT] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);
		jointDef.lowerAngle = -0.5f * b2_pi;

		// HANCHE GAUCHE
		jointDef.Initialize(m_bodies[CUISSE_GAUCHE],m_bodies[TRONC],m_bodies[CUISSE_GAUCHE]->GetWorldCenter()+b2Vec2(0.0f,0.15f));
		m_joints[HANCHE_GAUCHE] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);

		// HANCHE DROIT
		jointDef.Initialize(m_bodies[CUISSE_DROIT],m_bodies[TRONC],m_bodies[CUISSE_DROIT]->GetWorldCenter()+b2Vec2(0.0f,0.15f));
		m_joints[HANCHE_DROIT] = (b2RevoluteJoint*) m_world->CreateJoint(&jointDef);
		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0);

		m_currentAnglesLocal.push_back(0.0);
		m_currentAnglesGlobal.push_back(0.0); // pour l'angle du tronc

		// PD Controleurs
		// ==============

	    float KpKvRatio = 0.83;
		float KpCheville = 2.32;
		float KpGenou = 4.43;
		float KpHanche = 9.95;
		float KpTronc = 11.89;
        m_PDControllers[CHEVILLE_GAUCHE] = new PDController(KpCheville,KpKvRatio*sqrt(KpCheville));
        m_PDControllers[CHEVILLE_DROIT] = new PDController(KpCheville,KpKvRatio*sqrt(KpCheville));
        m_PDControllers[GENOU_GAUCHE] = new PDController(KpGenou,KpKvRatio*sqrt(KpGenou));
        m_PDControllers[GENOU_DROIT] = new PDController(KpGenou,KpKvRatio*sqrt(KpGenou));
        m_PDControllers[HANCHE_GAUCHE] = new PDController(KpHanche,KpKvRatio*sqrt(KpHanche));
        m_PDControllers[HANCHE_DROIT] = new PDController(KpHanche,KpKvRatio*sqrt(KpHanche));
        m_PDControllers[NB_ARTICULATIONS] = new PDController(KpTronc,KpKvRatio*sqrt(KpTronc)); // TRONC

        // Finite State Machine
        // ====================
        m_stateMachines.push_back(new FSM_Stand());
        m_stateMachines.push_back(new FSM_Walk());
		m_stateMachines.push_back(new FSM_FIGHT());
		m_stateMachines.push_back(new FSM_JUMP());
		m_stateMachines.push_back(new FSM_Walk_Inv());
		m_stateMachine=m_stateMachines[0];
}

Biped::~Biped() { // Destructor
    for (int i = 0; i <= NB_ARTICULATIONS; ++i) {
        if (m_PDControllers[i]!=NULL) {delete m_PDControllers[i]; m_PDControllers[i] = NULL;}
    }
	for(unsigned int i=0;i<m_stateMachines.size();i++)
	{
		delete m_stateMachines[i];
	}
}

void Biped::update(double Dt) {
    // Remise � zero des moments articulaires
    for (int j = 0; j <= NB_ARTICULATIONS; ++j) m_motorTarget[j] = 0.0;

    // Mise � jour de la position et de la vitesse du COM
    computeCenterOfMass();

    if (!hasFallen()) { // Teste si le bip�de est tomb�
        // Mise � jour de l'�tat dans la machine si condition remplie
        m_stateMachine->update(Dt,m_currentAnglesLocal,m_currentAnglesGlobal);
        // Calcul des moments n�cessaires au suivi des poses cl�s
        KeyPoseTracking();

        // Application des moments
        for (int j = 0; j <= NB_ARTICULATIONS; ++j) m_bodies[j]->ApplyTorque(m_motorTarget[j],true);
    }
}

bool Biped::hasFallen() {
    // vrai si d�ja � terre (impossible de se remettre debout)
	if (m_hasFallen) return m_hasFallen;

	// d�tection que le bip�de est tomb� : le CdM du tronc est au niveau des genoux
	m_hasFallen = m_bodies[TRONC]->GetWorldCenter().y < 0.4;

	return m_hasFallen;
}

float Biped::sumTorque() const {
    // Retourne la somme des carr�s des moments articulaires appliqu�s (pour l'optimisation)
    float sum = 0;
    for (int j = 0; j <= NB_ARTICULATIONS; ++j) sum += m_motorTarget[j]*m_motorTarget[j];
    return sum;
}

float Biped::sumAngleVelocity() const {
    // Retourne la somme des vitesses angulaires absolues (pour l'optimisation)
    float sum = 0;
    for (int j = 0; j < NB_ARTICULATIONS; ++j) sum += fabs(m_joints[j]->GetJointSpeed());
    return sum;
}

//====================== PRIVATE ============================//

void Biped::computeCenterOfMass() {
    // Calcul de la position et de la vitesse du CdM du bipede dans le repere du monde
    float32 total_mass = 0.0f;
    m_velocityCOM = m_positionCOM;
    m_positionCOM.SetZero();
    for (int i = 0; i < NB_CORPS; ++i) {
        float32 massBody = m_bodies[i]->GetMass();
        b2Vec2 comBody = m_bodies[i]->GetWorldCenter();
        m_positionCOM += massBody * comBody;
        total_mass += massBody;
    }
    m_positionCOM = (1.0f / total_mass) * m_positionCOM;
    m_velocityCOM = m_positionCOM - m_velocityCOM;
}

void Biped::KeyPoseTracking () {
    // R�cup�ration des cibles et de l'information local/global
    std::vector<float> targetAngles = m_stateMachine->getCurrentTargetAngles();
    std::vector<bool> targetLocal = m_stateMachine->getCurrentTargetLocal();

    // Pour toutes les articulations
    for (int j = 0; j <= NB_ARTICULATIONS; ++j) {
        // Lit la cible pour l'articulation j dans targetAngles
        float targetAngle = targetAngles[j];
        // Affecte la cible au r�gulateur PD par setTarget
        m_PDControllers[j]->setTarget(targetAngle);
        // Mise � jour de m_currentAnglesLocal par b2RevoluteJoint::GetJointAngle() (attention au signe et attention pour j==NB_ARTICULATIONS pas d'angle local)
        if (j!=NB_ARTICULATIONS) m_currentAnglesLocal[j] = -m_joints[j]->GetJointAngle();
        else m_currentAnglesLocal[j] = 0.0;
        // Mise � jour de m_currentAnglesGlobal par b2Body::GetTransform().q.GetAngle() avec l'�quivalence d'indice d'articulation et de corps rigide (cf. �num�rations)
        m_currentAnglesGlobal[j] = m_bodies[j]->GetTransform().q.GetAngle();
        // Calcul du moment � ajouter dans m_motorTarget gr�ce au r�gulateur PD et en fonction de si la cible est locale ou globale
        if (targetLocal[j]) m_motorTarget[j] += m_PDControllers[j]->compute(m_currentAnglesLocal[j]);
        else m_motorTarget[j] += m_PDControllers[j]->compute(m_currentAnglesGlobal[j]);

    }
}

