
#include "Skeleton.h"

using namespace chara;

void Skeleton::init(const BVH& bvh)
{
	m_joints.resize(0);
	for(int i=0;i<bvh.getNumberOfJoint();i++)
	{
		SkeletonJoint m;
		BVHJoint b=bvh.getJoint(i);
		m.m_parentId=b.getParentId();
		float x,y,z;
		b.getOffset(x,y,z);
		m.m_l2w=Translation(x,y,z);
		if(b.getParentId()!=-1)
		{
			m.m_l2w=m_joints[b.getParentId()].m_l2w*m.m_l2w;
		}
		m_joints.push_back(m);
	}
}


Point Skeleton::getJointPosition(int i) const
{
	return (Point)m_joints[i].m_l2w(Point(0,0,0));
}


int Skeleton::getParentId(const int i) const
{
	return m_joints[i].m_parentId;
}


void Skeleton::setPose(const BVH& bvh, int frameNumber)
{
	if(frameNumber>=-1 && frameNumber< bvh.getNumberOfFrame())
	{
		for(int i=0;i<bvh.getNumberOfJoint();i++)
		{
			float x,y,z;
			bvh.getJoint(i).getOffset(x,y,z);
			Transform T1 = Translation(x,y,z);
			Transform T2 = Identity();
			for(int j=0;j<bvh.getJoint(i).getNumberOfChannel();j++)
			{
				BVHChannel c=bvh.getJoint(i).getChannel(j);
				if(c.isTranslation())
				{
					if(c.getAxis()==chara::AXIS_X)
					{
						T1 =T1* Translation(c.getData(frameNumber),0,0);
					}
					if(c.getAxis()==chara::AXIS_Y)
					{
						T1 =T1* Translation(0,c.getData(frameNumber),0);
					}
					if(c.getAxis()==chara::AXIS_Z)
					{
						T1 =T1* Translation(0,0,c.getData(frameNumber));
					}
				}
				if(c.isRotation())
				{
					if(c.getAxis()==chara::AXIS_X)
					{
						T2 =T2* RotationX(c.getData(frameNumber));
					}
					if(c.getAxis()==chara::AXIS_Y)
					{
						T2 =T2* RotationY(c.getData(frameNumber));
					}
					if(c.getAxis()==chara::AXIS_Z)
					{
						T2 =T2* RotationZ(c.getData(frameNumber));
					}
				}
			}
			Transform l2w=T1*T2;
			if(bvh.getJoint(i).getParentId()!=-1)
			{
				l2w=m_joints[bvh.getJoint(i).getParentId()].m_l2w*l2w;
			}
			m_joints[i].m_l2w=l2w;
		}
	}
	
}
