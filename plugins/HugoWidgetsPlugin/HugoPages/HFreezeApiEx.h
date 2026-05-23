/*
 * Copyright 2025-2026 howdy213, JYardX
 *
 * This file is part of HugoProgs.
 *
 * HugoProgs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HugoProgs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HugoProgs. If not, see <https://www.gnu.org/licenses/>.
 */
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