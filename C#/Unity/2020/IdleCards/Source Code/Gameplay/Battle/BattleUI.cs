using System.Collections;
using System.Collections.Generic;
using BaerAndHoggo.Gameplay.Battle;
using BaerAndHoggo.Gameplay.Cards;
using BaerAndHoggo.IO;
using BaerAndHoggo.UI;
using Sirenix.OdinInspector;
using TMPro;
using UnityEngine;
using Random = UnityEngine.Random;

public struct BattleData
{
    public EnemyAI EnemyAI;
    public CardDeck PlayerDeck;
}

public class BattleUI : ManagedMenu
{

    [Title("Battle")] 
    [SerializeField] private BattlePlayer enemy;
    [SerializeField] private BattlePlayer player;

    [SerializeField] private TMP_Text turnTextRef;
    [SerializeField] private RectTransform cardDragArea;

    [SerializeField] private int playerTurnIndex;
    [SerializeField] private int enemyTurnIndex;

    [Title("Debug Battle Data")] 
    [SerializeField] private BattleData debugBattle;

    [SerializeField] private bool enemyReady = false;
    [SerializeField] private bool playerReady = false;
    
    private void Start()
    {
        InitializeNewBattle(debugBattle);
    }

    public void InitializeNewBattle(BattleData battleData)
    {
        playerTurnIndex = 1;
        enemyTurnIndex = 1;
        
        LoadingManager.Instance.StopLoading();
        
        player.InitPlayer(battleData.PlayerDeck, enemy);
        enemy.InitPlayer(battleData.EnemyAI.GetRandomDeck(), player);
        
        StartCoroutine(InitialCardDraw(player, 2));
        StartCoroutine(InitialCardDraw(enemy, 2));
        
        player.OnReady += ReadyCheck;
        enemy.OnReady += ReadyCheck;

    }

    private void ReadyCheck()
    {
        if (player.isReady && enemy.isReady)
        {
            player.OnReady -= ReadyCheck;
            enemy.OnReady -= ReadyCheck;

            SetRandomTurn();
        }
    }

    private void SetRandomTurn()
    {
        if (Random.Range(0.0f, 1.0f) >= .50d)
        {
            CallTurnStart(player, playerTurnIndex);

        }
        else
        {            
            CallTurnStart(enemy, enemyTurnIndex);
        }

        SetTurnText();
    }

    private void SetTurnText()
    {
        turnTextRef.text = player.isTurn ? "Your turn." : "Enemy turn.";
    }

    public void EndTurn()
    {
        if (player.isTurn)
        {
            player.EndTurn();
            playerTurnIndex++;
            
            CallTurnStart(enemy, enemyTurnIndex);
        }
        else
        {
            enemy.EndTurn();
            enemyTurnIndex++;
            
            CallTurnStart(player, playerTurnIndex);

        }

        SetTurnText();
    }

    public void CallTurnStart(BattlePlayer player, int turn)
    {
        // Do Captain passive
        CardDB.Instance.GetBaseItem(player.deck.Captain.passiveSpellId, out var spell);
                
        if (spell.type != Type.Spell)
            throw new System.Exception("Some captain has a non-spell card as passive ability");

        var copySpell = Instantiate(spell) as CardSpell;
        copySpell.OnLoad_Implementation(((CardSpell)spell).OnSave_Implementation());
        
        foreach (var slot in player.boardSlots)
        {
            if (!slot.Card) continue;
            var cardMinion = (CardMinion) slot.Card;

            copySpell.DoSpellMinion(cardMinion);

            if (cardMinion.statusEffects == null) continue;
            foreach (var statusEffect in cardMinion.statusEffects)
            {
                statusEffect.Activate_Minion();
            }
        }

        if (player.deck.Captain.statusEffects != null)
        {
            foreach (var statusEffect in player.deck.Captain.statusEffects)
            {
                statusEffect.Activate_Captain();
            }
        }
        
        
        player.SetTurn(turn);
    }

    private IEnumerator InitialCardDraw(BattlePlayer player, int count)
    {
        int index = 0;

        while (index < count)
        {
            yield return new WaitForSeconds(.5F);
            player.DrawCard(true);
            index++;
        }

        player.OnReadyInvocator();
    }
}
