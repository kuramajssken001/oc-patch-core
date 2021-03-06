#include "ScriptPCH.h"
#include "Config/Config.h"

enum ActNpcRename
{
    ACT_RENAME   = 1001,
    ACT_CONFIRM  = 1002
};

#define GOSSIP_RENAME_HELLO        "I want to change my name and hide a terrible deed."
#define GOSSIP_RENAME_CONFIRM      "Are you sure?"
#define MSG_NOT_ENOUGH_MONEY       "Get out, you are wasting my time!  Come back if you can pay me!"
#define MSG_COMPLETE_RENAME        "Done!  Now log out of the game and sign in again.  Your secret will die with you!"

bool GossipHello_npc_rename(Player* pPlayer, Creature* pCreature)
{
    bool EnableRename = sConfig.GetBoolDefault("RenameNPC.Enable", true);

    if (EnableRename)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RENAME_HELLO, GOSSIP_SENDER_MAIN, ACT_RENAME);
        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    }
    return true;
}

bool GossipSelect_npc_rename(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiSender != GOSSIP_SENDER_MAIN)
        return false;

        uint32 RenameCost = sConfig.GetIntDefault("RenameNPC.CostInGold", 100);
        uint32 RenameCopper = RenameCost * 10000;

    switch (uiAction)
    {
        case ACT_RENAME:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RENAME_CONFIRM, GOSSIP_SENDER_MAIN, ACT_CONFIRM);
            pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
            break;
        case ACT_CONFIRM:
            if (pPlayer->GetMoney() < RenameCopper)
            {
                pCreature->MonsterWhisper(MSG_NOT_ENOUGH_MONEY, pPlayer->GetGUID());
            } else {
                pCreature->MonsterWhisper(MSG_COMPLETE_RENAME, pPlayer->GetGUID());
                pPlayer->ModifyMoney(-RenameCopper);
                pPlayer->SetAtLoginFlag(AT_LOGIN_RENAME);
            }
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        default: 
            pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}

void AddSC_npc_rename()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_rename";
    newscript->pGossipHello = &GossipHello_npc_rename;
    newscript->pGossipSelect = &GossipSelect_npc_rename;
    newscript->RegisterSelf();
}

