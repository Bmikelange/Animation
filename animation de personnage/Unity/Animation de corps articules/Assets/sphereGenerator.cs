using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class sphereGenerator : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                GameObject sphere = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                sphere.transform.position=new Vector3(i*20,0,j*20);
                Rigidbody rb=sphere.AddComponent<Rigidbody>();
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
