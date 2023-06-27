using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class localRotation : MonoBehaviour
{
public GameObject objectToControl;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

                //5cases here, finish win 1,finished lose 2 , start ends with team a win, start with team B win
                
                if (Input.GetKeyUp("a")) {
                        Vector3 rotationVector1 = new Vector3();
                        rotationVector1 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        -60);
                        Debug.Log("running 1");
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector1);
                }
                if (Input.GetKeyUp("s")) {
                        Vector3 rotationVector2 = new Vector3();
                        rotationVector2 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        60);
                        Debug.Log("running 3");
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector2);
                }

                if (Input.GetKeyUp("d")) {
                        Vector3 rotationVector3 = new Vector3();
                        rotationVector3 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        0);
                        Debug.Log("running 2");
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector3);
                    }
                    
                    if (Input.GetKeyUp("f")) {
                        Vector3 rotationVector5 = new Vector3();
                        rotationVector5 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        0);
                        Debug.Log("running 4");
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector5);
                    }
                
            }
        
}
