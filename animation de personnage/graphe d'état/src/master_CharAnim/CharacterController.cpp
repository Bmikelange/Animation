#include "CharacterController.h"
#include <iostream>

CharacterController::CharacterController()
{
    setPosition(Vector(0,0,0));
	setVelocityMax(2);
	setVelocityNorm(0);
}

void CharacterController::update(const float dt)
{
    if(!key_state('x') && !key_state('r') && !key_state('t') && !key_state('p') && !key_state('i') && !key_state('u') && !key_state('m') && !key_state('l'))
    {
        if (key_state('q') && !key_state('s'))
        {
            turnXZ(-1);
        }
        if(key_state('d') && !key_state('q'))
        {
            turnXZ(1);
        }
        if(key_state('z') && !key_state('s'))
        {
            if(key_state('f'))
            {
                setVelocityNorm(2);
            }
            else
            {
                setVelocityNorm(1);
            }
            m_ch2w=Translation(direction() * m_v * 0.1) * m_ch2w;
        }
        else if(key_state('s') && !key_state('z'))
        {
            if(key_state('f'))
            {
                setVelocityNorm(2);
            }
            else
            {
                setVelocityNorm(1);
            }
            m_ch2w=Translation(-direction() * m_v * 0.1) * m_ch2w;
        }
        else
        {
            setVelocityNorm(0);
        }
        if(key_state('d') && !key_state('q'))
        {
            turnXZ(1);
        }
        if(key_state('z') && !key_state('s'))
        {
            if(key_state('f'))
            {
                setVelocityNorm(2);
            }
            else
            {
                setVelocityNorm(1);
            }
            m_ch2w=Translation(direction() * m_v * 0.1) * m_ch2w;
        }
        else if(key_state('s') && !key_state('z'))
        {
            if(key_state('f'))
            {
                setVelocityNorm(2);
            }
            else
            {
                setVelocityNorm(1);
            }
            m_ch2w=Translation(-direction() * m_v * 0.1) * m_ch2w;
        }
        else
        {
            setVelocityNorm(0);
        }
    }
}


void CharacterController::turnXZ(const float& rot_angle_v)
{
    m_ch2w=m_ch2w*RotationY(rot_angle_v);
}

void CharacterController::accelerate(const float& speed_inc)
{
    m_v+=speed_inc;
}

void CharacterController::setVelocityMax(const float vmax)
{
    m_vMax=vmax;
}

void CharacterController::setPosition(const Vector& p)
{
    Position=p;
    m_ch2w=Translation(p);
}

void CharacterController::setVelocityNorm(const float v){
    m_v=v;
}

float CharacterController::velocity() const
{
    return m_v;
}

const Vector CharacterController::position() const
{
    return (Vector)m_ch2w(Point(0,0,0));
}

const Vector CharacterController::direction() const
{
    return m_ch2w(Vector(0,0,1));
}

Transform CharacterController::getCh2w()
{
    return m_ch2w;
}
