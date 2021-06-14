using UnityEngine;

public class User : MonoBehaviour
{
    [SerializeField] private long userId;
    [SerializeField] private string userName;

    //private void Update()
    //{
    //var eventSystem = GameObject.FindObjectOfType<EventSystem>();
    //uiRaycast = eventSystem.GetComponent<GraphicRaycaster>();

    //List<RaycastResult> results = new List<RaycastResult>();
    //uiRaycast.Raycast(eventData, results);

    //results has all of the canvas objects

    //if (eventSystem.IsPointerOverGameObject())
    //{
    //    PointerEventData test = ExtendedStandaloneInputModule.GetPointerEventData();

    //    for (int i = 0; i < test.hovered.Count; i++) {
    //       GameObject hovered = test.hovered[i];
    //       Debug.Log($"{i} ---- {hovered.name}");

    //    }
    //}
    //}
}