using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IKHandling : MonoBehaviour
{
    Animator anim;

    public float ikWeight = 1;

    Vector3 leftFootp;
    Vector3 rightFootp;

    Quaternion leftFootr;
    Quaternion rightFootr;

    float leftFootw;
    float rightFootw;

    Transform leftFoot;
    Transform rightFoot;

    public float offset_Y=0;


    // Start is called before the first frame update
    void Start()
    {
        anim=GetComponent<Animator>();
        leftFoot=anim.GetBoneTransform(HumanBodyBones.LeftFoot);
        rightFoot=anim.GetBoneTransform(HumanBodyBones.RightFoot);
    }

    // Update is called once per frame
    void Update()
    {
        RaycastHit leftHit;
        RaycastHit rightHit;

        Vector3 lpos = leftFoot.TransformPoint(Vector3.zero);
        Vector3 rpos = rightFoot.TransformPoint(Vector3.zero);
        if(Physics.Raycast(lpos,-Vector3.up,out leftHit,1))
        {
            leftFootp=leftHit.point;
            leftFootr=Quaternion.FromToRotation(transform.up,leftHit.normal)*transform.rotation;
        }

        if(Physics.Raycast(rpos,-Vector3.up,out rightHit,1))
        {
            rightFootp=rightHit.point;
            rightFootr=Quaternion.FromToRotation(transform.up,rightHit.normal)*transform.rotation;
        }
    }

    void OnAnimatorIK()
    {
        leftFootw=1-anim.GetFloat("leftFoot");
        rightFootw=1-anim.GetFloat("rightFoot");
        anim.SetIKPositionWeight(AvatarIKGoal.LeftFoot,leftFootw);
        anim.SetIKPositionWeight(AvatarIKGoal.RightFoot,rightFootw);
        anim.SetIKPosition(AvatarIKGoal.LeftFoot,leftFootp);
        anim.SetIKPosition(AvatarIKGoal.RightFoot,rightFootp);

        anim.SetIKRotationWeight(AvatarIKGoal.LeftFoot,leftFootw);
        anim.SetIKRotationWeight(AvatarIKGoal.RightFoot,rightFootw);
        anim.SetIKRotation(AvatarIKGoal.LeftFoot,leftFootr);
        anim.SetIKRotation(AvatarIKGoal.RightFoot,rightFootr);

    }
}
