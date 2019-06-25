using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerScript : MonoBehaviour {

	public float playerHealth = 1;
	

	public GameObject yes;
	
	public bool isPlayerDead = true;

	private string playerName = "ye";

	public float playerMoney = 1337;
	public float Counter = 0;

	public float wdowadowad = 1000;


	private void Start () {
		InvokeRepeating("DecreaseHP", Time.deltaTime, Time.deltaTime);
		InvokeRepeating("RandomMoney", Time.deltaTime, Time.deltaTime);
		InvokeRepeating("SomeFunction", Time.deltaTime, Time.deltaTime);
	}

	private void Update() {
		GraphFieldSettingsData newStyle = new GraphFieldSettingsData();
		//newStyle.Randomize();

		playerHealth.GraphLog("Player Health", 0, newStyle);
		playerMoney.GraphLog("Player Money", 1, newStyle);
		Counter.GraphLog("Counter", 2, newStyle);

		Mathf.Sin(Time.time).GraphLog("Sin of Time", 3, newStyle);

	}
	private void SomeFunction() {

	}

	private void RandomMoney() {
		playerMoney+= Random.Range(-500,500);

		playerMoney = Mathf.Clamp(playerMoney, 0, 10000) * Mathf.Sin(Time.time) * Time.deltaTime * Random.Range(-Mathf.Tan(Time.time), Mathf.Cos(Time.time));
		playerMoney = Mathf.Clamp(playerMoney, Mathf.Sin(Time.time), Mathf.Cos(Time.time));
	}

	private void DecreaseHP() {
		if (Counter < 10){
			playerHealth = 0;
		} else{
			playerHealth = 1;
		}
		if (Counter > 20){
			Counter = 0;
		}else{
			Counter++;
		}
	}
}
