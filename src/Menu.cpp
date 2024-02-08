#include "../pch.h"
#include "../include/Menu.hpp"
#include "SDK.hpp"
#include "config.h"
#include <algorithm>
#include <array>
std::string rand_str(const int len)
{
    std::string str;
    char c;
    int idx;
    for (idx = 0; idx < len; idx++)
    {
        c = 'a' + rand() % 26;
        str.push_back(c);
    }
    return str;
}
void Damage(SDK::APalCharacter* character, int32 damage)
{
    SDK::FPalDamageInfo  info = SDK::FPalDamageInfo();
    info.AttackElementType = SDK::EPalElementType::Normal;
    info.Attacker = Config.GetPalPlayerCharacter();
    info.AttackerGroupID = Config.GetPalPlayerState()->IndividualHandleId.PlayerUId;
    info.AttackerLevel = 50;
    info.AttackType = SDK::EPalAttackType::Weapon;
    info.bApplyNativeDamageValue = true;
    info.bAttackableToFriend = true;
    info.IgnoreShield = true;
    info.NativeDamageValue = damage;
    Config.GetPalPlayerState()->SendDamage_ToServer(character, info);
}

void Heal(SDK::APalCharacter* character)
{
    int32 newHealth = 0;
    SDK::UPalCharacterParameterComponent* pParams = character->CharacterParameterComponent;
    if (!pParams)
        return;

    SDK::FFixedPoint64 maxHP = pParams->GetMaxHP();
    newHealth = maxHP.Value;

    SDK::FFixedPoint newHealthPoint = SDK::FFixedPoint(newHealth);
    character->ReviveCharacter_ToServer(newHealthPoint);
    character->ReviveCharacter(newHealthPoint);
}

int InputTextCallback(ImGuiInputTextCallbackData* data) {
    char inputChar = data->EventChar;

    Config.Update(Config.inputTextBuffer);

    return 0;
}

namespace DX11_Base
{
    // helper variables
    char inputBuffer_getFnAddr[100];
    char inputBuffer_getClass[100];
    char inputBuffer_setWaypoint[32];

    namespace Styles
    {
        void InitStyle()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
            style.WindowPadding = ImVec2(15, 15);
            style.WindowRounding = 5.0f;
            style.FramePadding = ImVec2(5, 5);
            style.FrameRounding = 4.0f;
            style.ItemSpacing = ImVec2(12, 8);
            style.ItemInnerSpacing = ImVec2(8, 6);
            style.IndentSpacing = 25.0f;
            style.ScrollbarSize = 15.0f;
            style.ScrollbarRounding = 9.0f;
            style.GrabMinSize = 5.0f;
            style.GrabRounding = 3.0f;
            ImGui::StyleColorsClassic();

            if (g_Menu->dbg_RAINBOW_THEME)
            {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else
            {
                /// YOUR DEFAULT STYLE PROPERTIES HERE
                colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
                colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
                colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
                colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
                colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
                colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
                colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
                colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
                colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
                colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
                colors[ImGuiCol_Tab] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
                colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
                colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
            }
        }
    }


    namespace Tabs
    {
        void TABPlayer()
        {

            //�л�����һ��
            ImGui::Checkbox("Toggle Speed", &Config.IsSpeedHack);

            ImGui::Checkbox("Toggle Atk Up", &Config.IsAttackModiler);

            ImGui::Checkbox("Toggle Def Up", &Config.IsDefuseModiler);

            ImGui::Checkbox("Infinite Health", &Config.IsGodMode);

            ImGui::Checkbox("Infinite Stamina", &Config.IsInfStamina);

            ImGui::Checkbox("Infinite Ammo", &Config.IsInfinAmmo);

            if (ImGui::Checkbox("Toggle FullBright", &Config.IsFullbright))
                SetFullbright(Config.IsFullbright);

            if (ImGui::Button("Spoof Name", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                        std::string s = rand_str(20);

                        wchar_t  ws[255];
                        swprintf(ws, 255, L"%hs", s.c_str());

                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, SDK::FString(ws));
                    }
                }
            }

