using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IKTargetMvt2 : MonoBehaviour
{
    private float vitesse=0.1f;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Q))
            transform.position += vitesse*Vector3.left;
        if (Input.GetKey(KeyCode.D))
            transform.position += vitesse*Vector3.right;
        if (Input.GetKey(KeyCode.Z))
            transform.position += vitesse*Vector3.back;
        if (Input.GetKey(KeyCode.S))
            transform.position += vitesse*Vector3.forward;
        if (Input.GetKey(KeyCode.A))
            transform.position += vitesse*Vector3.up;
        if (Input.GetKey(KeyCode.E))
            transform.position += vitesse*Vector3.down;
    }
}
