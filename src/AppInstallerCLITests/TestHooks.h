// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once
#include <SourceFactory.h>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include <AppInstallerTelemetry.h>
#include <AppInstallerRuntime.h>
#include <winget/UserSettings.h>
#include <winget/Filesystem.h>
#include <winget/IconExtraction.h>

#ifdef AICLI_DISABLE_TEST_HOOKS
static_assert(false, "Test hooks have been disabled");
#endif

namespace AppInstaller
{
    // Don't forget to clear the overrides after use!
    // A good way is to create a helper struct that cleans when destroyed

    namespace Runtime
    {
        void TestHook_SetPathOverride(PathName target, const std::filesystem::path& path);
        void TestHook_SetPathOverride(PathName target, const PathDetails& details);
        void TestHook_ClearPathOverrides();
    }

    namespace Repository
    {
        void TestHook_SetSourceFactoryOverride(const std::string& type, std::function<std::unique_ptr<ISourceFactory>()>&& factory);
        void TestHook_ClearSourceFactoryOverrides();
        void TestHook_SetExtractIconFromArpEntryResult_Override(std::vector<AppInstaller::Repository::ExtractedIconInfo>* result);
    }

    namespace Repository::Microsoft
    {
        void TestHook_SetPinningIndex_Override(std::optional<std::filesystem::path>&& indexPath);
    }

    namespace Logging
    {
        void TestHook_SetTelemetryOverride(std::shared_ptr<TelemetryTraceLogger> ttl);
    }

    namespace Settings
    {
        void SetUserSettingsOverride(UserSettings* value);
    }

    namespace Filesystem
    {
        void TestHook_SetCreateSymlinkResult_Override(bool* status);
    }

    namespace Archive
    {
        void TestHook_SetScanArchiveResult_Override(bool* status);
    }

    namespace CLI::Workflow
    {
        void TestHook_SetEnableWindowsFeatureResult_Override(std::optional<DWORD>&& result);
        void TestHook_SetDoesWindowsFeatureExistResult_Override(std::optional<DWORD>&& result);
    }

    namespace Reboot
    {
        void TestHook_SetInitiateRebootResult_Override(bool* status);
    }
}

namespace TestHook
{
    struct SetCreateSymlinkResult_Override
    {
        SetCreateSymlinkResult_Override(bool status) : m_status(status)
        {
            AppInstaller::Filesystem::TestHook_SetCreateSymlinkResult_Override(&m_status);
        }

        ~SetCreateSymlinkResult_Override()
        {
            AppInstaller::Filesystem::TestHook_SetCreateSymlinkResult_Override(nullptr);
        }

    private:
        bool m_status;
    };

    struct SetScanArchiveResult_Override
    {
        SetScanArchiveResult_Override(bool status) : m_status(status)
        {
            AppInstaller::Archive::TestHook_SetScanArchiveResult_Override(&m_status);
        }

        ~SetScanArchiveResult_Override()
        {
            AppInstaller::Archive::TestHook_SetScanArchiveResult_Override(nullptr);
        }

    private:
        bool m_status;
    };

    struct SetPinningIndex_Override
    {
        SetPinningIndex_Override(const std::filesystem::path& indexPath)
        {
            AppInstaller::Repository::Microsoft::TestHook_SetPinningIndex_Override(indexPath);
        }

        ~SetPinningIndex_Override()
        {
            AppInstaller::Repository::Microsoft::TestHook_SetPinningIndex_Override({});
        }
    };

    struct SetExtractIconFromArpEntryResult_Override
    {
        SetExtractIconFromArpEntryResult_Override(std::vector<AppInstaller::Repository::ExtractedIconInfo> extractedIcons) : m_extractedIcons(std::move(extractedIcons))
        {
            AppInstaller::Repository::TestHook_SetExtractIconFromArpEntryResult_Override(&m_extractedIcons);
        }

        ~SetExtractIconFromArpEntryResult_Override()
        {
            AppInstaller::Repository::TestHook_SetExtractIconFromArpEntryResult_Override(nullptr);
        }

    private:
        std::vector<AppInstaller::Repository::ExtractedIconInfo> m_extractedIcons;
    };

    struct SetEnableWindowsFeatureResult_Override
    {
        SetEnableWindowsFeatureResult_Override(DWORD result)
        {
            AppInstaller::CLI::Workflow::TestHook_SetEnableWindowsFeatureResult_Override(result);
        }

        ~SetEnableWindowsFeatureResult_Override()
        {
            AppInstaller::CLI::Workflow::TestHook_SetEnableWindowsFeatureResult_Override({});
        }
    };

    struct SetDoesWindowsFeatureExistResult_Override
    {
        SetDoesWindowsFeatureExistResult_Override(DWORD result)
        {
            AppInstaller::CLI::Workflow::TestHook_SetDoesWindowsFeatureExistResult_Override(result);
        }

        ~SetDoesWindowsFeatureExistResult_Override()
        {
            AppInstaller::CLI::Workflow::TestHook_SetDoesWindowsFeatureExistResult_Override({});
        }
    };

    struct SetInitiateRebootResult_Override
    {
        SetInitiateRebootResult_Override(bool status) : m_status(status)
        {
            AppInstaller::Reboot::TestHook_SetInitiateRebootResult_Override(&m_status);
        }

        ~SetInitiateRebootResult_Override()
        {
            AppInstaller::Reboot::TestHook_SetInitiateRebootResult_Override(nullptr);
        }

    private:
        bool m_status;
    };
}