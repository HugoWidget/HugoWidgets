#pragma once
#include "HugoUtils/HFreezeInterface.h"
#ifndef HU_DISABLE_FREEZE

class HFreezeNone : public IHugoFreeze {
public:
    HFreezeNone();
    virtual ~HFreezeNone();

    virtual FreezeResult Init() noexcept override;
    virtual void Cleanup() noexcept override;
    virtual bool IsInitialized() const noexcept override;

    virtual FreezeResult GetFreezeState() const noexcept override;
    virtual FreezeResult TryProtect(const std::wstring& driveLetters) const noexcept override;
    virtual FreezeResult SetFreezeState(const std::wstring& driveLetters) noexcept override;

    virtual std::wstring GetLastErrorMsg() const noexcept override;
    virtual DWORD GetLastErrorCode() const noexcept override;

private:
    bool m_initialized;
    mutable DWORD m_lastError;
    mutable std::wstring m_lastErrorMsg;
};

#endif // HU_DISABLE_FREEZE