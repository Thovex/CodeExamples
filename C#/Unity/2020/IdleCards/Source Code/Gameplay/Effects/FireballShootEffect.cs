using BaerAndHoggo.Gameplay.Effects;
using DG.Tweening;
using UnityEngine;

namespace Gameplay.Effects
{
    public class FireballShootEffect : MonoBehaviour
    {
        public void Shoot(Vector3 accurateLocation, Transform onCompleteParent )
        {
            var randomOffset = new Vector3(Random.Range(-.15F, .15F), Random.Range(-.15F, .15F), 0);

            var shootPosition = accurateLocation + randomOffset;
            transform.LookAt(shootPosition);
                        
            var tweenFireball = transform.DOMove(shootPosition, Random.Range(0.30F, 0.50F));
            tweenFireball.SetEase(Ease.InFlash);
            tweenFireball.onComplete += () =>
            {
                Instantiate(EffectsDB.Instance.GetItemById(4).effectPrefab, onCompleteParent);
                Destroy(gameObject);
            };
        }
    }
}