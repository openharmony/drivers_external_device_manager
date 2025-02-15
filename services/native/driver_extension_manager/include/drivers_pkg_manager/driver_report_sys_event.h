/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DRIVER_PKG_MANAGER_H
#define DRIVER_PKG_MANAGER_H

#include <string>
#include <vector>
#include "pkg_tables.h"

namespace OHOS {
namespace ExternalDeviceManager {

class ExtDevReportSysEvent
{
public:
    static void ReportDriverPackageCycleMangeSysEvent(PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);
};

}
}