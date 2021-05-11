#include "FSM.h"
#include <iostream>
using namespace std;

// Interpolation lin�aire :
// Doit retourner la valeur dans l'intervalle [x1,x2] qui correspond au placement de y1 dans l'intervalle [0,y2]
float linear_interpolate (float x1, float x2, float y1, float y2) {
    if (y1<=0) return x1;
    if (y1>=y2) return x2;
    float p = y1 / y2;
    return (1-p) * x1 + p * x2;
}

FSM::FSM() { }

void FSM::update(double Dt, std::vector<float> currentAnglesLocal, std::vector<float> currentAnglesGlobal) {
    // Mise � jour du temps �coul� dans l'�tat
    m_timeInState += Dt;
    // L'�tat courant
    State s = m_states[m_currentState];
    // Si la machine a un seul �tat on reste dedans
    if (m_nbStates==1) return;
    // Transition si dur�e �coul�e
    if (m_timeInState > s.transitionTime) {
        // Remise � z�ro du temps �coul�
        m_timeInState = 0.0;
        // R�cup�ration des angles courants en tant que point de d�part de l'�tat (animation fluide)
        for (unsigned int i=0;i<s.targetAngles.size();i++) {
                if (m_states[s.nextState].targetLocal[i]) m_anglesAtTransition[i] = currentAnglesLocal[i];
                else m_anglesAtTransition[i] = currentAnglesGlobal[i];
        }
        // Passage � l'�tat suivant
        m_currentState = s.nextState;
    }
}

std::vector<float> FSM::getCurrentTargetAngles() const {
    // Poses cl�s non interpol�es si un seul �tat ou transition directe
    if (m_nbStates==1 || m_states[m_currentState].transitionTime==0.0) return m_states[m_currentState].targetAngles;

    // Poses cl�s interpol�es sinon
    std::vector<float> targetAnglesInterpolated;
    float y1 = m_timeInState; // dur�e �coul�e depuis le d�but de l'�tat courant
    float y2 = m_states[m_currentState].transitionTime;  // dur�e max avant transition
    for (unsigned int i=0; i<m_states[m_currentState].targetAngles.size();i++) {
        float x1 = m_anglesAtTransition[i]; // l'angle au d�but de l'�tat
        float x2 = m_states[m_currentState].targetAngles[i]; // la cible courante
        if (x1==x2) targetAnglesInterpolated.push_back(x1); // pas d'interpolation si identiques
        else targetAnglesInterpolated.push_back(linear_interpolate(x1,x2,y1,y2)); // interpolation lin�aire sinon
    }
    return targetAnglesInterpolated;
}

FSM_Stand::FSM_Stand() {
    // La machine � �tats finis pour un mouvement stable debout
    // un seul �tat, toutes les articulations � z�ro dans le monde
    m_nbStates = 1;
    m_currentState = 0;
    State s;
    // ETAT 0 //
    s.ID = 0;
    s.nextState = 0;
    s.transitionTime = 0.0;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
}

FSM_Walk::FSM_Walk() {
    m_nbStates = 6;
    m_currentState = 0;
    State s;
    // ETAT 0 //
    s.ID = 0;
    s.nextState = 1;
    s.transitionTime = 0.08;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(-1.53); s.targetLocal.push_back(true);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.48); s.targetLocal.push_back(true);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 1 //
    s.ID = 1;
    s.nextState = 2;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(1.12); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 2 //
    s.ID = 2;
    s.nextState = 3;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(-0.56); s.targetLocal.push_back(true); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_GAUCHE
    s.targetAngles.push_back(-0.13); s.targetLocal.push_back(true); //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_GAUCHE
    s.targetAngles.push_back(-0.23); s.targetLocal.push_back(true); //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 3 //
    s.ID = 3;
    s.nextState = 4;
    s.transitionTime = 0.08;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(-1.53); s.targetLocal.push_back(true);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.48); s.targetLocal.push_back(true);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 4 //
    s.ID = 4;
    s.nextState = 5;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(1.12); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 5 //
    s.ID = 5;
    s.nextState = 0;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(-0.56); s.targetLocal.push_back(true); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_DROIT
    s.targetAngles.push_back(-0.13); s.targetLocal.push_back(true); //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_DROIT
    s.targetAngles.push_back(-0.23); s.targetLocal.push_back(true); //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // copie des premi�res valeurs dans m_anglesAtTransition
    for (unsigned int i=0;i<s.targetAngles.size();i++)
        m_anglesAtTransition.push_back(m_states[m_currentState].targetAngles[i]);

}


