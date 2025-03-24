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

#ifndef DRIVER_REPORT_SYS_EVENT_H
#define DRIVER_REPORT_SYS_EVENT_H

#include <string>
#include <vector>
#include "pkg_tables.h"

namespace OHOS {
namespace ExternalDeviceManager {

static std::map<uint32_t id, sptr<ExtDevEvent> event> matchMap_;
static std::map<uint32_t id, sptr<ExtDevEvent> event> deviceMap_;
static std::map<std::string id, sptr<ExtDevEvent> event> driverMap_;
constexpr int MAP_SIZE_MAX = 1024;

enum EXTDEV_EXP_EVENT {  // 操作类型枚举
    DRIVER_BIND = 1,             // 绑定设备驱动
    DRIVER_UNBIND,               // 解绑设备驱动
    DRIVER_PACKAGE_DATA_REFRESH, // 驱动包数据刷新
    DRIVER_PACKAGE_CYCLE_MANAGE, // 驱动包管理生命周期
    GET_DEVICE_INFO,             // 设备信息获取
    CHANGE_FUNC,                 // 切换过程中生命周期
    DRIVER_DEVICE_MATCH,         // 设备与驱动匹配
};

struct ExtDevEvent {
    int32_t deviceClass;       // 设备类型
    int32_t deviceSubClass;    // 设备子类型
    int_32t deviceProtocol     // 设备协议
    std::string snNum;         // 设备SN号
    int32_t vendorId;          // 厂商Id
    int32_t productId;         // 产品Id
    int32_t deviceId;          // 设备Id
    std::string driverUid;     // 驱动Uid
    std::string driverName;    // 驱动名称
    int32_t versionCode;       // 驱动版本
    std::string vids;          // 驱动配置的vid
    std::string pids;          // 驱动配置的pid
    int32_t userId;            // 用户Id
    std::string bundleName;    // 驱动包名
    int32_t operatType;        // 操作类型
    std::string interfaceName; // 接口名称
    int32_t errCode;           // 故障码
};

class ExtDevReportSysEvent {
public:
    static void ReportDriverPackageCycleManageSysEvent(const PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);
    
    static void ReportExternalDeviceEvent(const std::shared_ptr<ExtDevEvent>& extDevEvent);
    
    static void ReportExternalDeviceSaEvent(const PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);

    static void ReportExternalDeviceDdkEvent(const PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);

    shared_ptr<ExtDevEvent> ExtDevEventInit(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<DriverInfo> &driverInfo);

    static bool IsMatched(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<DriverInfo> &driverInfo);
    
    shared_ptr<ExtDevEvent> DeviceEventReport(const uint32_t deviceId);

    shared_ptr<ExtDevEvent> DriverEventReport(const std::string driverUid);

    shared_ptr<ExtDevEvent> MatchEventReport(const uint32_t deviceId);

    void SetEventValue(const std::string interfaceName, const int32_t operatType, 
        const int32_t errCode, std::shared_ptr<ExtDevEvent> eventPtr);
private:
    std::mutex hisyseventMutex_;
};

} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // DRIVER_REPORT_SYS_EVENT_H