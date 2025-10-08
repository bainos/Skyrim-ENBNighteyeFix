#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

#include "ENBSeriesAPI.h"

constexpr RE::FormID NIGHT_EYE_SPELL = 0x000AA01D;

// Global ENB API pointer
ENB_API::ENBSDK1001* g_ENB = nullptr;

// Pending Night Eye state to set in callback
bool g_PendingNightEyeState = false;
bool g_HasPendingUpdate = false;

/**
 * Night Eye state persistence
 */
struct NightEyeState {
    static constexpr std::uint32_t kDataKey = 'BNEF';
    static constexpr std::uint32_t kSerializationVersion = 1;

    bool isActive = false;

    static NightEyeState* GetSingleton() {
        static NightEyeState instance;
        return &instance;
    }

    void Save(SKSE::SerializationInterface* intfc) {
        if (!intfc->WriteRecordData(&isActive, sizeof(isActive))) {
            spdlog::error("Failed to write Night Eye state");
        }
    }

    void Load(SKSE::SerializationInterface* intfc) {
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;

        while (intfc->GetNextRecordInfo(type, version, length)) {
            if (type == kDataKey && version == kSerializationVersion) {
                if (intfc->ReadRecordData(&isActive, sizeof(isActive))) {
                    spdlog::info("Loaded Night Eye state: {}", isActive);
                    // Reapply state to ENB
                    g_PendingNightEyeState = isActive;
                    g_HasPendingUpdate = true;
                }
            }
        }
    }

    void Revert(SKSE::SerializationInterface*) {
        isActive = false;
    }
};

// ENB Callback function
void WINAPI ENBCallback(ENB_SDK::ENBCallbackType calltype) {
    if (calltype == ENB_SDK::ENBCallbackType::ENBCallback_PostLoad ||
        calltype == ENB_SDK::ENBCallbackType::ENBCallback_EndFrame) {

        if (g_HasPendingUpdate && g_ENB) {
            ENB_SDK::ENBParameter param;
            param.Type = ENB_SDK::ENBParameterType::ENBParam_BOOL;
            param.Size = 4;
            *reinterpret_cast<BOOL*>(param.Data) = g_PendingNightEyeState ? TRUE : FALSE;

            if (g_ENB->SetParameter(nullptr, "ENBEFFECT.FX", "KNActive", &param)) {
                spdlog::info("ENB: KNActive = {}", g_PendingNightEyeState);
                g_HasPendingUpdate = false;
            }
        }
    }
}

void SetupLog() {
    auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("BainosNighteyeFix.log", true);
    auto logger = std::make_shared<spdlog::logger>("log", std::move(sink));
    spdlog::set_default_logger(std::move(logger));
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("BainosNighteyeFix v13 - State Persistence");
}

class EquipHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
public:
    static EquipHandler* Get() {
        static EquipHandler instance;
        return &instance;
    }

    RE::BSEventNotifyControl ProcessEvent(
        const RE::TESEquipEvent* event,
        RE::BSTEventSource<RE::TESEquipEvent>*) override {

        if (!event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || event->actor.get() != player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->baseObject == NIGHT_EYE_SPELL) {
            if (event->equipped) {
                spdlog::info("Night Eye EQUIPPED");
                RE::DebugNotification("Night Eye: ON");

                // Update state
                NightEyeState::GetSingleton()->isActive = true;
                g_PendingNightEyeState = true;
                g_HasPendingUpdate = true;
            }
            else {
                spdlog::info("Night Eye UNEQUIPPED");
                RE::DebugNotification("Night Eye: OFF");

                // Update state
                NightEyeState::GetSingleton()->isActive = false;
                g_PendingNightEyeState = false;
                g_HasPendingUpdate = true;
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
};

void MessageHandler(SKSE::MessagingInterface::Message* msg) {
    switch (msg->type) {
    case SKSE::MessagingInterface::kPostLoad:
        g_ENB = reinterpret_cast<ENB_API::ENBSDK1001*>(
            ENB_API::RequestENBAPI(ENB_API::SDKVersion::V1001));

        if (g_ENB) {
            spdlog::info("ENB API obtained - SDK v{}, ENB v{}",
                g_ENB->GetSDKVersion(), g_ENB->GetVersion());
            g_ENB->SetCallbackFunction(ENBCallback);
        }
        else {
            spdlog::warn("ENB API not available");
        }
        break;

    case SKSE::MessagingInterface::kDataLoaded:
        auto events = RE::ScriptEventSourceHolder::GetSingleton();
        if (events) {
            events->AddEventSink(EquipHandler::Get());
            spdlog::info("Event handler registered");
        }
        break;
    }
}

void InitializePlugin() {
    SetupLog();

    // Register serialization callbacks
    auto serialization = SKSE::GetSerializationInterface();
    if (serialization) {
        serialization->SetUniqueID(NightEyeState::kDataKey);
        serialization->SetSaveCallback([](SKSE::SerializationInterface* intfc) {
            if (!intfc->OpenRecord(NightEyeState::kDataKey, NightEyeState::kSerializationVersion)) {
                spdlog::error("Failed to open save record");
                return;
            }
            NightEyeState::GetSingleton()->Save(intfc);
            });
        serialization->SetLoadCallback([](SKSE::SerializationInterface* intfc) {
            NightEyeState::GetSingleton()->Load(intfc);
            });
        serialization->SetRevertCallback([](SKSE::SerializationInterface* intfc) {
            NightEyeState::GetSingleton()->Revert(intfc);
            });
        spdlog::info("Serialization registered");
    }

    if (auto messaging = SKSE::GetMessagingInterface()) {
        messaging->RegisterListener(MessageHandler);
        spdlog::info("Messaging registered");
    }

    spdlog::info("Plugin initialized");
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SKSE::RegisterForAPIInitEvent(InitializePlugin);
    return true;
}