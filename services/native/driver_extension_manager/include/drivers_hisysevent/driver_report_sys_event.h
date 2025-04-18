/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include <mutex>
#include <memory>
#include <map>

namespace OHOS {
namespace ExternalDeviceManager {
class DeviceInfo;
class DriverInfo;
struct PkgInfoTable;

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

typedef struct ExtDevEvent {
    int32_t deviceClass;       // 设备类型
    int32_t deviceSubClass;    // 设备子类型
    int32_t deviceProtocol;    // 设备协议
    std::string snNum;         // 设备SN号
    int32_t vendorId;          // 厂商Id
    int32_t productId;         // 产品Id
    uint64_t deviceId;          // 设备Id
    std::string driverUid;     // 驱动Uid
    std::string driverName;    // 驱动名称
    std::string versionCode;   // 驱动版本
    std::string vids;          // 驱动配置的vid
    std::string pids;          // 驱动配置的pid
    int32_t userId;            // 用户Id
    std::string bundleName;    // 驱动包名
    int32_t operatType;        // 操作类型
    std::string interfaceName; // 接口名称
    std::string message;       // 信息
    int32_t errCode;           // 故障码
} ExtDevEvent;

class ExtDevReportSysEvent {
public:
    static void ReportDriverPackageCycleManageSysEvent(const PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);

    static void ReportDelPkgsCycleManageSysEvent(const std::string &bundleName, const std::string &driverEventName);

    static void ReportExternalDeviceEvent(const std::shared_ptr<ExtDevEvent> &extDevEvent);
    
    static void ReportExternalDeviceSaEvent(const PkgInfoTable &pkgInfoTable, std::string pids,
        std::string vids, uint32_t versionCode, std::string driverEventName);

    static std::shared_ptr<ExtDevEvent> ExtDevEventInit(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<DriverInfo> &driverInfo, std::shared_ptr<ExtDevEvent> eventObj);

    static bool IsMatched(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<DriverInfo> &driverInfo, const std::string &type, const std::string &interfaceName);
    
    static std::shared_ptr<ExtDevEvent> DeviceEventReport(const uint64_t deviceId, const std::string &message = "");

    static std::shared_ptr<ExtDevEvent> DriverEventReport(const std::string driverUid);

    static std::shared_ptr<ExtDevEvent> MatchEventReport(const uint64_t deviceId);

    static void SetEventValue(const std::string interfaceName, const int32_t operatType,
        const int32_t errCode, std::shared_ptr<ExtDevEvent> eventPtr);

    static void DriverMapInsert(const std::string driverUid, std::shared_ptr<ExtDevEvent> eventPtr);

    static void DeviceMapInsert(const uint64_t deviceId, std::shared_ptr<ExtDevEvent> eventPtr);

    static void DriverMapErase(const std::string driverUid);

    static void DriverMapDelete(const std::string &bundleName);

    static void DeviceMapErase(const uint64_t deviceId);

    static void MatchMapErase(const uint64_t deviceId);

    static std::string ParseIdVector(std::vector<uint16_t> ids);

private:
    static std::map<uint64_t, std::shared_ptr<ExtDevEvent>> matchMap_;
    static std::map<uint64_t, std::shared_ptr<ExtDevEvent>> deviceMap_;
    static std::map<std::string, std::shared_ptr<ExtDevEvent>> driverMap_;
    static std::mutex hisyseventMutex_;
};

} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // DRIVER_REPORT_SYS_EVENT_H