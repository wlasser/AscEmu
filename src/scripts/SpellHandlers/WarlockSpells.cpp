/*
 * Copyright (c) 2014-2020 AscEmu Team <http://www.ascemu.org>
 * Copyright (c) 2007-2015 Moon++ Team <http://www.moonplusplus.info>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
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
 */

#include "Setup.h"
#include "Units/Stats.h"
#include "Storage/MySQLDataStore.hpp"
#include "Map/MapMgr.h"
#include "Spell/SpellMgr.h"
#include "Spell/SpellAuras.h"
#include <Spell/Definitions/PowerType.h>
#include <Units/Creatures/Pet.h>

 //////////////////////////////////////////////////////////////
 //bool SoulLinkParent( uint32 i, Spell *s )
 //
 //Precondition(s)
 //  Caster is a Player.
 //  Target is a Unit, a warlock pet
 //
 //Effect(s)
 //  Makes the warlock pet cast the Soul Link owner aree aura
 //
 //
 /////////////////////////////////////////////////////////////
bool SoulLinkParent(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
    {
        return true;
    }

    Unit* u = s->GetUnitTarget();
    if (u == nullptr)
    {
        return true;
    }

    u->castSpell(u, 25228, true);

    return true;
}

bool LifeTap(uint8_t effectIndex, Spell* s)
{
    Player* playerTarget = s->GetPlayerTarget();

    if (!s->p_caster || !playerTarget)
    {
        return false;
    }

    uint32 mod;    // spirit bonus coefficient multiplied by 2
    if (s->getSpellInfo()->getId() == 1454)
        mod = 2;
    else if (s->getSpellInfo()->getId() == 1455)
        mod = 3;
    else if (s->getSpellInfo()->getId() == 1456)
        mod = 4;
    else if (s->getSpellInfo()->getId() == 11687)
        mod = 5;
    else
        mod = 6;

    uint32 damage = s->getSpellInfo()->getEffectBasePoints(effectIndex) + 1 + mod * playerTarget->getStat(STAT_SPIRIT) / 2;

    if (damage >= playerTarget->getHealth())
        return false;

    s->p_caster->DealDamage(playerTarget, damage, 0, 0, s->getSpellInfo()->getId());
    damage = damage * (100 + playerTarget->m_lifetapbonus) / 100;    // Apply improved life tap
    if (playerTarget->getPower(POWER_TYPE_MANA) + damage > playerTarget->getMaxPower(POWER_TYPE_MANA))
        playerTarget->setPower(POWER_TYPE_MANA, playerTarget->getMaxPower(POWER_TYPE_MANA));
    else
        playerTarget->setPower(POWER_TYPE_MANA, playerTarget->getPower(POWER_TYPE_MANA) + damage);
    s->SendHealManaSpellOnPlayer(s->p_caster, playerTarget, damage, POWER_TYPE_MANA, s->getSpellInfo()->getId());

    return true;
}

bool SoulShatter(uint8_t /*effectIndex*/, Spell* s)
{
    Unit* unitTarget = s->GetUnitTarget();

    if (!s->u_caster || !s->u_caster->isAlive() || !unitTarget || !unitTarget->isAlive())
        return false;

    s->u_caster->castSpell(unitTarget, 32835, false);

    return true;
}

bool MinorHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18692))
        s->CreateItem(19004);
    else if (s->p_caster->HasSpell(18693))
        s->CreateItem(19005);
    else
        s->CreateItem(5512);

    return true;
}

bool LesserHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(19007);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(19006);
    else
        s->p_caster->getItemInterface()->AddItemById(5511, 1, 0);
    return true;
}

bool HealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(19009);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(19008);
    else
        s->CreateItem(5509);

    return true;
}

bool GreaterHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(19011);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(19010);
    else
        s->CreateItem(5510);

    return true;
}

bool MajorHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(19013);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(19012);
    else
        s->CreateItem(9421);

    return true;
}

