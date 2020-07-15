
#ifndef _CHARANIMVIEWER_H
#define _CHARANIMVIEWER_H


#include "quaternion.h"
#include "Viewer.h"
#include "BVH.h"
#include "Skeleton.h"
#include "TransformQ.h"
#include "CharacterController.h"

#include <PhysicalWorld.h>

class CharAnimViewer : public Viewer
{
public:
    CharAnimViewer();

    int init();
    int render();
    int update( const float time, const float delta );

	static CharAnimViewer& singleton() { return *psingleton;  }

protected:
	void bvhDrawRec(const chara::BVHJoint& bvh, const Transform& f2w, int f);

    chara::BVH m_bvh;
    chara::BVH m_bvh1;
    chara::BVH m_bvh2;
    chara::BVH m_bvh3;
    chara::BVH m_bvh4;
    chara::BVH m_bvh5;
    chara::BVH m_bvh6;
    chara::BVH m_bvh7;
    chara::BVH m_bvh8;
    chara::BVH m_bvh9;
    chara::BVH m_bvh10;
    chara::BVH m_bvh11;

    int m_frameNumber;

    Skeleton m_ske;

    Skeleton* m_skePrincipal;

    PhysicalWorld m_world;

	void draw_skeleton(const Skeleton& );
    void draw_skeleton_transform(const Skeleton& s,Transform T);

private:
	static CharAnimViewer* psingleton;
    CharacterController cp;
};



#endif
