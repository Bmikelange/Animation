using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BasicCharacterController : MonoBehaviour
{
    float speed = 0.001f;

    float vs=0.0f;
    public Vector3 X= new Vector3(1,0,0);
    public Vector3 Z= new Vector3(0,0,1);
    
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        bool run=false;
        Vector3 forward_world = transform.TransformDirection(Vector3.forward);
        if (Input.GetKey(KeyCode.RightArrow) && !Input.GetKey(KeyCode.LeftControl))
        {
            gameObject.transform.Rotate(new Vector3(0,-45*Time.deltaTime,0));
        }
        if (Input.GetKey(KeyCode.LeftArrow) && !Input.GetKey(KeyCode.LeftControl))
        {
            gameObject.transform.Rotate(new Vector3(0,45*Time.deltaTime,0));
        }
        if (Input.GetKey(KeyCode.DownArrow) && !Input.GetKey(KeyCode.LeftControl))
        {
            gameObject.GetComponent<CharacterController>().Move(transform.TransformDirection(-speed * Z));
        }
        if (Input.GetKey(KeyCode.UpArrow) && !Input.GetKey(KeyCode.LeftControl))
        {
            gameObject.GetComponent<CharacterController>().Move(transform.TransformDirection(speed * Z));
        }
        
        if (Input.GetKey(KeyCode.RightArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
             gameObject.transform.Rotate(new Vector3(0,-45*Time.deltaTime,0));
        }
        if (Input.GetKey(KeyCode.LeftArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
             gameObject.transform.Rotate(new Vector3(0,45*Time.deltaTime,0));
        }
        if (Input.GetKey(KeyCode.DownArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
            gameObject.GetComponent<CharacterController>().Move(transform.TransformDirection(-(1+vs)*speed * Z));
        }
        if (Input.GetKey(KeyCode.UpArrow) && Input.GetKey(KeyCode.LeftControl))
        {
            run=true;
            gameObject.GetComponent<CharacterController>().Move(transform.TransformDirection((1+vs)*speed * Z));
        }
        if(run)
            vs+=1.0f*Time.deltaTime;
        else
            vs-=1.0f*Time.deltaTime;
        vs=Mathf.Clamp(vs,0.0f,1.0f);
        Vector2 mouseInput = new Vector2(Input.GetAxis("Mouse X"), Input.GetAxis("Mouse Y"));
    }
}
