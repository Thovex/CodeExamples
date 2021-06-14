using DG.Tweening;
using UnityEngine;
using UnityEngine.UI;

namespace BaerAndHoggo.Gameplay.Effects
{
    public class ArrowShootEffect : MonoBehaviour
    {

        [SerializeField] private Image imageRef;
        [SerializeField] private Sprite hitSprite;
        
        public void Shoot(Vector3 accurateLocation, Transform onCompleteParent )
        {
            
            var randomOffset = new Vector3(Random.Range(-.25F, .25F), Random.Range(-.25F, .25F), 0);

            var shootPosition = accurateLocation + randomOffset;
            transform.LookAt(shootPosition);
                        
            var tweenArrow = transform.DOMove(shootPosition, Random.Range(0.10F, 0.30F));
            tweenArrow.SetEase(Ease.InFlash);
            tweenArrow.onComplete += () =>
            {
                bool hasChild = onCompleteParent.childCount > 0;
                transform.transform.SetParent(hasChild ? onCompleteParent.GetChild(0) : onCompleteParent, true);

                transform.DOShakeRotation(.5F, new Vector3(25, 25, 0));

                imageRef.sprite = hitSprite;
                
                Invoke(nameof(RemoveArrow), Random.Range(2.5F, 5F));
            };
        }

        private void RemoveArrow()
        {
            var removeTween = imageRef.DOFade(0, 1);
            removeTween.onComplete += () =>
            {
                Destroy(gameObject);
            };
        }
    }
}