FSM_FIGHT::FSM_FIGHT() {

    m_nbStates = 6;
    m_currentState = 0;
    State s;
    // ETAT 0 // B
    s.ID = 0;
    s.nextState = 1;
    s.transitionTime = 0.20;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(-0.20); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // ETAT 1 // L
    s.ID = 1;
    s.nextState = 2;
    s.transitionTime = 0.10;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.2); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(-1.2); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(1.2); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // ETAT 2 // L2
    s.ID = 2;
    s.nextState = 3;
    s.transitionTime = 0.30;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.2); s.targetLocal.push_back(false); //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_GAUCHE
    s.targetAngles.push_back(1.53); s.targetLocal.push_back(false); //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // ETAT 3 // C
    s.ID = 3;
    s.nextState = 4;
    s.transitionTime = 0.50;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(3.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.2); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // ETAT 4 // L2
    s.ID = 4;
    s.nextState = 5;
    s.transitionTime = 0.10;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.2); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(1.53); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.2); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // ETAT 5 // L
    s.ID = 5;
    s.nextState = 0;
    s.transitionTime = 0.30;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.2); s.targetLocal.push_back(false); //GENOU_GAUCHE
    s.targetAngles.push_back(-1.2); s.targetLocal.push_back(false); //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_GAUCHE
    s.targetAngles.push_back(1.2); s.targetLocal.push_back(false); //HANCHE_DROIT
    s.targetAngles.push_back(-0.1); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // copie des premi�res valeurs dans m_anglesAtTransition
    for (unsigned int i=0;i<s.targetAngles.size();i++)
        m_anglesAtTransition.push_back(m_states[m_currentState].targetAngles[i]);

}


FSM_Walk_Inv::FSM_Walk_Inv() {
    m_nbStates = 6;
    m_currentState = 0;
    State s;
    // ETAT 0 //
    s.ID = 0;
    s.nextState = 1;
    s.transitionTime = 0.08;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(1.53); s.targetLocal.push_back(true);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(-0.48); s.targetLocal.push_back(true);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 1 //
    s.ID = 1;
    s.nextState = 2;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(-1.12); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 2 //
    s.ID = 2;
    s.nextState = 3;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.56); s.targetLocal.push_back(true); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_GAUCHE
    s.targetAngles.push_back(0.13); s.targetLocal.push_back(true); //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_GAUCHE
    s.targetAngles.push_back(0.23); s.targetLocal.push_back(true); //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 3 //
    s.ID = 3;
    s.nextState = 4;
    s.transitionTime = 0.08;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(1.53); s.targetLocal.push_back(true);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(-0.48); s.targetLocal.push_back(true);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 4 //
    s.ID = 4;
    s.nextState = 5;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(-1.12); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 5 //
    s.ID = 5;
    s.nextState = 0;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.56); s.targetLocal.push_back(true); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.13); s.targetLocal.push_back(true); //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_DROIT
    s.targetAngles.push_back(0.23); s.targetLocal.push_back(true); //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_DROIT
    s.targetAngles.push_back(-0.06); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    // copie des premi�res valeurs dans m_anglesAtTransition
    for (unsigned int i=0;i<s.targetAngles.size();i++)
        m_anglesAtTransition.push_back(m_states[m_currentState].targetAngles[i]);

}

FSM_JUMP::FSM_JUMP() {
    m_nbStates = 3;
    m_currentState = 0;
    State s;
    // ETAT 0 //
    s.ID = 0;
    s.nextState = 1;
    s.transitionTime = 0.3;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true);  //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 1 //
    s.ID = 1;
    s.nextState = 2;
    s.transitionTime = 0.50;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //CHEVILLE_DROIT
    s.targetAngles.push_back(-1.20); s.targetLocal.push_back(false);  //GENOU_GAUCHE
    s.targetAngles.push_back(-1.20); s.targetLocal.push_back(false);  //GENOU_DROIT
    s.targetAngles.push_back(1.20); s.targetLocal.push_back(false);  //HANCHE_GAUCHE
    s.targetAngles.push_back(1.20); s.targetLocal.push_back(false);  //HANCHE_DROIT
    s.targetAngles.push_back(-1.00); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);
    // ETAT 2 //
    s.ID = 2;
    s.nextState = 0;
    s.transitionTime = 0.12;
    s.targetAngles.clear(); s.targetLocal.clear();
    s.targetAngles.push_back(-1.0); s.targetLocal.push_back(false); //CHEVILLE_GAUCHE
    s.targetAngles.push_back(-1.0); s.targetLocal.push_back(true); //CHEVILLE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //GENOU_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true); //GENOU_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false); //HANCHE_GAUCHE
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(true); //HANCHE_DROIT
    s.targetAngles.push_back(0.0); s.targetLocal.push_back(false);  //TRONC
    m_states.push_back(s);

    for (unsigned int i=0;i<s.targetAngles.size();i++)
        m_anglesAtTransition.push_back(m_states[m_currentState].targetAngles[i]);
}
