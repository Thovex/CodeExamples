using UnityEngine;

namespace BaerAndHoggo.UI
{
    public abstract class UpdateAbleItem<T> : MonoBehaviour
    {
        private T _item;
        public T Item
        {
            get => _item;
            set
            {
                _item = value;
                InitItem();
            }
        }

        protected abstract void InitItem();
    }
}