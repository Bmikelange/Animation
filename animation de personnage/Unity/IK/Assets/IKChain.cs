using System.Collections;
using System.Collections.Generic;
using UnityEngine;




public class IKChain
{
    // Quand la chaine comporte une cible pour la racine. 
    // Ce sera le cas que pour la chaine comportant le root de l'arbre.
    private IKJoint rootTarget = null;                              
    
    // Quand la chaine à une cible à atteindre, 
    // ce ne sera pas forcément le cas pour toutes les chaines.
    private IKJoint endTarget = null;                               

    // Toutes articulations (IKJoint) triées de la racine vers la feuille. N articulations.
    private List<IKJoint> joints = new List<IKJoint>();             
    
    // Contraintes pour chaque articulation : la longueur (à modifier pour 
    // ajouter des contraintes sur les angles). N-1 contraintes.
    private List<float> constraints = new List<float>();            
    
    
    // Un cylindre entre chaque articulation (Joint). N-1 cylindres.
    private List<GameObject> cylinders = new List<GameObject>();    

    public void CreateCylinderBP(Vector3 start, Vector3 end, float width, int i)
    {
        Vector3 offset = end - start;
        Vector3 scale = new Vector3(width, offset.magnitude / 2.0f, width);
        Vector3 position = start + (offset / 2.0f);
        GameObject cylinder=GameObject.CreatePrimitive(PrimitiveType.Cylinder);
        cylinder.transform.position = position;
        cylinder.transform.up = offset;
        cylinder.transform.localScale = scale;
        cylinders.Add(cylinder);
    }

    public void destroyCylinder()
    {
        for(int i=0;i<cylinders.Count;i++)
        {
            UnityEngine.Object.Destroy(cylinders[i]);
        }
    }

    // Créer la chaine d'IK en partant du noeud endNode et en remontant jusqu'au noeud plus haut, ou jusqu'à la racine
    public IKChain(Transform _endNode, Transform _rootTarget=null, Transform _endTarget=null, Transform _Root_Target_Princ=null)
    {
        // TODO : construire la chaine allant de _endNode vers _rootTarget en remontant dans l'arbre. 
        // Chaque Transform dans Unity a accès à son parent 'tr.parent'
        if(_Root_Target_Princ.name==_rootTarget.name)
            rootTarget=new IKJoint(_rootTarget);
        else 
            rootTarget=null;
        if(_endTarget==null)
            endTarget=null;
        else
            endTarget=new IKJoint(_endTarget);
        Transform temp=_endNode;
        while(temp !=_rootTarget && temp != null )
        {
            joints.Add(new IKJoint(temp));
            temp=temp.parent;
        }
        if(temp !=null)
            joints.Add(new IKJoint(temp));
        joints.Reverse();
        for(int i=0;i<joints.Count-1;i++)
        {
            float f=(joints[i].position-joints[i+1].position).magnitude;
            constraints.Add(f);
            CreateCylinderBP(joints[i].position, joints[i+1].position, 0.5f, i);
        }

    }


    public void Merge(IKJoint j)
    {
        // TODO-2 : fusionne les noeuds carrefour quand il y a plusieurs chaines cinématiques
        // Dans le cas d'une unique chaine, ne rien faire pour l'instant.
        if (First().name == j.name)
        {
            joints[0] = j;
        }
        if (Last().name == j.name)
        {
            joints[joints.Count - 1] = j;
        } 
    }


    public IKJoint First()
    {
        return joints[0];
    }
    public IKJoint Last()
    {
        return joints[ joints.Count-1 ];
    }

    public void Backward()
    {
        // TODO : une passe remontée de FABRIK. Placer le noeud N-1 sur la cible, 
        // puis on remonte du noeud N-2 au noeud 0 de la liste 
        // en résolvant les contrainte avec la fonction Solve de IKJoint.
        if(endTarget==null)
            Last().SetPosition(Last().position);
        else
            joints[joints.Count-1].SetPosition(endTarget.position);
        for(int i=joints.Count-2; i>=0;--i)
        {
            joints[i].Solve(joints[i+1],constraints[i]);
        }

    }

    public void Forward()
    {
        // TODO : une passe descendante de FABRIK. Placer le noeud 0 sur son origine puis on descend.
        // Codez et deboguez déjà Backward avant d'écrire celle-ci.
        if (rootTarget == null)
            First().SetPosition(First().position);
        else
            First().SetPosition(rootTarget.position);
        for(int i=1; i<joints.Count;i++)
        {
            joints[i].Solve(joints[i-1],constraints[i-1]);
        }
    }

    public void ToTransform()
    {
        // TODO : pour tous les noeuds de la liste appliquer la position au transform : voir ToTransform de IKJoint
        for(int i=0;i<joints.Count;i++)
            joints[i].ToTransform();
    }

    public void Check()
    {
        // TODO : des Debug.Log pour afficher le contenu de la chaine (ne sert que pour le debug)
        Debug.Log("Check");
        foreach (IKJoint j in joints)
        {
            Debug.Log(j.position);
        }
        Debug.Log(constraints.Count);
        foreach (float f in constraints)
        {
            Debug.Log(f);
        }
    }

}
