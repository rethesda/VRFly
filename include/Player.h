#include "Utils.h"
#include "Settings.h"

using namespace SKSE;

class PlayerState {
public:
    RE::Actor* player;

    RE::TESObjectWEAP* leftWeap;
    RE::MagicItem* leftSpell;
    RE::TESObjectWEAP* rightWeap;
    RE::MagicItem* rightSpell;

    bool setVelocity;
    RE::hkVector4 velocity;

    PlayerState() : player(nullptr), setVelocity(false){}

    static PlayerState& GetSingleton() {
        static PlayerState singleton;
        if (singleton.player == nullptr) {
            // Get player
            auto playerCh = RE::PlayerCharacter::GetSingleton();
            if (!playerCh) {
                log::error("Can't get player!");
            }

            auto playerActor = static_cast<RE::Actor*>(playerCh);
            auto playerRef = static_cast<RE::TESObjectREFR*>(playerCh);
            if (!playerActor || !playerRef) {
                log::error("Fail to cast player to Actor or ObjRef");
            }
            singleton.player = playerActor;
        }
        

        return singleton;
    }

    void UpdateEquip() {
        auto equipR = player->GetEquippedObject(false);
        rightSpell = nullptr;
        rightWeap = nullptr;
        if (equipR) {
            if (auto equipWeapR = equipR->As<RE::TESObjectWEAP>(); equipWeapR) {
                rightWeap = equipWeapR;
            } else if (auto equipSpellR = equipR->As<RE::MagicItem>(); equipSpellR && equipR->IsMagicItem()) {
                rightSpell = equipSpellR;
            }
        }

        auto equipL = player->GetEquippedObject(true);
        leftSpell = nullptr;
        leftWeap = nullptr;
        if (equipL) {
            if (auto equipWeapL = equipL->As<RE::TESObjectWEAP>(); equipWeapL) {
                leftWeap = equipWeapL;
            } else if (auto equipSpellL = equipL->As<RE::MagicItem>(); equipSpellL && equipL->IsMagicItem()) {
                leftSpell = equipSpellL;
            }
        }
    }

    void SetVelocity(float x, float y, float z) {
        velocity = RE::hkVector4(x, y, z, 0.0f);
        setVelocity = true;

        //if (player->GetCharController()) {
        //    log::trace("About to set player velocity to: {}, {}, {}", x, y, z);
        //    player->GetCharController()->SetLinearVelocityImpl(velocity);
        //}
    }

    void StopSetVelocity() { setVelocity = false;
    }

    void CancelFall() {
         if (player->GetCharController()) {
            player->GetCharController()->fallStartHeight = 0.0f;
            player->GetCharController()->fallTime = 0.0f;
        }
    }
};