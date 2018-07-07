/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2005-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
#include "Management/Item.h"
#include "Management/ItemInterface.h"
#include "Storage/MySQLDataStore.hpp"
#include "Map/MapMgr.h"
#include "Units/Creatures/Pet.h"
#include "Server/Packets/SmsgBindPointUpdate.h"
#include "Server/Packets/SmsgSetProficiency.h"
#include "Server/Packets/SmsgTutorialFlags.h"
#include "Server/Packets/SmsgInstanceDifficulty.h"
#include "Server/Packets/SmsgInitialSpells.h"
#include "Server/Packets/SmsgDestoyObject.h"
#include "Server/Packets/MsgSetRaidDifficulty.h"
#include "Server/Packets/MsgSetDungeonDifficulty.h"

using namespace AscEmu::Packets;

void Player::SendTalentResetConfirm()
{
    WorldPacket data(MSG_TALENT_WIPE_CONFIRM, 12);

    data << uint64(getGuid());
    data << uint32(CalcTalentResetCost(GetTalentResetTimes()));

    m_session->SendPacket(&data);
}

void Player::SendPetUntrainConfirm()
{
    Pet* pPet = GetSummon();

    if (pPet == NULL)
        return;

    WorldPacket data(SMSG_PET_UNLEARN_CONFIRM, 12);

    data << uint64(pPet->getGuid());
    data << uint32(pPet->GetUntrainCost());

    m_session->SendPacket(&data);
}

void Player::SendWorldStateUpdate(uint32 WorldState, uint32 Value)
{
    WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);

    data << uint32(WorldState);
    data << uint32(Value);

    m_session->SendPacket(&data);
}

void Player::Gossip_SendPOI(float X, float Y, uint32 Icon, uint32 Flags, uint32 Data, const char* Name)
{
    size_t namelen = 0;

    if (Name != NULL)
        namelen = strlen(Name);

    WorldPacket data(SMSG_GOSSIP_POI, 21 + namelen);

    data << uint32(Flags);
    data << float(X);
    data << float(Y);
    data << uint32(Icon);
    data << uint32(Data);

    if (namelen == 0)
        data << uint8(0);
    else
        data.append((const uint8*)Name, namelen + 1);

    m_session->SendPacket(&data);
}

void Player::SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4)
{
    WorldPacket data(SMSG_LEVELUP_INFO, 14 * 4);

    data << uint32(level);
    data << uint32(Hp);
    data << uint32(Mana);

    for (uint8 i = 0; i < 6; ++i)
        data << uint32(0);

    data << uint32(Stat0);
    data << uint32(Stat1);
    data << uint32(Stat2);
    data << uint32(Stat3);
    data << uint32(Stat4);

    m_session->SendPacket(&data);
}

void Player::SendLogXPGain(uint64 guid, uint32 NormalXP, uint32 RestedXP, bool type)
{
    WorldPacket data(SMSG_LOG_XPGAIN, 24);

    if (type == false)
    {
        data << uint64(guid);
        data << uint32(NormalXP);
        data << uint8(0);

        data << uint32(RestedXP);
        data << float(1.0f);

    }
    else if (type == true)
    {
        data << uint64(0);            // does not need to be set for questxp
        data << uint32(NormalXP);
        data << uint8(1);

        data << uint8(0);
    }
    m_session->SendPacket(&data);
}

void Player::SendCastResult(uint32 SpellId, uint8 ErrorMessage, uint8 MultiCast, uint32 Extra)
{
    WorldPacket data(SMSG_CAST_FAILED, 80);

    data << uint8(MultiCast);
    data << uint32(SpellId);
    data << uint8(ErrorMessage);

    if (Extra)
        data << uint32(Extra);

    m_session->SendPacket(&data);
}

void Player::SendSpellCooldownEvent(uint32 SpellId)
{
    WorldPacket data(SMSG_COOLDOWN_EVENT, 12);

    data << uint32(SpellId);
    data << uint64(getGuid());

    m_session->SendPacket(&data);
}


void Player::SendSpellModifier(uint8 spellgroup, uint8 spelltype, int32 v, bool is_pct)
{
    WorldPacket data((uint16)(is_pct ? SMSG_SET_PCT_SPELL_MODIFIER : SMSG_SET_FLAT_SPELL_MODIFIER), 48);
    data << uint8(spellgroup);
    data << uint8(spelltype);
    data << uint32(v);

    m_session->SendPacket(&data);
}

