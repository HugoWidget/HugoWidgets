#ifndef HFREEZEAPIEX_H
#define HFREEZEAPIEX_H

#include "HugoUtils/HFreezeApi.h"
#include <QJsonObject>

class HFreezeApiEx final : public HFreezeApi
{
public:
    HFreezeApiEx() = default;
    ~HFreezeApiEx() override;

    FreezeResult Init() noexcept override;
    void Cleanup() noexcept override;
    bool IsInitialized() const noexcept override;

    FreezeResult GetFreezeState() const noexcept override;
    FreezeResult TryProtect(const std::wstring& driveLetters) const noexcept override;
    FreezeResult SetFreezeState(const std::wstring& driveLetters) noexcept override;

    std::wstring GetLastErrorMsg() const noexcept override;
    DWORD GetLastErrorCode() const noexcept override;

private:
    bool parseJsonResponse(const std::wstring& rawMsg, QJsonObject& rootObj) const;
};

#endif // HFREEZEAPIEX_H