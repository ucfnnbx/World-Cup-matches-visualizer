using UnityEngine;

public class mqttControllerList : MonoBehaviour
{   
    public Material[] dot_mat;
    public Material[] dot_mat2;

    public int current_angle = 0;
    public Color colorOff = new Color();

    public Color colorOn = new Color();

    [Tooltip("Optional name for the controller")]
    public string nameController = "Controller 1";
    public string tagOfTheMQTTReceiver = ""; //to be set on the Inspector panel. It must match one of the mqttReceiverList.cs GameObject
    [Header("   Case Sensitive!!")]
    [Tooltip("the topic to subscribe must contain this value. !!Case Sensitive!! ")]
    public string topicSubscribed = ""; //the topic to subscribe, it need to match a topic from the mqttReceiver
    private float pointerValue = 0.0f;
    public convert.Root convertObject = new convert.Root();

    private string used;
    
    [Space] [Space]
    public GameObject objectToControl; //pointer of the gauge, or other 3D models
    [Tooltip("Select the rotation axis of the object to control")]
    public enum State { X, Y, Z };
    public State rotationAxis;
    [Space] [Tooltip("Direction Rotation")]
    public bool CW = true; //CW True = 1; CW False = -1
    private int rotationDirection = 1;
    [Space] [Space]
    [Tooltip("minimum value on the dial")]
    public float startValue = 0f; //start value of the gauge
    [Tooltip("maximum value on the dial")]
    public float endValue = 180f; // end value of the gauge
    [Tooltip("full extension of the gauge in EulerAngles")]
    public float fullAngle = 180f; // full extension of the gauge in EulerAngles

    [Tooltip("Adjust the origin of the scale. negative values CCW; positive value CW")]
    public float adjustedStart = 0f; // negative values CCW; positive value CW
    [Space]
    public mqttReceiverList _eventSender;

    public Material Flag1;
    public Material Flag2;
    [System.Serializable]
    public struct flags
    {
        public string country;
        public Texture2D image;
    }
    public flags[] pictures;


    public class convert{
    [System.Serializable]
    public class Root
    {
        public int home_score ;
        public int away_score ;
        public string home_team_en ;
        public string away_team_en ;
        public string finished ;
        public string datetime ;
    }
    }



    void Awake()
    {
        if (GameObject.FindGameObjectsWithTag(tagOfTheMQTTReceiver).Length > 0)
        {
            _eventSender = GameObject.FindGameObjectsWithTag(tagOfTheMQTTReceiver)[0].gameObject.GetComponent<mqttReceiverList>();
        }
        else
        {
            Debug.LogError("At least one GameObject with mqttReceiver component and Tag == tagOfTheMQTTReceiver needs to be provided");
        }
    }

    void OnEnable()
    {
        _eventSender.OnMessageArrived += OnMessageArrivedHandler;
    }

    private void OnDisable()
    {
        _eventSender.OnMessageArrived -= OnMessageArrivedHandler;
    }