void Player::SendItemPushResult(bool created, bool recieved, bool sendtoset, bool newitem, uint8 destbagslot, uint32 destslot, uint32 count, uint32 entry, uint32 suffix, uint32 randomprop, uint32 stack)
{
    WorldPacket data(SMSG_ITEM_PUSH_RESULT, 8 + 4 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 4);
    data << uint64(getGuid());

    if (recieved)
        data << uint32(1);
    else
        data << uint32(0);

    if (created)
        data << uint32(1);
    else
        data << uint32(0);

    data << uint32(1);
    data << uint8(destbagslot);

    if (newitem)
        data << uint32(destslot);
    else
        data << uint32(-1);

    data << uint32(entry);
    data << uint32(suffix);
    data << uint32(randomprop);
    data << uint32(count);
    data << uint32(stack);

    if (sendtoset && InGroup())
        GetGroup()->SendPacketToAll(&data);
    else
        m_session->SendPacket(&data);

}

void Player::SendSetProficiency(uint8 ItemClass, uint32 Proficiency)
{
    WorldPacket data(SMSG_SET_PROFICIENCY, 40);
    data << uint8(ItemClass);
    data << uint32(Proficiency);

    m_session->SendPacket(&data);
}

void Player::SendLoginVerifyWorld(uint32 MapId, float X, float Y, float Z, float O)
{
    WorldPacket data(SMSG_LOGIN_VERIFY_WORLD, 20);
    data << uint32(MapId);
    data << float(X);
    data << float(Y);
    data << float(Z);
    data << float(O);

    m_session->SendPacket(&data);
}

void Player::SendDungeonDifficulty()
{
    m_session->SendPacket(MsgSetDungeonDifficulty(m_RaidDifficulty, 1, InGroup()).serialise().get());
}

void Player::SendRaidDifficulty()
{
#if VERSION_STRING > TBC
    m_session->SendPacket(MsgSetRaidDifficulty(m_RaidDifficulty, 1, InGroup()).serialise().get());
#endif
}

void Player::SendInstanceDifficulty(uint8 difficulty)
{
    WorldPacket data(SMSG_INSTANCE_DIFFICULTY, 8);
    data << uint32(difficulty);
    data << uint32(0);

    m_session->SendPacket(&data);
}

void Player::SendNewDrunkState(uint32 state, uint32 itemid)
{
    WorldPacket data(SMSG_CROSSED_INEBRIATION_THRESHOLD, (8 + 4 + 4));
    data << getGuid();
    data << uint32(state);
    data << uint32(itemid);

    SendMessageToSet(&data, true);
}

