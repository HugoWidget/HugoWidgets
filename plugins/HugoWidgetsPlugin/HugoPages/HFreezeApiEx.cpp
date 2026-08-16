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
#include "hfreezeapiex.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

HFreezeApiEx::~HFreezeApiEx() = default;

FreezeResult HFreezeApiEx::Init() noexcept {
    return HFreezeApi::Init();
}

void HFreezeApiEx::Cleanup() noexcept {
    return HFreezeApi::Cleanup();
}

bool HFreezeApiEx::IsInitialized() const noexcept {
    return HFreezeApi::IsInitialized();
}

bool HFreezeApiEx::parseJsonResponse(const std::wstring& rawMsg, QJsonObject& rootObj) const {
    const size_t pos = rawMsg.find(L'{');
    if (pos == std::wstring::npos) {
        return false;
    }
    const QString jsonStr = QString::fromStdWString(rawMsg.substr(pos));
    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (jsonDoc.isNull() || parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return false;
    }
    rootObj = jsonDoc.object();
    return true;
}

FreezeResult HFreezeApiEx::GetFreezeState() const noexcept {
    const FreezeResult origin = HFreezeApi::GetFreezeState();
    FreezeResult result(FrzOR::Failed);

    QJsonObject rootObj;
    if (!parseJsonResponse(origin.msg, rootObj)) {
        return result;
    }

    result.error = rootObj["code"].toBool();
    result.msg   = rootObj["message"].toString().toStdWString();

    const QJsonArray dataArray = rootObj["data"].toArray();
    if (dataArray.isEmpty()) {
        return result;
    }

    const QJsonObject dataObj = dataArray[0].toObject();
    const QJsonArray disksArray = dataObj["disksData"].toArray();
    for (const QJsonValue& val : disksArray) {
        const QJsonObject obj = val.toObject();
        DiskInfo info;
        info.bytesTotal = obj["totalSizes"].toVariant().toLongLong();
        info.bytesFree  = obj["freeSizes"].toVariant().toLongLong();
        info.state      = static_cast<DriveFreezeState>(obj["protectedStatus"].toInt());

        const std::wstring diskName = obj["diskName"].toString().toStdWString();
        if (!diskName.empty()) {
            const wchar_t driveLetter = std::toupper(diskName[0]);
            result.diskInfos[driveLetter] = info;
        }
    }

    return result;
}

FreezeResult HFreezeApiEx::TryProtect(const std::wstring& driveLetters) const noexcept {
    return HFreezeApi::TryProtect(driveLetters);
}

FreezeResult HFreezeApiEx::SetFreezeState(const std::wstring& driveLetters) noexcept {
    const FreezeResult origin = HFreezeApi::SetFreezeState(driveLetters);
    FreezeResult result(FrzOR::Failed);

    QJsonObject rootObj;
    if (!parseJsonResponse(origin.msg, rootObj)) {
        return result;
    }

    result.error = rootObj["code"].toBool();
    result.msg   = rootObj["errMsg"].toString().toStdWString();

    const QString message = rootObj["message"].toString();
    if (!message.isEmpty()) {
        result.msg += L":" + message.toStdWString();
    }

    return result;
}

std::wstring HFreezeApiEx::GetLastErrorMsg() const noexcept {
    return HFreezeApi::GetLastErrorMsg();
}

DWORD HFreezeApiEx::GetLastErrorCode() const noexcept {
    return HFreezeApi::GetLastErrorCode();
}