    private void OnMessageArrivedHandler(mqttObj mqttObject) //the mqttObj is defined in the mqttReceiverList.cs
    {

        //We need to check the topic of the message to know where to use it 
        if (mqttObject.topic.Contains(topicSubscribed))
        {
            used = "false";
           // pointerValue = float.Parse(mqttObject.msg);
            convertObject = JsonUtility.FromJson<convert.Root>(mqttObject.msg);
            Debug.Log("Event Fired. The message, from Object " + nameController + " is = " + convertObject.home_score);

foreach (var item in pictures){
        if (item.country == convertObject.home_team_en){
            Flag1.mainTexture=  item.image;
        }
        if (item.country == convertObject.away_team_en){
            Flag2.mainTexture=  item.image;
        }
    }

        for (int i = 0; i< dot_mat.Length; i++)
        {
            if (i < convertObject.home_score ){
                dot_mat[i].color = colorOn;
            }else{
                dot_mat[i].color = colorOff;
            }
        }

        for (int i = 0; i< dot_mat2.Length; i++)
        {
            if (i < convertObject.away_score ){
                dot_mat2[i].color = colorOn;
            }else{
                dot_mat2[i].color = colorOff;
            }
        }

            if (convertObject.finished != used){
                //5cases here, finish win 1,finished lose 2 , start ends with team a win, start with team B win
                
                if (convertObject.home_score < convertObject.away_score) {
                    if ( current_angle == 0){
                        Vector3 rotationVector1 = new Vector3();
                        rotationVector1 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        -60);
                        Debug.Log("running 1");
                        current_angle = -60;
                        used = convertObject.finished;
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector1);
                    }
                }
                if (convertObject.home_score >convertObject.away_score){
                    if (current_angle == 0){
                        Vector3 rotationVector2 = new Vector3();
                        rotationVector2 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        60);
                        current_angle = 60;
                        Debug.Log("running 3");
                        used = convertObject.finished;
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector2);
                    }
                }

                    if (current_angle == -60 ){
                        if (convertObject.finished != used){
                        Vector3 rotationVector3 = new Vector3();
                        rotationVector3 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        60);
                        Debug.Log("running 2");
                        current_angle = 0;
                        used = convertObject.finished;
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector3);
                    }
                    }
                    
                    if (current_angle == 60){
                        if (convertObject.finished != used){
                        Vector3 rotationVector5 = new Vector3();
                        rotationVector5 = new Vector3
                        (objectToControl.transform.localEulerAngles.x,
                        objectToControl.transform.localEulerAngles.y,
                        -60);
                        Debug.Log("running 4");
                        current_angle = 0;
                        used = convertObject.finished;
                        objectToControl.transform.localRotation = Quaternion.Euler(rotationVector5);
                    }
                    }
                
            
                
            }
    //     if (convertObject.finished != used){// && (response.home_score < response.away_score)){
    //             if (objectToControl.transform.localRotation.eulerAngles.z < -10){
    //                 if (convertObject.home_score < convertObject.away_score){
    //         Vector3 rotationVector3 = new Vector3();
    //         rotationVector3 = new Vector3
    //             (objectToControl.transform.localEulerAngles.x,
    //             objectToControl.transform.localEulerAngles.y,
    //             60);
    //             objectToControl.transform.localRotation = Quaternion.Euler(rotationVector3); }
    //             }

    //             if (objectToControl.transform.localRotation.eulerAngles.z > 10){
    //                 if (convertObject.home_score > convertObject.away_score){
    //         Vector3 rotationVector4 = new Vector3();
    //         rotationVector4 = new Vector3
    //             (objectToControl.transform.localEulerAngles.x,
    //             objectToControl.transform.localEulerAngles.y,
    //             -60);
    //             objectToControl.transform.localRotation = Quaternion.Euler(rotationVector4);}
    //             }
                    
    //     if((objectToControl.transform.localRotation.eulerAngles.z < 1)&&(objectToControl.transform.localRotation.eulerAngles.z > -1)){
    //     if (convertObject.home_score > convertObject.away_score){
    //         Vector3 rotationVector1 = new Vector3();
    //         rotationVector1 = new Vector3
    //             (objectToControl.transform.localEulerAngles.x,
    //             objectToControl.transform.localEulerAngles.y,
    //             30);
    //             objectToControl.transform.localRotation = Quaternion.Euler(rotationVector1);
    //     }
    //     if (convertObject.home_score < convertObject.away_score){//a<b
    //         Vector3 rotationVector2 = new Vector3();
    //         rotationVector2 = new Vector3
    //             (objectToControl.transform.localEulerAngles.x,
    //             objectToControl.transform.localEulerAngles.y,
    //             -30);
    // objectToControl.transform.localRotation = Quaternion.Euler(rotationVector2);
    //     } }}



        }
    }

    private void Update()
    {
       // float step = 1.5f * Time.deltaTime;

                
        // float step = 1.5f * Time.deltaTime;
        // // ternary conditional operator https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/conditional-operator
        // rotationDirection = CW ? 1 : -1;

        // if (pointerValue >= startValue)
        // {
        //     Vector3 rotationVector = new Vector3();
        //     //If the rotation Axis is X
        //     if (rotationAxis == State.X)
        //     {
        //         rotationVector = new Vector3(
        //         (rotationDirection * ((pointerValue - startValue) * (fullAngle / (endValue - startValue)))) - adjustedStart,
        //         objectToControl.transform.localEulerAngles.y,
        //         objectToControl.transform.localEulerAngles.z);
        //     }
        //     //If the rotation Axis is Y
        //     else if (rotationAxis == State.Y)
        //     {
        //         rotationVector = new Vector3(
        //         objectToControl.transform.localEulerAngles.x,
        //         (rotationDirection * ((pointerValue - startValue) * (fullAngle / (endValue - startValue)))) - adjustedStart,
        //         objectToControl.transform.localEulerAngles.z);

        //     }
        //     //If the rotation Axis is Z
        //     else if (rotationAxis == State.Z)
        //     {
        //         rotationVector = new Vector3(
        //         objectToControl.transform.localEulerAngles.x,
        //         objectToControl.transform.localEulerAngles.y,
        //         (rotationDirection * ((pointerValue - startValue) * (fullAngle / (endValue - startValue)))) - adjustedStart);
        //     }
        //     objectToControl.transform.localRotation = Quaternion.Lerp(
        //             objectToControl.transform.localRotation,
        //             Quaternion.Euler(rotationVector),
        //             step);
        // }
    }
        }