            //Creadit Mokobake
            //ImGui::Checkbox("MuteKiGodmode", &Config.IsMuteki);
            //             
            ImGui::SliderFloat("Speed Modifier", &Config.SpeedModiflers, 1, 10);
            ImGui::SliderInt("Atk Modifier", &Config.DamageUp, 0, 200000);
            ImGui::SliderInt("Def Modifier", &Config.DefuseUp, 0, 200000);
            ImGui::Text("Player Position");
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    SDK::FVector PlayerLocation = p_appc->K2_GetActorLocation();
                    std::string MyLocation = std::format("X: {} | Y: {} | Z: {}", floor(PlayerLocation.X), floor(PlayerLocation.Y), floor(PlayerLocation.Z));
                    ImGui::Text(MyLocation.c_str());
                    if (ImGui::Button("Print Coords", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                        g_Console->printdbg(MyLocation.c_str(), Console::Colors::green, p_appc);
                }
            }
        }

        void TABExploit()
        {
            //Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestSpawnMonsterForPlayer(name, 5, 1);
            ImGui::Checkbox("Show Quick Tab", &Config.IsQuick);
            ImGui::Checkbox("Open Entity List", &Config.bisOpenManager);
            ImGui::InputInt("EXP:", &Config.EXP);
            //Creadit WoodgamerHD
            if (ImGui::Button("Give EXP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                GiveExperiencePoints(Config.EXP);
            ImGui::InputInt("Slot to modify (start 0):", &Config.AddItemSlot);
            ImGui::InputInt("Multiple of how much:", &Config.AddItemCount);

            if (ImGui::Button("Dupe Items In Slot", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                IncrementInventoryItemCountByIndex(Config.AddItemCount, Config.AddItemSlot);

            if (ImGui::Button("ToggleFly", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                Config.IsToggledFly = !Config.IsToggledFly;
                ExploitFly(Config.IsToggledFly);
            }

            /*if (ImGui::Button("DeleteSelf", ImVec2(ImGui::GetWindowContentRegionWidth() - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->Debug_RequestDeletePlayerSelf_ToServer();
                        }
                    }
                }
            }*/

            if (ImGui::Button("Revive", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                ReviveLocalPlayer();

            //Credit emoisback & Zanzer
            if (ImGui::Button("Easy Pal Condensation", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::TMap<int32, int32> RankRequired = SDK::TMap<int32, int32>();
                SDK::UWorld *world = Config.GetUWorld();
                SDK::UPalUtility *aPalUtility = SDK::UPalUtility::GetDefaultObj();
                aPalUtility->GetGameSetting(world)->CharacterRankUpRequiredNumDefault = 1;
                aPalUtility->GetGameSetting(world)->CharacterRankUpRequiredNumMap = RankRequired;
            }
        }

        void TABConfig()
        {

            ImGui::Text("NuLL");
            ImGui::Text("Version v2.0");
            ImGui::Text("Credits to: bluesword007");
            ImGui::Text("Credits to: UnknownCheats.me");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("UNHOOK DLL", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
#if DEBUG
                g_Console->printdbg("\n\n[+] UNHOOK INITIALIZED [+]\n\n", Console::Colors::red);

#endif
                g_KillSwitch = TRUE;
            }
        }

        //void TABDatabase()
        //{
        //    //ImGui::Checkbox("IsItems", &Config.matchDbItems);

        //    ImGui::InputText("Filter", Config.inputTextBuffer, sizeof(Config.inputTextBuffer), ImGuiInputTextFlags_CallbackCharFilter, InputTextCallback);

        //    Config.Update(Config.inputTextBuffer);

        //    const auto& filteredItems = Config.GetFilteredItems();

        //    for (const auto& itemName : filteredItems) {
        //        if (ImGui::Button(itemName.c_str())) 
        //        {
        //                strcpy_s(Config.ItemName, itemName.c_str());
        //                continue;
        //        //if (Config.matchDbItems) {}
        //        //strcpy_s(Config.PalName, itemName.c_str());
        //        }
        //    }
        //}

        void TABMisc()
        {
            if (ImGui::Button("KillAura", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {

                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState())
                        {
                            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                            for (int i = 0; i < T.Count(); i++)
                            {

                                if (T[i] != NULL)
                                {
                                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                                    {
                                        SDK::APalCharacter* monster = (SDK::APalCharacter*)T[i];
                                        if (monster->IsLocallyControlled() || monster->GetCharacterParameterComponent()->IsOtomo()) // credit emoisback
                                        {
                                            continue;
                                        }
                                        Config.GetPalPlayerState()->SendDeath_ToServer(monster);
                                        Damage(monster, 9999999999999);

                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (ImGui::Button("Crash Server", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))//
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->RequestLiftup_ToServer(NULL);
                    }
                }
            }

            if (ImGui::Button("Destroy All Visible Objects", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                DismantleObjects();

            if (ImGui::Button("Unlock All Effigies", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                UnlockAllEffigies();

            if (ImGui::Button("Unlock Chests", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) //Credit emoisback
            {
                UnlockChest();
            }

            if (ImGui::Button("Teleport Pals to XHair", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                Config.IsTeleportAllToXhair = !Config.IsTeleportAllToXhair;
            }

            if (Config.IsTeleportAllToXhair)
            {
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderFloat("##ENT_CAP_DISTANCE", &Config.mDebugEntCapDistance, 1.0f, 100.f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            }
        }

        void TABTeleporter()
        {
            ImGui::Checkbox("SafeTeleport", &Config.IsSafe);
            ImGui::SameLine();
            ImGui::Checkbox("Custom Waypoints", &Config.bisOpenWaypoints);
            if (ImGui::Button("Home", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                RespawnLocalPlayer(Config.IsSafe);
            ImGui::InputFloat3("Pos", Config.Pos);
            ImGui::SameLine();
            if (ImGui::Button("TP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::FVector vector = { Config.Pos[0],Config.Pos[1],Config.Pos[2] };
                AnyWhereTP(vector, Config.IsSafe);
            }

            if (ImGui::Button("LastWaypointTP")) // Credit: emoisback, bycEEE, w0dash
            {

                TpToLastWaypoint(Config.WaypointTpCleanup);
            }

            ImGui::SameLine();

            ImGui::Checkbox("DeleteWaypointAfterTP", &Config.WaypointTpCleanup);

            ImGui::SameLine();

            // TP to every new waypoint that gets set
            // TODO: button labeling (generally everywhere)
            ImGui::Checkbox("AutomaticWaypointTP", &Config.AutoWaypointTP);

            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 500), true);
            for (const auto& pair : database::locationMap)
            {
                const std::string& locationName = pair.first;
                if (ImGui::Button(locationName.c_str()))
                {
                    SDK::FVector location = SDK::FVector(pair.second[0], pair.second[1], pair.second[2]);
                    AnyWhereTP(location, Config.IsSafe);
                }
            }
            ImGui::EndChild();
        }

        void TABCageSpawner()
        {
            

            std::initializer_list list = itemlist::pals;

            int cur_size = 0;

            static char pal_search[100];

            ImGui::InputText("Search", pal_search, IM_ARRAYSIZE(pal_search));
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 500), true);
            for (const auto& pal : list) {
                std::istringstream ss(pal);
                std::string left_text, right_text;

                std::getline(ss, left_text, '|');
                std::getline(ss, right_text);

                auto right_to_lower = right_text;
                std::string pal_search_to_lower = pal_search;

                std::transform(right_to_lower.begin(), right_to_lower.end(), right_to_lower.begin(), ::tolower);
                std::transform(pal_search_to_lower.begin(), pal_search_to_lower.end(), pal_search_to_lower.begin(), ::tolower);

                if (pal_search[0] != '\0' && (right_to_lower.find(pal_search_to_lower) == std::string::npos))
                    continue;

                cur_size += right_text.length();

                ImGui::PushID(pal);
                if (ImGui::Button(right_text.c_str()))
                {
                    strcpy_s(Config.palSpawnCage, (char*)left_text.c_str());
                }
                ImGui::PopID();
            }
 

            if (ImGui::Button("Add Pal To Nearby Cage", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))//
            {
                SDK::APalPlayerCharacter* pLocalPlayer = Config.GetPalPlayerCharacter();
                if (!pLocalPlayer)
                    return;

                std::vector<SDK::AActor*> actors;

                if (!config::GetAllActorsofType(SDK::APalCapturedCage::StaticClass(), &actors, true))
                    return;

                auto draw = ImGui::GetBackgroundDrawList();

                for (auto actor : actors)
                {
                    SDK::APalCapturedCage* cage = static_cast<SDK::APalCapturedCage*>(actor); // Try to cast actor to APalCapturedCage
                    if (!cage)
                        continue; // Skip to the next actor if casting fails

                    SDK::FVector actorLocation = cage->K2_GetActorLocation();
                    SDK::FVector localPlayerLocation = pLocalPlayer->K2_GetActorLocation();
                    float distanceTo = GetDistanceToActor(pLocalPlayer, cage);

                    if (distanceTo < 50)
                    {
                        static SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

                        std::string palname = Config.palSpawnCage;
                        wchar_t  ws[255];
                        swprintf(ws, 255, L"%hs", palname.c_str());
                        SDK::FName Name = lib->Conv_StringToName(SDK::FString(ws));

                        cage->SpawnPal(Name, 50);
                    }
                }
            }
            
            ImGui::EndChild();
        }

        void TABItemSpawner()
        {
            static int num_to_add = 1;
            static int category = 0;

            ImGui::InputInt("Num To Add", &num_to_add);

            ImGui::Combo("Item Category", &category, "Accessories\0Ammo\0Armor\0Blueprints\0Crafting Materials\0Eggs\0Food\0Hats\0Medicine\0Money\0Other\0Pal Sphere\0Saddles\0Seeds\0Skill Fruits\0Tools\0Weapons\0");

            std::initializer_list list = itemlist::accessories;

            switch (category)
            {
            case 1:
                list = itemlist::ammo;
                break;
            case 2:
                list = itemlist::armor;
                break;
            case 3:
                list = itemlist::blueprints;
                break;
            case 4:
                list = itemlist::craftingmaterials;
                break;
            case 5:
                list = itemlist::eggs;
                break;
            case 6:
                list = itemlist::food;
                break;
            case 7:
                list = itemlist::hats;
                break;
            case 8:
                list = itemlist::medicine;
                break;
            case 9:
                list = itemlist::money;
                break;
            case 10:
                list = itemlist::other;
                break;
            case 11:
                list = itemlist::palspheres;
                break;
            case 12:
                list = itemlist::palskill;
                break;
            case 13:
                list = itemlist::seeds;
                break;
	    case 14:
    		list = itemlist::skillfruit;
		break;
            case 15:
                list = itemlist::tools;
                break;
            case 16:
                list = itemlist::weapons;
                break;
            default:
                list = itemlist::accessories;
            }

            int cur_size = 0;

            static char item_search[100];

            ImGui::InputText("Search", item_search, IM_ARRAYSIZE(item_search));
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 500), true);
            for (const auto& item : list) {
                std::istringstream ss(item);
                std::string left_text, right_text;

                std::getline(ss, left_text, '|');
                std::getline(ss, right_text);

                auto right_to_lower = right_text;
                std::string item_search_to_lower = item_search;

                std::transform(right_to_lower.begin(), right_to_lower.end(), right_to_lower.begin(), ::tolower);
                std::transform(item_search_to_lower.begin(), item_search_to_lower.end(), item_search_to_lower.begin(), ::tolower);

                if (item_search[0] != '\0' && (right_to_lower.find(item_search_to_lower) == std::string::npos))
                    continue;

                cur_size += right_text.length();

                ImGui::PushID(item);
                if (ImGui::Button(right_text.c_str()))
                {
                    SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();
                    AddItemToInventoryByName(InventoryData, (char*)left_text.c_str(), num_to_add);
                }
                ImGui::PopID();
            }
            ImGui::EndChild();
        }

        void TABQuick()
        {
            if (ImGui::Button("Basic Items stack", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::basic_items_stackable);

            if (ImGui::Button("Basic Items single", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::basic_items_single);

            if (ImGui::Button("Unlock Pal skills", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::pal_unlock_skills);

            if (ImGui::Button("Spheres", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::spheres);

            if (ImGui::Button("Tools", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::tools);

            if (ImGui::Button("Skill Fruits", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                SpawnMultiple_ItemsToInventory(config::QuickItemSet::skillfruit);
        }

        void TABDebug()
        {
            ImGui::Checkbox("Debug ESP", &Config.isDebugESP);
            if (Config.isDebugESP)
            {
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderFloat("##DISTANCE", &Config.mDebugESPDistance, 1.0f, 100.f, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            }

            //  @TODO: print additional debug information
            if (ImGui::Button("PrintPlayerAddr", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc)
                    g_Console->printdbg("\n\n[+] APalPlayerCharacter: 0x%llX\n", Console::Colors::green, p_appc);

            }

            ImGui::InputTextWithHint("##INPUT", "INPUT GOBJECT fn NAME", inputBuffer_getFnAddr, 100);
            ImGui::SameLine();
            if (ImGui::Button("GET fn", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
            {
                std::string input = inputBuffer_getFnAddr;
                SDK::UFunction* object = SDK::UObject::FindObject<SDK::UFunction>(input);
                if (object)
                {
                    static __int64 dwHandle = reinterpret_cast<__int64>(GetModuleHandle(0));
                    void* fnAddr = object->ExecFunction;
                    unsigned __int64 fnOffset = (reinterpret_cast<__int64>(fnAddr) - dwHandle);
                    g_Console->printdbg("[+] Found [%s] -> 0x%llX\n", Console::Colors::yellow, input.c_str(), fnOffset);
                }
                else
                    g_Console->printdbg("[!] OBJECT [%s] NOT FOUND!\n", Console::Colors::red, input.c_str());
                memset(inputBuffer_getFnAddr, 0, 100);
            }

        }
    }
    
	void Menu::Draw()
	{

		if (g_GameVariables->m_ShowMenu)
			MainMenu();

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();
	}

    void Menu::Waypoints()
    {
        float windowWidth = 300.0f;
        float windowHeight = 150.0f;

        if (!ImGui::Begin("Waypoints", &g_GameVariables->m_ShowMenu, ImGuiWindowFlags_NoResize))
        {
            ImGui::End();
            return;
        }

        ImGui::InputTextWithHint("##INPUT_SETWAYPOINT", "Set Name", inputBuffer_setWaypoint, 32);
        ImGui::SameLine();
        if (ImGui::Button("Add", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
        {
            std::string wpName = inputBuffer_setWaypoint;
            if (wpName.size() > 0)
            {
                AddWaypointLocation(wpName);
                memset(inputBuffer_setWaypoint, 0, 32);
            }
        }
        ImVec2 contentSize;
        if (Config.db_waypoints.size() > 0)
        {
            if (ImGui::BeginChild("##CHILD_WAYPOINTS", { 0.0f, 100.f }))
            {
                DWORD index = -1;
                for (auto waypoint : Config.db_waypoints)
                {
                    index++;
                    ImGui::PushID(index);
                    //  ImGui::Checkbox("SHOW", &waypoint.bIsShown);
                    //  ImGui::SameLine();
                    if (ImGui::Button(waypoint.waypointName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20)))
                        AnyWhereTP(waypoint.waypointLocation, false);
                    ImGui::PopID();
                }

                contentSize = ImGui::GetWindowSize();

                ImGui::EndChild();
            }
        }

        windowWidth = (((windowWidth) > (contentSize.x)) ? (windowWidth) : (contentSize.x));
        windowHeight = (((windowHeight) > (contentSize.y)) ? (windowHeight) : (contentSize.y));
        ImGui::SetWindowSize(ImVec2(windowWidth, windowHeight));

        ImGui::End();

    }

    void Menu::EntityList()
    {
        if (!ImGui::Begin("Entity List", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }

        
        if (Config.gWorld)
        {
            ImGui::Checkbox("Player", &Config.filterPlayer);
            ImGui::SameLine();
            ImGui::Checkbox("Pal", &Config.filterPal);
            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
            for (int i = 0; i < T.Count(); i++)
            {
                if (!T[i])
                    continue;

                if (!T[i]->IsA(SDK::APalCharacter::StaticClass()))
                    continue;

                SDK::APalCharacter* Character = (SDK::APalCharacter*)T[i];
                SDK::FString name;
                if (Config.isfilterSelf)
                    if (Character)
                    {
                        if (Character->IsLocallyControlled())
                        {
                            continue;
                        }
                        auto ParameterComponent = Character->GetCharacterParameterComponent();
                        if (ParameterComponent)
                        {
                            if (ParameterComponent->IsOtomo())
                            {
                                continue;
                            }
                        }
                    }
                if (Config.filterPlayer)
                {
                    if (!T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()))
                        continue;
                }
                if (Config.filterPal)
                {
                    if (!Character->StaticCharacterParameterComponent->IsPal)
                    {
                        continue;
                    }
                }
                if (T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()) || Character->StaticCharacterParameterComponent->IsPal)
                {
                    if (!Character)
                        continue;

                    Character->CharacterParameterComponent->GetNickname(&name);
                }
                else
                {
                    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                    if (!Character)
                        continue;

                    std::string s = Character->GetFullName();
                    size_t firstUnderscorePos = s.find('_');

                    if (firstUnderscorePos != std::string::npos) 
                    {
                        std::string result = s.substr(firstUnderscorePos + 1);

                        size_t secondUnderscorePos = result.find('_');

                        if (secondUnderscorePos != std::string::npos) {
                            result = result.substr(0, secondUnderscorePos);
                        }
                        wchar_t  ws[255];
                        swprintf(ws, 255, L"%hs", result);
                        name = SDK::FString(ws);
                    }
                }
                ImGui::Text(
                    "%s%s%s",
                    Character->StaticCharacterParameterComponent->IsRarePal() ? "** " : "",
                    name.ToString().c_str(),
                    Character->StaticCharacterParameterComponent->IsRarePal() ? " **" : ""
                );
                ImGui::SameLine();
                ImGui::PushID(i);
                if (ImGui::Button("Kill"))
                {
                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                        Damage(Character, 99999999999);
                }
                ImGui::SameLine();
                if (ImGui::Button("Set 1 HP"))
                {
                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                        Damage(Character, (Character->CharacterParameterComponent->GetHP().Value - 1) / 1000);
                }
                ImGui::SameLine();
                if (ImGui::Button("Heal"))
                {
                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                        Heal(Character);
                }
                ImGui::SameLine();
                if (ImGui::Button("TP"))
                {
                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                    {
                        if (Character)
                        {
                            SDK::FVector vector = Character->K2_GetActorLocation();
                            AnyWhereTP(vector, Config.IsSafe);
                        }
                    }
                }
                /*if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Boss"))
                    {
                        if (Config.GetPalPlayerCharacter() != NULL)
                        {
                            auto controller = Config.GetPalPlayerCharacter()->GetPalPlayerController();
                            if (controller != NULL)
                            {
                                controller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::ElectricBoss, (SDK::APalPlayerCharacter*)Character);
                                controller->Transmitter->BossBattle->RequestBossBattleStart_ToServer(SDK::EPalBossType::ElectricBoss, (SDK::APalPlayerCharacter*)Character);
                            }
                        }
                    }
                }*/
                if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                {
                    ImGui::SameLine();
                    if (ImGui::Button("Join Guild"))
                    {
                        if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                            ForceJoinGuild(Character);
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("MaskIt"))
                    {
                        if (Config.GetPalPlayerCharacter() != NULL)
                        {
                            auto controller = Config.GetPalPlayerCharacter()->GetPalPlayerController();
                            if (controller != NULL)
                            {
                                auto player = (SDK::APalPlayerCharacter*)Character;
                                SDK::FString fakename;
                                player->CharacterParameterComponent->GetNickname(&fakename);
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, fakename);
                            }
                        }
                    }
                }
                ImGui::PopID();
            }

        }

        if (Config.GetUWorld() != NULL)
        {
        }
        
        ImGui::End();
    }
	
    void Menu::MainMenu()
	{
        if (!g_GameVariables->m_ShowDemo)
            Styles::InitStyle();

        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
        }
        if (!ImGui::Begin("NuLLxD", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }
        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        
        ImGuiContext* pImGui = GImGui;

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Player"))
            {
                Tabs::TABPlayer();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Exploit"))
            {
                Tabs::TABExploit();
                ImGui::EndTabItem();
            }
            //if (ImGui::BeginTabItem("Database"))
            //{
            //    Tabs::TABDatabase();
            //    ImGui::EndTabItem();
            //}
            if (ImGui::BeginTabItem("Item Spawner"))
            {
                Tabs::TABItemSpawner();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Cage Spawner"))
            {
                Tabs::TABCageSpawner();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Teleporter"))
            {
                Tabs::TABTeleporter();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Misc"))
            {
                Tabs::TABMisc();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Info"))
            {
                Tabs::TABConfig();
                ImGui::EndTabItem();
            }
#if DEBUG
            if (ImGui::BeginTabItem("Debug"))
            {
                Tabs::TABDebug();
                ImGui::EndTabItem();
            }
#endif
            if (Config.IsQuick && ImGui::BeginTabItem("Quick"))
            {
                Tabs::TABQuick();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();

        if (Config.bisOpenWaypoints)
            Waypoints();

        if (Config.bisOpenManager)
            EntityList();
	}

	void Menu::HUD(bool* p_open)
	{
        
        ImGui::SetNextWindowPos(g_D3D11Window->pViewport->WorkPos);
        ImGui::SetNextWindowSize(g_D3D11Window->pViewport->WorkSize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, NULL);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
        if (!ImGui::Begin("##HUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
        {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
            return;
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        auto ImDraw = ImGui::GetWindowDrawList();
        auto draw_size = g_D3D11Window->pViewport->WorkSize;
        auto center = ImVec2({ draw_size.x * .5f, draw_size.y * .5f });
        auto top_center = ImVec2({ draw_size.x * .5f, draw_size.y * 0.0f });
        
        // // Watermark
        //ImDraw->AddText(top_center, g_Menu->dbg_RAINBOW, "PalWorld-NetCrack");

        if (Config.IsESP)
            ESP();

        if (Config.isDebugESP)
            ESP_DEBUG(Config.mDebugESPDistance);

        ImGui::End();
	}

    void Menu::Loops()
    {
        //  Respawn
        if ((GetAsyncKeyState(VK_F5) & 1))
            RespawnLocalPlayer(Config.IsSafe);

        //  Revive Player
        if ((GetAsyncKeyState(VK_F6) & 1))
            ReviveLocalPlayer();
        
        //
        if (Config.IsTeleportAllToXhair)
            TeleportAllPalsToCrosshair(Config.mDebugEntCapDistance);

        //  
        if (Config.IsSpeedHack)
            SpeedHack(Config.SpeedModiflers);
        else
        {
            SpeedHack(1.0f); // Turn off the feature
        }
        
        //  
        if (Config.IsAttackModiler)
            SetPlayerAttackParam(Config.DamageUp);
        else
        {
            SetPlayerAttackParam(1.0f); // Turn off the feature
        }

        //  
        if (Config.IsDefuseModiler)
            SetPlayerDefenseParam(Config.DefuseUp);
        else
        {
            SetPlayerDefenseParam(1.0f); // Turn off the feature
        }

        //  
        if (Config.IsInfStamina)
            ResetStamina();

        //
        if (Config.IsGodMode)
            SetPlayerHealth(INT_MAX);

        //
        if (Config.IsInfinAmmo)
        {
            SetInfiniteAmmo(true);
        }
        else
        {
            SetInfiniteAmmo(false);
        }
        if (Config.AutoWaypointTP && GetCurrentWaypointCount() > Config.AutoWaypointTpLastCount)
        {
            TpToLastWaypoint(true);
        }

        Config.AutoWaypointTpLastCount = GetCurrentWaypointCount();

        //  
        //  SetDemiGodMode(Config.IsMuteki);
    }
}
