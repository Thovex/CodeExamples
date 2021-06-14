namespace BaerAndHoggo.SaveData
{
    public interface ISaveData<T>
    {
        T OnSave_Implementation();
        void OnLoad_Implementation(T loadData);
    }
}