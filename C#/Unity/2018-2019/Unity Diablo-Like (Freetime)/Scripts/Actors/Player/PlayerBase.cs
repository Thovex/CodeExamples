using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

namespace Thovex.Gameplay {
	public class PlayerBase : MonoBehaviour  {

		[SerializeField] private MeshRenderer baseMesh;

		/* Navigational Settings */

		[SerializeField] private NavMeshAgent playerAgent;
	
		[SerializeField] private PlayerCommands playerCommands;

        public PlayerCommands PlayerCommands {
            get {
                return playerCommands;
            }
        }
		private void Update () {
			if (Input.GetMouseButton(0))
            {	
            	MoveCharacter(Input.mousePosition); 
            }
        }

        private void MoveCharacter(Vector3 pos)
        {
            Ray ray = Camera.main.ScreenPointToRay(pos);
			
			RaycastHit hit;
				
			if (Physics.Raycast(ray, out hit)) {
				playerAgent.destination = hit.point;
				Debug.DrawLine(ray.origin, ray.direction + ray.origin, Color.red, 1f);
			}
        }

        private void OnDrawGizmos() {
			Gizmos.DrawLine(transform.position, transform.position + transform.forward);

			if (baseMesh != null) {
				Gizmos.DrawIcon(transform.position + new Vector3(0, baseMesh.bounds.size.y, 0), "player_icon.tiff", true);
			}

			if (playerAgent != null) { 
				for (int i = 1; i < playerAgent.path.corners.Length; i++) {
					Gizmos.DrawLine(playerAgent.path.corners[i], playerAgent.path.corners[i - 1]);
				}
			}
		}
	}
}
