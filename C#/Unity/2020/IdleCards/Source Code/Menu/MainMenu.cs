using System.Collections;
using Sirenix.OdinInspector;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.Serialization;
using UnityEngine.UI;

public class MainMenu : MonoBehaviour
{
    [Title("Button Sprites")] [SerializeField] [Required]
    private Image audioImage;

    [SerializeField] [Required] private AudioMixer audioMixer;

    [Title("Audio References")] [SerializeField] [Required]
    private AudioSource audioSource;

    private Coroutine _coroutine;
     [SerializeField] private string guideUrl = "https://www.jesse.vanvliet.com/idlecards/guide.pdf";

     [Title("URL redirections")] [SerializeField]
    private string likeUrl = "AndroidStore/AppleStore?URL";

    [PreviewField] [SerializeField] private Sprite muteSprite;
     [SerializeField] private string redditUrl = "https://www.reddit.com/r/IdleCards/";
    [PreviewField] [SerializeField] private Sprite unmuteSprite;
     [SerializeField] private string websiteUrl = "https://www.jessevanvliet.com/";

    private void Start()
    {
        if (audioSource.clip)
        {
            var startTime = Random.Range(0, audioSource.clip.length);
            audioSource.time = startTime;
            LerpBackgroundMusic(0);
            audioSource.Play();
        }
    }

    private IEnumerator IntroduceMusic(float to, bool fadeBack = false)
    {
        audioMixer.GetFloat("Volume", out var lerpValue);

        while (!Mathf.Approximately(lerpValue, to))
        {
            lerpValue = Mathf.Lerp(lerpValue, to, Time.deltaTime * (fadeBack ? 10 : 1));
            audioMixer.SetFloat("Volume", lerpValue);
            yield return new WaitForEndOfFrame();
        }

        if (fadeBack)
            while (!Mathf.Approximately(lerpValue, 0))
            {
                lerpValue = Mathf.Lerp(lerpValue, 0, Time.deltaTime * 5);
                audioMixer.SetFloat("Volume", lerpValue);
                yield return new WaitForEndOfFrame();
            }
    }

    // Buttons can only take 1 input
    public void LerpBackgroundMusicFade(float to)
    {
        LerpBackgroundMusic(to, true);
    }

    public void LerpBackgroundMusic(float to, bool fadeBack = false)
    {
        if (_coroutine != null) StopCoroutine(_coroutine);
        _coroutine = StartCoroutine(IntroduceMusic(to, fadeBack));
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

    public void OpenLikeUrl()
    {
        Application.OpenURL(likeUrl);
    }

    public void OpenGuideUrl()
    {
        Application.OpenURL(guideUrl);
    }

    public void OpenWebsiteUrl()
    {
        Application.OpenURL(websiteUrl);
    }

    public void OpenRedditUrl()
    {
        Application.OpenURL(redditUrl);
    }

    public void Disable()
    {
        gameObject.SetActive(false);
    }
}