bool MasterHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(22105);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(22104);
    else
        s->CreateItem(22103);

    return true;
}

bool DemonicHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(36891);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(36890);
    else
        s->CreateItem(36889);

    return true;
}

bool FelHealthStone(uint8_t /*effectIndex*/, Spell* s)
{
    if (s->p_caster == nullptr)
        return false;

    if (s->p_caster->HasSpell(18693))    // Improved Healthstone (2)
        s->CreateItem(36894);
    else if (s->p_caster->HasSpell(18692))    // Improved Healthstone (1)
        s->CreateItem(36893);
    else
        s->CreateItem(36892);

    return true;
}

bool MasterDemonologist1(uint8_t /*effectIndex*/, Spell* s)
{
    Unit* unitTarget = s->GetUnitTarget();

    if (!s->p_caster || !unitTarget)
        return false; //can't imagine how this talent got to anybody else then a player casting on pet

    uint32 casted_spell_id = 0;
    uint32 inc_resist_by_level = 0;
    uint32 inc_resist_by_level_spell = 0;

    if (unitTarget->getEntry() == 416) //in case it is imp
        casted_spell_id = 23759;
    else if (unitTarget->getEntry() == 1860) //VoidWalker
        casted_spell_id = 23760;
    else if (unitTarget->getEntry() == 1863) //Succubus
        casted_spell_id = 23761;
    else if (unitTarget->getEntry() == 417) //Felhunter
    {
        casted_spell_id = 0;
        inc_resist_by_level_spell = 23762;
        inc_resist_by_level = 20;
    }
    else if (unitTarget->getEntry() == 17252) //Felguard
    {
        casted_spell_id = 35702;
        inc_resist_by_level_spell = 23762;
        inc_resist_by_level = 10;
    }
    if (casted_spell_id)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(s->p_caster, sSpellMgr.getSpellInfo(casted_spell_id), true, NULL);
        SpellCastTargets tgt(s->p_caster->getGuid());
        sp->prepare(&tgt);

        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(casted_spell_id), true, NULL);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    if (inc_resist_by_level_spell)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(s->p_caster, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, NULL);
        SpellCastTargets tgt(s->p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, NULL);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    return true;
}

bool MasterDemonologist2(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;
    Unit* unitTarget = s->GetUnitTarget();

    if (!p_caster || !unitTarget)
        return false; //can't imagine how this talent got to anybody else then a player casting on pet

    uint32 casted_spell_id = 0;
    uint32 inc_resist_by_level = 0;
    uint32 inc_resist_by_level_spell = 0;

    if (unitTarget->getEntry() == 416) //in case it is imp
        casted_spell_id = 23826;
    else if (unitTarget->getEntry() == 1860) //VoidWalker
        casted_spell_id = 23841;
    else if (unitTarget->getEntry() == 1863) //Succubus
        casted_spell_id = 23833;
    else if (unitTarget->getEntry() == 417) //Felhunter
    {
        casted_spell_id = 1;
        inc_resist_by_level_spell = 23837;
        inc_resist_by_level = 40;
    }
    else if (unitTarget->getEntry() == 17252) //Felguard
    {
        casted_spell_id = 35703;
        inc_resist_by_level_spell = 23837;
        inc_resist_by_level = 20;
    }
    if (casted_spell_id)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }
    if (inc_resist_by_level_spell)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);

        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, NULL);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    return true;
}

bool MasterDemonologist3(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;
    Unit* unitTarget = s->GetUnitTarget();

    if (!p_caster || !unitTarget)
        return false; //can't imagine how this talent got to anybody else then a player casting on pet

    uint32 casted_spell_id = 0;
    uint32 inc_resist_by_level = 0;
    uint32 inc_resist_by_level_spell = 0;

    if (unitTarget->getEntry() == 416) //in case it is imp
        casted_spell_id = 23827;
    else if (unitTarget->getEntry() == 1860) //VoidWalker
        casted_spell_id = 23842;
    else if (unitTarget->getEntry() == 1863) //Succubus
        casted_spell_id = 23834;
    else if (unitTarget->getEntry() == 417) //Felhunter
    {
        casted_spell_id = 0;
        inc_resist_by_level_spell = 23838;
        inc_resist_by_level = 60;
    }
    else if (unitTarget->getEntry() == 17252) //Felguard
    {
        casted_spell_id = 35704;
        inc_resist_by_level_spell = 23838;
        inc_resist_by_level = 30;
    }
    if (casted_spell_id)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }
    if (inc_resist_by_level_spell)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    return true;
}

