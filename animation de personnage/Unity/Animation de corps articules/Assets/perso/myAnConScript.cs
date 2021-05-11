using System.Collections;
using System.Collections.Generic;
using UnityEngine;



public class myAnConScript : MonoBehaviour
{
    // Start is called before the first frame update
    Animator myAnimator;
    public float vs=0.0f;
    void Start()
    {
        myAnimator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        bool walk=false;
        bool run=false;
        float clampe=0.5f;

        if (Input.GetKey(KeyCode.RightArrow))
        {
            walk=true;
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            walk=true;
        }
        if (Input.GetKey(KeyCode.DownArrow) )
        {
            walk=true;
        }
        if (Input.GetKey(KeyCode.UpArrow))
        {
            walk=true;
        }
        
        if (Input.GetKey(KeyCode.RightArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
        }
        if (Input.GetKey(KeyCode.LeftArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
        }
        if (Input.GetKey(KeyCode.DownArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
        }
        if (Input.GetKey(KeyCode.UpArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
        }
        if(walk || run)
        {
            vs+=1.0f*Time.deltaTime;
        }
        else
        {
            vs-=1.0f*Time.deltaTime;
        }
        if (run)
        {
            clampe=1.0f;
        }
        vs=Mathf.Clamp(vs,0.0f,clampe);
        myAnimator.SetFloat("vSpeed", vs);
    }
}
