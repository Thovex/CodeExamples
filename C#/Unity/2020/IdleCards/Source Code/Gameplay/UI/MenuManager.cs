using System.Collections;
using System.Collections.Generic;
using System.Linq;
using BaerAndHoggo.UI;
using BaerAndHoggo.Utilities;
using UnityEngine;

public class MenuManager : Singleton<MenuManager>
{
    [SerializeField] private ManagedMenu fallbackMenu;
    
    [SerializeField] private ManagedMenu currentMenu;
    [SerializeField] private ManagedMenu previousMenu;

    [SerializeField] private List<ManagedMenu> registeredMenus 
        = new List<ManagedMenu>();

    [SerializeField] private Stack<ManagedMenu> navigationHistory 
        = new Stack<ManagedMenu>();

    public void RegisterMenu(ManagedMenu menu)
    {
        if (!registeredMenus.Contains(menu))
        {
            registeredMenus.Add(menu);
            Debug.Log($"{menu.name} added to registered menu's.");

            if (menu.moveAwayOnRegister)
            {
                menu.MoveAway(0);
            }
            else
            {
                currentMenu = menu;
                currentMenu.MoveInScreen(0);
            }
        }
    }
    
    // For UnityUI buttons.
    public void NavigateMenu(ManagedMenu newMenu) => NavigateMenu(newMenu, false);

    // ReSharper disable once MethodOverloadWithOptionalParameter
    public void NavigateMenu(ManagedMenu newMenu, bool keepPreviousActive = false)
    {
        if (newMenu == currentMenu) return;
        
        if (!keepPreviousActive)
            if (!currentMenu.MoveAway())
                return;

        previousMenu = currentMenu;

        if (newMenu.MoveInScreen())
        {
            currentMenu = newMenu;
            navigationHistory.Push(newMenu);
        }
    }

    public ManagedMenu GetCurrentMenu() => currentMenu;

    public void Return() => NavigateMenu(previousMenu ? previousMenu : fallbackMenu);
}
