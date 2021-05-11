using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TripleSrcRootTarget
{
    public Transform src;
    public Transform tgt;
    public Transform root;
}
public class IK : MonoBehaviour
{
    // Le transform (noeud) racine de l'arbre, 
    // le constructeur créera une sphère sur ce point pour en garder une copie visuelle.
    public GameObject rootNode = null;   
    public GameObject TargetNode = null;         
    
    // Un transform (noeud) (probablement une feuille) qui devra arriver sur targetNode

    public List<TripleSrcRootTarget> NodeInfo=new List<TripleSrcRootTarget>();                      

    // Si vrai, recréer toutes les chaines dans Update
    public bool createChains = true;                            
    
    // Toutes les chaines cinématiques 
    public List<IKChain> chains = new List<IKChain>();          

    // Nombre d'itération de l'algo à chaque appel
    public int nb_ite = 10;   

    public bool init=true;                                  

    void recursiveFunc(ref Transform tr,ref Transform Root)
    {
        int nbChild=tr.childCount;
        if(nbChild>1)
        {
            TripleSrcRootTarget L = new TripleSrcRootTarget();
            L.root=Root;
            L.src=tr;
            L.tgt=null;
            NodeInfo.Add(L);

            for(int i=0;i<nbChild;i++)
            {
                Transform t=tr.GetChild(i);
                recursiveFunc(ref t,ref tr);
            }
        }
        else if(nbChild==0)
        {
            TripleSrcRootTarget L = new TripleSrcRootTarget();
            L.root=Root;
            L.src=tr;
            string tag=tr.tag;
            L.tgt=TargetNode.transform.GetChild(int.Parse(tag));
            NodeInfo.Add(L);
            return;
        }
        else
        {
            Transform t=tr.GetChild(0);
            recursiveFunc(ref t,ref Root);
        }
        return;
    }

    void Start()
    {
        if (createChains)
        {
            for(int i=0;i<chains.Count;i++)
            {
                chains[i].destroyCylinder();
            }
            chains.Clear();
            // TODO : 
            // Création des chaines : une chaine cinématique est un chemin entre deux nœuds carrefours.
            // Dans la 1ere question, une unique chaine sera suffisante entre srcNode et rootNode.

            // voir partie 2

            // TODO-2 : Pour parcourir tous les transform d'un arbre d'Unity vous pouvez faire une fonction récursive
            // ou utiliser GetComponentInChildren comme ceci :
            if(init)
            {
                Transform t=gameObject.transform;
                recursiveFunc(ref t, ref t);
                init=false;
            }
            for(int i=0; i<NodeInfo.Count;i++)
            {
                chains.Add(new IKChain(NodeInfo[i].src, NodeInfo[i].root, NodeInfo[i].tgt, rootNode.transform));
            }
            for (int i = 0; i < chains.Count; i++)
            {
                for (int k = 0; k < chains.Count; k++)
                {
                    chains[i].Merge(chains[k].Last());
                    chains[i].Merge(chains[k].First());
                }
            }
            
            // TODO-2 : Dans le cas où il y a plusieurs chaines, fusionne les IKJoint entre chaque articulation.
        }
    }

    void Update()
    {
        if (createChains)
            Start();

        //if (Input.GetKeyDown(KeyCode.I))
        //{
            IKOneStep(true);
        //}
        
        if (Input.GetKeyDown(KeyCode.C))
        {
            Debug.Log("Chains count="+chains.Count);
            foreach (IKChain ch in chains)
                ch.Check();
        }
    }


    void IKOneStep(bool down)
    {
        int j;

        for (j = 0; j < nb_ite; ++j)
        {
            // TODO : IK Backward (remontée), appeler la fonction Backward de IKChain 
            // sur toutes les chaines cinématiques.

            for(int i=chains.Count-1;i>=0;i--)
            {
                chains[i].Backward();
            }
            for(int i=chains.Count-1;i>=0;i--)
            {
                chains[i].ToTransform();
            }
            for(int i=0;i<chains.Count;i++)
            {
                chains[i].Forward();
            }
            for(int i=0;i<chains.Count;i++)
            {
                chains[i].ToTransform();
            }
            // IK Forward (descente), appeler la fonction Forward de IKChain 
            // sur toutes les chaines cinématiques.
                      
            // TODO : appliquer les positions des IKJoint aux transform en appelant ToTransform de IKChain
            
        }



    }


}