bool MasterDemonologist4(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;
    Unit* unitTarget = s->GetUnitTarget();

    if (!p_caster || !unitTarget)
        return false; //can't imagine how this talent got to anybody else then a player casting on pet

    uint32 casted_spell_id = 0;
    uint32 inc_resist_by_level = 0;
    uint32 inc_resist_by_level_spell = 0;

    if (unitTarget->getEntry() == 416) //in case it is imp
        casted_spell_id = 23828;
    else if (unitTarget->getEntry() == 1860) //VoidWalker
        casted_spell_id = 23843;
    else if (unitTarget->getEntry() == 1863) //Succubus
        casted_spell_id = 23835;
    else if (unitTarget->getEntry() == 417) //Felhunter
    {
        casted_spell_id = 0;
        inc_resist_by_level_spell = 23839;
        inc_resist_by_level = 80;
    }
    else if (unitTarget->getEntry() == 17252) //Felguard
    {
        casted_spell_id = 35705;
        inc_resist_by_level_spell = 23839;
        inc_resist_by_level = 40;
    }
    if (casted_spell_id)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }
    if (inc_resist_by_level_spell)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    return true;
}

bool MasterDemonologist5(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;
    Unit* unitTarget = s->GetUnitTarget();

    if (!p_caster || !unitTarget)
        return false; //can't imagine how this talent got to anybody else then a player casting on pet

    uint32 casted_spell_id = 0;
    uint32 inc_resist_by_level = 0;
    uint32 inc_resist_by_level_spell = 0;

    if (unitTarget->getEntry() == 416) //in case it is imp
        casted_spell_id = 23829;
    else if (unitTarget->getEntry() == 1860) //VoidWalker
        casted_spell_id = 23844;
    else if (unitTarget->getEntry() == 1863) //Succubus
        casted_spell_id = 23836;
    else if (unitTarget->getEntry() == 417) //Felhunter
    {
        casted_spell_id = 0;
        inc_resist_by_level_spell = 23840;
        inc_resist_by_level = 100;
    }
    else if (unitTarget->getEntry() == 17252) //Felguard
    {
        casted_spell_id = 35706;
        inc_resist_by_level_spell = 23840;
        inc_resist_by_level = 50;
    }
    if (casted_spell_id)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(casted_spell_id), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    if (inc_resist_by_level_spell)
    {
        //for self
        Spell* sp = sSpellMgr.newSpell(p_caster, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt(p_caster->getGuid());
        sp->prepare(&tgt);
        //for pet
        sp = sSpellMgr.newSpell(unitTarget, sSpellMgr.getSpellInfo(inc_resist_by_level_spell), true, nullptr);
        SpellCastTargets tgt1(unitTarget->getGuid());
        sp->prepare(&tgt1);
    }

    return true;
}

bool SummonSuccubusQuest(uint8_t /*effectIndex*/, Spell* s)
{
    CreatureProperties const* cp = sMySQLStore.getCreatureProperties(5677);
    if (cp == nullptr)
        return false;

    Creature* pCreature = s->p_caster->GetMapMgr()->CreateCreature(cp->Id);
    pCreature->Load(cp, s->p_caster->GetPositionX(), s->p_caster->GetPositionY(), s->p_caster->GetPositionZ());
    pCreature->GetAIInterface()->Init(pCreature, AI_SCRIPT_AGRO, Movement::WP_MOVEMENT_SCRIPT_NONE);
    pCreature->GetAIInterface()->taunt(s->p_caster, true);
    pCreature->PushToWorld(s->p_caster->GetMapMgr());
    pCreature->Despawn(60000, 0);

    return true;
}

bool SummonVoidWalkerQuest(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;

    CreatureProperties const* cp = sMySQLStore.getCreatureProperties(5676);
    if (cp == nullptr)
        return false;

    Creature* pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
    pCreature->Load(cp, p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ());
    pCreature->GetAIInterface()->Init(pCreature, AI_SCRIPT_AGRO, Movement::WP_MOVEMENT_SCRIPT_NONE);
    pCreature->GetAIInterface()->taunt(p_caster, true);
    pCreature->PushToWorld(p_caster->GetMapMgr());
    pCreature->Despawn(60000, 0);

    return true;
}

bool SummonFelHunterQuest(uint8_t /*effectIndex*/, Spell* s)
{
    Player* p_caster = s->p_caster;

    CreatureProperties const* cp = sMySQLStore.getCreatureProperties(6268);
    if (cp == nullptr)
        return false;

    Creature* pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
    pCreature->Load(cp, p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ());
    pCreature->GetAIInterface()->Init(pCreature, AI_SCRIPT_AGRO, Movement::WP_MOVEMENT_SCRIPT_NONE);
    pCreature->GetAIInterface()->taunt(p_caster, true);
    pCreature->PushToWorld(p_caster->GetMapMgr());
    pCreature->Despawn(60000, 0);

    return true;
}

bool DemonicKnowledge(uint8_t effectIndex, Aura* a, bool apply)
{
    Unit* m_target = a->GetTarget();

    if (m_target->isPet())
    {
        Player* PetOwner = static_cast<Pet*>(m_target)->getPlayerOwner();
        if (PetOwner != nullptr)
        {
            uint32 val1 = m_target->getStat(STAT_STAMINA);
            uint32 val2 = m_target->getStat(STAT_INTELLECT);
            uint32 val0 = val1 + val2;
            float dmginc = (float)(val0 * a->GetModAmount(effectIndex)) / 100;

            int32 val;

            if (apply)
                val = (int32)dmginc;
            else
                val = (int32)-dmginc;

            for (uint16_t x = 0; x < 7; x++)
                PetOwner->modModDamageDonePositive(x, val);

            PetOwner->CalcDamage();
        }
    }

    return true;
}

bool ImprovedLifeTap(uint8_t effectIndex, Aura* a, bool apply)
{
    Unit* u_target = a->GetTarget();
    if (!u_target->isPlayer())
        return true;

    Player* p_target = static_cast<Player*>(u_target);
    int32 amount = a->GetModAmount(effectIndex);


    if (apply)
        p_target->m_lifetapbonus = amount;
    else
        p_target->m_lifetapbonus = 0;

    return true;
}

bool SoulSiphon(uint8_t effectIndex, Aura* a, bool apply)
{
    Unit* caster = a->GetUnitCaster();
    int32 amount = a->GetModAmount(effectIndex);

    if (caster)
    {
        if (apply)
            caster->m_soulSiphon.amt += amount;
        else
            caster->m_soulSiphon.amt -= amount;
    }

    return true;
}

bool SoulStoneResurrection(uint8_t /*effectIndex*/, Aura* a, bool apply)
{
    Unit* u_target = a->GetTarget();
    if (!u_target->isPlayer())
        return true;

    Player* p_target = static_cast<Player*>(u_target);
    uint32 soulstone = a->GetSpellInfo()->getEffectMiscValue(0);

    if (apply)
    {
        p_target->SetSoulStone(soulstone);
        p_target->SetSoulStoneReceiver((uint32)a->m_casterGuid);
    }
    else if (p_target->isAlive())
    {
        p_target->SetSoulStone(0);
        p_target->SetSoulStoneReceiver(0);
    }
    return true;
}

bool DemonicCircleSummon(uint8_t /*effectIndex*/, Aura* a, bool apply)
{
    Unit* m_target = a->GetTarget();

    if (m_target->GetMapMgr() == nullptr)
        return true;

    if (apply)
    {

        GameObject* circle = m_target->GetMapMgr()->GetGameObject(a->GetTarget()->m_ObjectSlots[0]);
        SpellInfo const* sp = sSpellMgr.getSpellInfo(48020);

        if (circle != NULL && sp != NULL && m_target->CalcDistance(circle) <= GetMaxRange(sSpellRangeStore.LookupEntry(sp->getRangeIndex())))
        {
            if (!m_target->HasAura(62388))
                m_target->castSpell(m_target, 62388, true);
        }
        else
            m_target->RemoveAura(62388);
    }
    else
    {
        m_target->removeAllAurasById(62388);
    }

    return true;
}

void SetupWarlockSpells(ScriptMgr* mgr)
{
    //////////////////////////////////////////////////////////////////////////////////////////
    // Dummy Effect

    mgr->register_dummy_spell(19028, &SoulLinkParent);

    mgr->register_dummy_spell(1454, &LifeTap);
    mgr->register_dummy_spell(1455, &LifeTap);
    mgr->register_dummy_spell(1456, &LifeTap);
    mgr->register_dummy_spell(11687, &LifeTap);
    mgr->register_dummy_spell(11688, &LifeTap);
    mgr->register_dummy_spell(11689, &LifeTap);
    mgr->register_dummy_spell(27222, &LifeTap);
    mgr->register_dummy_spell(57946, &LifeTap);

    mgr->register_dummy_spell(29858, &SoulShatter);

    //////////////////////////////////////////////////////////////////////////////////////////
    // Scripted Effect

    mgr->register_script_effect(6201, &MinorHealthStone);

    mgr->register_script_effect(6202, &LesserHealthStone);

    mgr->register_script_effect(5699, &HealthStone);

    mgr->register_script_effect(11729, &GreaterHealthStone);

    mgr->register_script_effect(11730, &MajorHealthStone);

    mgr->register_script_effect(27230, &MasterHealthStone);

    mgr->register_script_effect(47871, &DemonicHealthStone);

    mgr->register_script_effect(47878, &FelHealthStone);

    mgr->register_script_effect(23784, &MasterDemonologist1);

    mgr->register_script_effect(23830, &MasterDemonologist2);

    mgr->register_script_effect(23831, &MasterDemonologist3);

    mgr->register_script_effect(23832, &MasterDemonologist4);

    mgr->register_script_effect(35708, &MasterDemonologist5);

    mgr->register_script_effect(8674, &SummonSuccubusQuest);
    mgr->register_script_effect(9223, &SummonSuccubusQuest);
    mgr->register_script_effect(9224, &SummonSuccubusQuest);

    mgr->register_script_effect(7728, &SummonVoidWalkerQuest);
    mgr->register_script_effect(9221, &SummonVoidWalkerQuest);
    mgr->register_script_effect(9222, &SummonVoidWalkerQuest);
    mgr->register_script_effect(30208, &SummonVoidWalkerQuest);

    mgr->register_script_effect(8712, &SummonFelHunterQuest);

    mgr->register_dummy_aura(35696, &DemonicKnowledge);

    uint32 improvedlifetapids[] =
    {
        18182,
        18183,
        0
    };
    mgr->register_dummy_aura(improvedlifetapids, &ImprovedLifeTap);

    uint32 soulsiphonids[] =
    {
        17804,
        17805,
        0
    };
    mgr->register_dummy_aura(soulsiphonids, &SoulSiphon);

    uint32 soulstoneresurrectionids[] =
    {
        20707,
        20762,
        20763,
        20764,
        20765,
        27239,
        47883,
        0
    };
    mgr->register_dummy_aura(soulstoneresurrectionids, &SoulStoneResurrection);

    mgr->register_dummy_aura(48018, &DemonicCircleSummon);
}
