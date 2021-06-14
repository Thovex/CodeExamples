
using System;
using BaerAndHoggo.Gameplay.Battle;
using BaerAndHoggo.Gameplay.Battle.Targeting;
using BaerAndHoggo.Gameplay.Cards;
using UnityEngine;
using Type = BaerAndHoggo.Gameplay.Cards.Type;

public class CaptainSkill : MonoBehaviour
{
    [SerializeField] private Transform dragAreaTransform;

    public void Initialize(BattlePlayer player)
    {
        CardDB.Instance.GetBaseItem(player.deck.Captain.activeSpellId, out var spell);

        if (spell.type != Type.Spell)
            throw new System.Exception("Some captain has a non-spell card as active ability");
        
        var copySpell = Instantiate(spell) as CardSpell;
        copySpell.OnLoad_Implementation(((CardSpell)spell).OnSave_Implementation());
        
        var captainSpellTarget= GetComponent<CaptainSpellTarget>();
        captainSpellTarget.Initialize(new CaptainSpellTargetData()
        {
            Player = player,
            DragAreaTransform = dragAreaTransform,
            IncludeCaptain = true,
            OriginParent = transform,
            OriginPosition = transform.position,
            Spell = copySpell,
        });
    }
}
