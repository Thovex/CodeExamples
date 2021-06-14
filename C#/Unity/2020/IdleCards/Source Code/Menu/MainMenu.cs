using System.Collections;
using System.Collections.Generic;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{
    [Title("Audio References")]
    [SerializeField] [Required] private AudioSource audioSource;
    [SerializeField] [Required] private AudioMixer audioMixer;

    [Title("URL redirections")]
    [SerializeField] private string likeURL = "AndroidStore/AppleStore?URL";
    [SerializeField] private string guideURL = "https://www.jesse.vanvliet.com/idlecards/guide.pdf";
    [SerializeField] private string websiteURL = "https://www.jessevanvliet.com/";
    [SerializeField] private string redditURL = "https://www.reddit.com/r/IdleCards/";

    [Title("Button Sprites")]
    [SerializeField] [Required] private Image audioImage;
    [PreviewField] [SerializeField] private Sprite unmuteSprite;
    [PreviewField] [SerializeField] private Sprite muteSprite;

    private Coroutine coroutine;

    private void Start()
    {

        float startTime = Random.Range(0, audioSource.clip.length);
        audioSource.time = startTime;
        LerpBackgroundMusic(0);
        audioSource.Play();
    }

    private IEnumerator IntroduceMusic(float to, bool fadeBack = false)
    {
        audioMixer.GetFloat("Volume", out float lerpValue);

        while (!Mathf.Approximately(lerpValue, to))
        {
            lerpValue = Mathf.Lerp(lerpValue, to, Time.deltaTime * ((fadeBack) ? 10 : 1));
            audioMixer.SetFloat("Volume", lerpValue);
            yield return new WaitForEndOfFrame();
        }

        if (fadeBack)
        {
            while (!Mathf.Approximately(lerpValue, 0))
            {
                lerpValue = Mathf.Lerp(lerpValue, 0, Time.deltaTime * 5);
                audioMixer.SetFloat("Volume", lerpValue);
                yield return new WaitForEndOfFrame();
            }
        }
    }

    // Buttons can only take 1 input
    public void LerpBackgroundMusicFade(float to) => LerpBackgroundMusic(to, true);
    public void LerpBackgroundMusic(float to, bool fadeBack = false)
    {
        if (coroutine != null) StopCoroutine(coroutine);
        coroutine = StartCoroutine(IntroduceMusic(to, fadeBack));
    }

    public void CloseApplication()
    {
        // TODO: Save game state.
        Application.Quit();
    }

    public void MuteUnmuteAudio()
    {
        if (AudioListener.volume == 0)
        {
            AudioListener.volume = 1;
            audioImage.sprite = unmuteSprite;
        }
        else
        {
            AudioListener.volume = 0;
            audioImage.sprite = muteSprite;
        }
    }

    public void OpenLikeURL() => Application.OpenURL(likeURL);
    public void OpenGuideURL() => Application.OpenURL(guideURL);
    public void OpenWebsiteURL() => Application.OpenURL(websiteURL);
    public void OpenRedditURL() => Application.OpenURL(redditURL);
    public void Disable() => gameObject.SetActive(false);
}
