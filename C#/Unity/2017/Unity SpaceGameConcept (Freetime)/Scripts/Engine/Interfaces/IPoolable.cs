namespace Thovex.GameScript {
    public delegate void DataSenderCallback(object sender, object data);
    public interface IPoolable {
        void Recycle();
        event DataSenderCallback onDeath;
    }
}