/*Loot type MUST be
1-corpse, go
2-skinning/herbalism/minning
3-Fishing
*/
void Player::SendLoot(uint64 guid, uint8 loot_type, uint32 mapid)
{
    Group* m_Group = m_playerInfo->m_Group;

    if (!IsInWorld())
        return;

    Loot* pLoot = NULL;
    uint32 guidtype = GET_TYPE_FROM_GUID(guid);

    int8 loot_method;

    if (m_Group != NULL)
        loot_method = m_Group->GetMethod();
    else
        loot_method = PARTY_LOOT_FFA;

    if (guidtype == HIGHGUID_TYPE_UNIT)
    {
        Creature* pCreature = GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
        if (!pCreature)return;
        pLoot = &pCreature->loot;
        m_currentLoot = pCreature->getGuid();

    }
    else if (guidtype == HIGHGUID_TYPE_GAMEOBJECT)
    {
        GameObject* pGO = GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
        if (!pGO)
            return;

        if (!pGO->IsLootable())
            return;

        GameObject_Lootable* pLGO = static_cast<GameObject_Lootable*>(pGO);
        pLGO->setState(0);
        pLoot = &pLGO->loot;
        m_currentLoot = pLGO->getGuid();
    }
    else if ((guidtype == HIGHGUID_TYPE_PLAYER))
    {
        Player* p = GetMapMgr()->GetPlayer((uint32)guid);
        if (!p)return;
        pLoot = &p->loot;
        m_currentLoot = p->getGuid();
    }
    else if ((guidtype == HIGHGUID_TYPE_CORPSE))
    {
        Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
        if (!pCorpse)return;
        pLoot = &pCorpse->loot;
        m_currentLoot = pCorpse->getGuid();
    }
    else if ((guidtype == HIGHGUID_TYPE_ITEM))
    {
        Item* pItem = GetItemInterface()->GetItemByGUID(guid);
        if (!pItem)
            return;
        pLoot = pItem->loot;
        m_currentLoot = pItem->getGuid();
    }

    if (!pLoot)
    {
        // something whack happened.. damn cheaters..
        return;
    }

    // add to looter set
    pLoot->looters.insert(getGuidLow());

    WorldPacket data, data2(32);
    data.SetOpcode(SMSG_LOOT_RESPONSE);

    m_lootGuid = guid;

    data << uint64(guid);
    data << uint8(loot_type);  //loot_type;
    data << uint32(pLoot->gold);
    data << uint8(0);   //loot size reserve
#if VERSION_STRING == Cata
    data << uint8(0);
#endif

    std::vector<__LootItem>::iterator iter = pLoot->items.begin();
    uint32 count = 0;
    uint8 slottype = 0;

    for (uint32 x = 0; iter != pLoot->items.end(); ++iter, x++)
    {
        if (iter->iItemsCount == 0)
            continue;

        LooterSet::iterator itr = iter->has_looted.find(getGuidLow());
        if (iter->has_looted.end() != itr)
            continue;

        ItemProperties const* itemProto = iter->item.itemproto;
        if (!itemProto)
            continue;

        // check if it's on ML if so only quest items and ffa loot should be shown based on mob
        if (loot_method == PARTY_LOOT_MASTER && m_Group && m_Group->GetLooter() != m_playerInfo)
            // pass on all ffa_loot and the grey / white items
            if (!iter->ffa_loot && !(itemProto->Quality < m_Group->GetThreshold()))
                continue;

        // team check
        if (itemProto->HasFlag2(ITEM_FLAG2_HORDE_ONLY) && IsTeamAlliance())
            continue;

        if (itemProto->HasFlag2(ITEM_FLAG2_ALLIANCE_ONLY) && IsTeamHorde())
            continue;

        //quest items check. type 4/5
        //quest items that don't start quests.
        if ((itemProto->Bonding == ITEM_BIND_QUEST) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
            continue;
        if ((itemProto->Bonding == ITEM_BIND_QUEST2) && !(itemProto->QuestId) && !HasQuestForItem(itemProto->ItemId))
            continue;

        //quest items that start quests need special check to avoid drops all the time.
        if ((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasQuest(itemProto->QuestId))
            continue;
        if ((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasQuest(itemProto->QuestId))
            continue;

        if ((itemProto->Bonding == ITEM_BIND_QUEST) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;
        if ((itemProto->Bonding == ITEM_BIND_QUEST2) && (itemProto->QuestId) && HasFinishedQuest(itemProto->QuestId))
            continue;

        //check for starting item quests that need questlines.
        if ((itemProto->QuestId && itemProto->Bonding != ITEM_BIND_QUEST && itemProto->Bonding != ITEM_BIND_QUEST2))
        {
            QuestProperties const* pQuest = sMySQLStore.getQuestProperties(itemProto->QuestId);
            if (pQuest)
            {
                uint32 finishedCount = 0;

                //check if its a questline.
                for (uint32 i = 0; i < pQuest->count_requiredquests; ++i)
                {
                    if (pQuest->required_quests[i])
                    {
                        if (!HasFinishedQuest(pQuest->required_quests[i]) || HasQuest(pQuest->required_quests[i]))
                        {

                        }
                        else
                        {
                            finishedCount++;
                        }
                    }
                }
            }
        }

        slottype = 0;
        if (m_Group != NULL && loot_type < 2)
        {
            switch (loot_method)
            {
                case PARTY_LOOT_MASTER:
                    slottype = 2;
                    break;
                case PARTY_LOOT_GROUP:
                case PARTY_LOOT_RR:
                case PARTY_LOOT_NBG:
                    slottype = 1;
                    break;
                default:
                    slottype = 0;
                    break;
            }
            // only quality items are distributed
            if (itemProto->Quality < m_Group->GetThreshold())
            {
                slottype = 0;
            }

            // if all people passed anyone can loot it? :P
            if (iter->passed)
                slottype = 0;					// All players passed on the loot

            //if it is ffa loot and not an masterlooter
            if (iter->ffa_loot)
                slottype = 0;
        }

        data << uint8(x);
        data << uint32(itemProto->ItemId);
        data << uint32(iter->iItemsCount);  //nr of items of this type
        data << uint32(iter->item.displayid);

        if (iter->iRandomSuffix)
        {
            data << uint32(Item::GenerateRandomSuffixFactor(itemProto));
            data << uint32(-int32(iter->iRandomSuffix->id));
        }
        else if (iter->iRandomProperty)
        {
            data << uint32(0);
            data << uint32(iter->iRandomProperty->ID);
        }
        else
        {
            data << uint32(0);
            data << uint32(0);
        }

        data << slottype;   // "still being rolled for" flag

        if (slottype == 1)
        {
            if (iter->roll == NULL && !iter->passed)
            {
                int32 ipid = 0;
                uint32 factor = 0;
                if (iter->iRandomProperty)
                    ipid = iter->iRandomProperty->ID;
                else if (iter->iRandomSuffix)
                {
                    ipid = -int32(iter->iRandomSuffix->id);
                    factor = Item::GenerateRandomSuffixFactor(iter->item.itemproto);
                }

                if (iter->item.itemproto)
                {
                    iter->roll = new LootRoll(60000, m_Group->MemberCount(), guid, x, itemProto->ItemId, factor, uint32(ipid), GetMapMgr());

                    data2.Initialize(SMSG_LOOT_START_ROLL);
                    data2 << guid;
                    data2 << uint32(mapid);
                    data2 << uint32(x);
                    data2 << uint32(itemProto->ItemId);
                    data2 << uint32(factor);
                    if (iter->iRandomProperty)
                        data2 << uint32(iter->iRandomProperty->ID);
                    else if (iter->iRandomSuffix)
                        data2 << uint32(ipid);
                    else
                        data2 << uint32(0);

                    data2 << uint32(iter->iItemsCount);
                    data2 << uint32(60000);	// countdown
                    data2 << uint8(7);		// some sort of flags that require research
                }

                Group* pGroup = m_playerInfo->m_Group;
                if (pGroup)
                {
                    pGroup->Lock();
                    for (uint32 i = 0; i < pGroup->GetSubGroupCount(); ++i)
                    {
                        for (GroupMembersSet::iterator itr2 = pGroup->GetSubGroup(i)->GetGroupMembersBegin(); itr2 != pGroup->GetSubGroup(i)->GetGroupMembersEnd(); ++itr2)
                        {

                            PlayerInfo* pinfo = *itr2;

                            if (pinfo->m_loggedInPlayer && pinfo->m_loggedInPlayer->GetItemInterface()->CanReceiveItem(itemProto, iter->iItemsCount) == 0)
                            {
                                if (pinfo->m_loggedInPlayer->m_passOnLoot)
                                    iter->roll->PlayerRolled(pinfo->m_loggedInPlayer, 3);		// passed
                                else
                                    pinfo->m_loggedInPlayer->SendPacket(&data2);
                            }
                        }
                    }
                    pGroup->Unlock();
                }
                else
                {
                    m_session->SendPacket(&data2);
                }
            }
        }
        count++;
    }
    data.wpos(13);
    data << uint8(count);

    m_session->SendPacket(&data);

    addUnitFlags(UNIT_FLAG_LOOTING);
}

#ifdef AE_TBC
// MIT Start
// TODO: Move to own file
void Player::SendInitialLogonPackets()
{
    // SMSG_SET_REST_START
    m_session->OutPacket(SMSG_SET_REST_START, 4, &m_timeLogoff); // Seem to be unused by client

    m_session->SendPacket(SmsgBindPointUpdate(m_bind_pos_x, m_bind_pos_y, m_bind_pos_z, m_bind_mapid, m_bind_zoneid).serialise().get());
    m_session->SendPacket(SmsgSetProficiency(4, armor_proficiency).serialise().get());
    m_session->SendPacket(SmsgSetProficiency(2, weapon_proficiency).serialise().get());

    std::vector<uint32_t> tutorials;
    for (auto tutorial : m_Tutorials)
        tutorials.push_back(tutorial);

    m_session->SendPacket(SmsgTutorialFlags(tutorials).serialise().get());
    // Normal difficulty - TODO implement
    //m_session->SendPacket(SmsgInstanceDifficulty(0).serialise().get());

    smsg_InitialSpells();

    // MIT End

    SendInitialActions();
    smsg_InitialFactions();

    StackWorldPacket<32> data(SMSG_BINDPOINTUPDATE);
    data.Initialize(SMSG_LOGIN_SETTIMESPEED);

    data << uint32(Arcemu::Util::MAKE_GAME_TIME());
    data << float(0.0166666669777748f);    // Normal Game Speed

    m_session->SendPacket(&data);

    UpdateSpeed();
    LOG_DETAIL("WORLD: Sent initial logon packets for %s.", getName().c_str());
}
#endif

#ifndef AE_TBC
void Player::SendInitialLogonPackets()
{
    // Initial Packets... they seem to be re-sent on port.
    //m_session->OutPacket(SMSG_SET_REST_START_OBSOLETE, 4, &m_timeLogoff); // Seem to be unused by client

    StackWorldPacket<32> data(SMSG_BINDPOINTUPDATE);

    data << float(m_bind_pos_x);
    data << float(m_bind_pos_y);
    data << float(m_bind_pos_z);
    data << uint32(m_bind_mapid);
    data << uint32(m_bind_zoneid);

    m_session->SendPacket(&data);

    //Proficiencies
    SendSetProficiency(4, armor_proficiency);
    SendSetProficiency(2, weapon_proficiency);

    //Tutorial Flags
    WorldPacket datab(SMSG_TUTORIAL_FLAGS, 4 * 8);
    for (int i = 0; i < 8; ++i)
        datab << uint32_t(m_Tutorials[i]);
    m_session->SendPacket(&datab);

#if VERSION_STRING > TBC
    smsg_TalentsInfo(false);
#endif

    smsg_InitialSpells();

#if VERSION_STRING > TBC
    data.Initialize(SMSG_SEND_UNLEARN_SPELLS);
    data << uint32(0); // count, for (count) uint32;
    GetSession()->SendPacket(&data);
#endif

    SendInitialActions();
    smsg_InitialFactions();

    data.Initialize(SMSG_LOGIN_SETTIMESPEED);

    data << uint32(Arcemu::Util::MAKE_GAME_TIME());
    data << float(0.0166666669777748f);    // Normal Game Speed
#if VERSION_STRING > TBC
    data << uint32(0);   // 3.1.2
#endif

    m_session->SendPacket(&data);

    UpdateSpeed();

#if VERSION_STRING > TBC
    WorldPacket ArenaSettings(SMSG_UPDATE_WORLD_STATE, 16);

    ArenaSettings << uint32(0xC77);
    ArenaSettings << uint32(worldConfig.arena.arenaProgress);
    ArenaSettings << uint32(0xF3D);
    ArenaSettings << uint32(worldConfig.arena.arenaSeason);

    m_session->SendPacket(&ArenaSettings);
#endif
    LOG_DETAIL("WORLD: Sent initial logon packets for %s.", getName().c_str());
}
#endif

void Player::SendLootUpdate(Object* o)
{
    if (!IsVisible(o->getGuid()))
        return;

    if (o->isCreatureOrPlayer())
    {
        // Build the actual update.
        ByteBuffer buf(500);

        uint32 Flags = static_cast<Unit*>(o)->getDynamicFlags();

        Flags |= U_DYN_FLAG_LOOTABLE;
        Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

        o->BuildFieldUpdatePacket(&buf, UNIT_DYNAMIC_FLAGS, Flags);

        PushUpdateData(&buf, 1);
    }
}

void Player::SendUpdateDataToSet(ByteBuffer* groupbuf, ByteBuffer* nongroupbuf, bool sendtoself)
{
    //////////////////////////////////////////////////////////////////////////////////////////
    // first case we need to send to both grouped and ungrouped players in the set
    if (groupbuf != nullptr && nongroupbuf != nullptr)
    {
        for (const auto& itr : getInRangePlayersSet())
        {
            Player* p = static_cast<Player*>(itr);

            if (p->GetGroup() != nullptr && GetGroup() != nullptr && p->GetGroup()->GetID() == GetGroup()->GetID())
                p->PushUpdateData(groupbuf, 1);
            else
                p->PushUpdateData(nongroupbuf, 1);
        }
    }
    else
    {
        //////////////////////////////////////////////////////////////////////////////////////////
        //second case we send to group only
        if (groupbuf != nullptr && nongroupbuf == nullptr)
        {
            for (const auto& itr : getInRangePlayersSet())
            {
                Player* p = static_cast<Player*>(itr);
                if (p && p->GetGroup() != nullptr && GetGroup() != nullptr && p->GetGroup()->GetID() == GetGroup()->GetID())
                    p->PushUpdateData(groupbuf, 1);
            }
        }
        else
        {
            //////////////////////////////////////////////////////////////////////////////////////////
            //Last case we send to nongroup only
            if (groupbuf == nullptr && nongroupbuf != nullptr)
            {
                for (const auto& itr : getInRangePlayersSet())
                {
                    if (itr)
                    {
                        Player* p = static_cast<Player*>(itr);
                        if (p->GetGroup() == nullptr || p->GetGroup()->GetID() != GetGroup()->GetID())
                            p->PushUpdateData(nongroupbuf, 1);
                    }
                }
            }
        }
    }

    if (sendtoself && groupbuf != nullptr)
        PushUpdateData(groupbuf, 1);
}

void Player::TagUnit(Object* o)
{
    if (o->isCreatureOrPlayer())
    {
        // For new players who get a create object
        uint32 Flags = static_cast<Unit*>(o)->getDynamicFlags();
        Flags |= U_DYN_FLAG_TAPPED_BY_PLAYER;

        // Update existing players.
        ByteBuffer buf(500);
        ByteBuffer buf1(500);

        o->BuildFieldUpdatePacket(&buf1, UNIT_DYNAMIC_FLAGS, Flags);
        o->BuildFieldUpdatePacket(&buf, UNIT_DYNAMIC_FLAGS, o->getUInt32Value(UNIT_DYNAMIC_FLAGS));

        SendUpdateDataToSet(&buf1, &buf, true);
    }
}

void Player::SendPartyKillLog(uint64 GUID)
{
    WorldPacket data(SMSG_PARTYKILLLOG, 16);
    data << getGuid();
    data << GUID;

    SendMessageToSet(&data, true);
}

void Player::SendDestroyObject(uint64 GUID)
{
    m_session->SendPacket(SmsgDestroyObject(GUID).serialise().get());
}


void Player::SendEquipmentSetList()
{
#if VERSION_STRING > TBC
    WorldPacket data(SMSG_EQUIPMENT_SET_LIST, 1000);

    m_ItemInterface->m_EquipmentSets.FillEquipmentSetListPacket(data);
    m_session->SendPacket(&data);

    LOG_DEBUG("Sent SMSG_EQUIPMENT_SET_LIST.");
#endif
}

void Player::SendEquipmentSetSaved(uint32 setID, uint32 setGUID)
{
#if VERSION_STRING > TBC
    WorldPacket data(SMSG_EQUIPMENT_SET_SAVED, 12);
    data << uint32(setID);
    data << WoWGuid(uint64(setGUID));

    m_session->SendPacket(&data);

    LOG_DEBUG("Sent SMSG_EQUIPMENT_SET_SAVED.");
#endif
}

void Player::SendEquipmentSetUseResult(uint8 result)
{
#if VERSION_STRING > TBC
    WorldPacket data(SMSG_EQUIPMENT_SET_USE_RESULT, 1);
    data << uint8(result);

    m_session->SendPacket(&data);

    LOG_DEBUG("SMSG_EQUIPMENT_SET_USE_RESULT sent.");
#endif
}

void Player::SendTotemCreated(uint8 slot, uint64 GUID, uint32 duration, uint32 spellid)
{
    WorldPacket data(SMSG_TOTEM_CREATED, 17);
    data << uint8(slot);
    data << uint64(GUID);
    data << uint32(duration);
    data << uint32(spellid);
    m_session->SendPacket(&data);
}

void Player::SendInitialWorldstates()
{
#if VERSION_STRING < Cata
    WorldPacket data(SMSG_INIT_WORLD_STATES, 100);

    m_mapMgr->GetWorldStatesHandler().BuildInitWorldStatesForZone(m_zoneId, m_AreaID, data);
    m_session->SendPacket(&data);
#endif
}