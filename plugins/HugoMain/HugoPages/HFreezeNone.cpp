#include "HugoPages/HFreezeNone.h"
#include <QStorageInfo>
#include <Windows.h>

#ifndef HU_DISABLE_FREEZE

HFreezeNone::HFreezeNone() : m_initialized(false), m_lastError(ERROR_SUCCESS) {}

HFreezeNone::~HFreezeNone() {}

FreezeResult HFreezeNone::Init() noexcept {
    m_initialized = true;
    m_lastError = ERROR_SUCCESS;
    m_lastErrorMsg.clear();
    return FreezeResult(FrzOR::Success, L"None mode initialized");
}

void HFreezeNone::Cleanup() noexcept { m_initialized = false; }

bool HFreezeNone::IsInitialized() const noexcept { return m_initialized; }

FreezeResult HFreezeNone::GetFreezeState() const noexcept {
    if (!m_initialized) {
        return FreezeResult(FrzOR::NotInitialized, L"Not initialized");
    }

    FreezeResult result(FrzOR::Success, L"Get disk info success");
    result.diskInfos.clear();

    auto diskList = QStorageInfo::mountedVolumes();
    for (const QStorageInfo &si : std::as_const(diskList)) {
        if (GetDriveTypeA(si.rootPath().toStdString().c_str()) != DRIVE_FIXED)
            continue;

        QString rootPath = si.rootPath();
        if (rootPath.isEmpty())
            continue;

        wchar_t driveLetter = rootPath.at(0).toUpper().toLatin1();

        DiskInfo info;
        info.bytesTotal = (si.bytesTotal() > 0)
                              ? static_cast<size_t>(si.bytesTotal()) / 1024 / 1024
                              : 0;
        info.bytesFree = (si.bytesFree() > 0)
                             ? static_cast<size_t>(si.bytesFree()) / 1024 / 1024
                             : 0;
        info.state = DriveFreezeState::Unknown;

        result.diskInfos[driveLetter] = info;
    }

    return result;
}

FreezeResult
HFreezeNone::TryProtect(const std::wstring & /*driveLetters*/) const noexcept {
    if (!m_initialized) {
        return FreezeResult(FrzOR::NotInitialized, L"Not initialized");
    }
    return FreezeResult(FrzOR::Success, L"No operation in None mode");
}

FreezeResult
HFreezeNone::SetFreezeState(const std::wstring & /*driveLetters*/) noexcept {
    if (!m_initialized) {
        return FreezeResult(FrzOR::NotInitialized, L"Not initialized");
    }
    return FreezeResult(FrzOR::Success, L"No operation in None mode");
}

std::wstring HFreezeNone::GetLastErrorMsg() const noexcept {
    return m_lastErrorMsg;
}

DWORD HFreezeNone::GetLastErrorCode() const noexcept { return m_lastError; }

#endif // HU_DISABLE_FREEZE