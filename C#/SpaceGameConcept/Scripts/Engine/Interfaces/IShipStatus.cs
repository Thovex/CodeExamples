namespace Thovex.GameScript {
    public interface IShipStatus {
        void TakeDamage(float damageAmount);
        bool CheckShield();
        //void GetShield();
    }
}
