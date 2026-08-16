/*
 * Copyright 2025-2026 howdy213, JYardX
 *
 * This file is part of HugoWidgets.
 *
 * HugoWidgets is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HugoWidgets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HugoWidgets. If not, see <https://www.gnu.org/licenses/>.
 */
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