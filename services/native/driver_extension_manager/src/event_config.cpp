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

#include "event_config.h"
#include "hilog_wrapper.h"

namespace {
constexpr const char *SYSTEM_PERIPHERAL_FAULT_NOTIFIER_CONFIG_PATH =
    "/system/etc/peripheral/peripheral_fault_notifier_config.json";
constexpr const char *SYS_PROD_PERIPHERAL_FAULT_NOTIFIER_CONFIG_PATH =
    "/sys_prod/etc/peripheral/peripheral_fault_notifier_config.json";
} // namespace
namespace OHOS {
namespace ExternalDeviceManager {
std::shared_ptr<EventConfig> EventConfig::instance_ = nullptr;
std::mutex EventConfig::mutex_;
EventConfig &EventConfig::GetInstance()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_ == nullptr) {
        instance_ = std::make_shared<EventConfig>();
    }
    return *(instance_.get());
}

std::vector<FaultInfo> EventConfig::GetFaultsInfoByDomain(const std::string &domain)
{
    std::vector<FaultInfo> faults;
    if (peripheralFaultsMap_.find(domain) == peripheralFaultsMap_.end()) {
        EDM_LOGE(MODULE_SERVICE, "fail to get faults info by domain %{public}s", domain.c_str());
        return faults;
    }
    return peripheralFaultsMap_[domain];
}

FaultInfo EventConfig::ExtractFaultInfo(const cJSON *faultItem)
{
    FaultInfo faultInfo;
    cJSON *nameObj = cJSON_GetObjectItemCaseSensitive(faultItem, "faultName");
    if (nameObj && cJSON_IsString(nameObj) && (strlen(nameObj->valuestring) > 0)) {
        faultInfo.faultName = nameObj->valuestring;
    }

    cJSON *typeObj = cJSON_GetObjectItemCaseSensitive(faultItem, "type");
    if (typeObj && cJSON_IsString(typeObj) && (strlen(typeObj->valuestring) > 0)) {
        faultInfo.type = typeObj->valuestring;
    }

    cJSON *titleObj = cJSON_GetObjectItemCaseSensitive(faultItem, "title");
    if (titleObj && cJSON_IsString(titleObj) && (strlen(titleObj->valuestring) > 0)) {
        faultInfo.title = titleObj->valuestring;
    }

    cJSON *msgObj = cJSON_GetObjectItemCaseSensitive(faultItem, "msg");
    if (msgObj && cJSON_IsString(msgObj) && (strlen(msgObj->valuestring) > 0)) {
        faultInfo.msg = msgObj->valuestring;
    }

    cJSON *uriObj = cJSON_GetObjectItemCaseSensitive(faultItem, "uri");
    if (uriObj && cJSON_IsString(uriObj) && (strlen(uriObj->valuestring) > 0)) {
        faultInfo.uri = uriObj->valuestring;
    }
    return faultInfo;
}

void EventConfig::DeleteJsonObj(cJSON *obj)
{
    if (obj &&!cJSON_IsNull(obj)) {
        cJSON_Delete(obj);
        obj = nullptr;
    }
}

bool EventConfig::ParseJsonFile(const std::string &targetPath, DomainFaultsMap &peripheralFaultMap)
{
    if (access(targetPath.c_str(), F_OK) != 0) {
        EDM_LOGE(MODULE_SERVICE, "targetPath %{public}s invalid", targetPath.c_str());
        return false;
    }

    std::ifstream file(targetPath.c_str(), std::ios::in | std::ios::binary);
    std::string content(std::istreambuf_iterator<char> {file}, std::istreambuf_iterator<char> {});
    file.close();

    cJSON *root = cJSON_Parse(content.c_str());
    if (!root) {
        EDM_LOGE(MODULE_SERVICE, "%{public}s json parse error.", targetPath.c_str());
        return false;
    }

    if (cJSON_IsNull(root) || !cJSON_IsArray(root)) {
        EDM_LOGE(MODULE_SERVICE, "%{public}s json root error", targetPath.c_str());
        DeleteJsonObj(root);
        return false;
    }

    cJSON *item = nullptr;
    cJSON_ArrayForEach(item, root)
    {
        std::string domain;
        cJSON *domainObj = cJSON_GetObjectItemCaseSensitive(item, "domain");
        if (!domainObj || !cJSON_IsString(domainObj) || (strlen(domainObj->valuestring) == 0)) {
            EDM_LOGE(MODULE_SERVICE, "domain %{public}s is invalid.", targetPath.c_str());
            DeleteJsonObj(root);
            return false;
        }

        domain = domainObj->valuestring;
        cJSON *faultArray = cJSON_GetObjectItemCaseSensitive(item, "fault");
        if (!faultArray || cJSON_IsNull(faultArray) || !cJSON_IsArray(faultArray)) {
            EDM_LOGE(MODULE_SERVICE, "%{public}s faultArray invalid.", targetPath.c_str());
            DeleteJsonObj(root);
            return false;
        }

        cJSON *faultItem = nullptr;
        std::vector<FaultInfo> faults;
        cJSON_ArrayForEach(faultItem, faultArray)
        {
            faults.push_back(ExtractFaultInfo(faultItem));
        }

        peripheralFaultMap[domain] = faults;
    }

    DeleteJsonObj(root);
    return true;
}

DomainFaultsMap EventConfig::FillFaultsMap(const DomainFaultsMap &ccmMap, const DomainFaultsMap &localMap)
{
    DomainFaultsMap resMap;
    DomainFaultsMap tempMap(localMap);

    for (const auto &[domain, ccmFaults] : ccmMap) {
        auto localIt = tempMap.find(domain);
        if (localIt == tempMap.end()) {
            EDM_LOGE(MODULE_SERVICE, "domain %{public}s has been configured in CCMMap but not configured in localMap",
                domain.c_str());
            continue;
        }
        const auto& localFaults = localIt->second;
        std::vector<FaultInfo> matchedFaults;
        for (const auto& ccmFault : ccmFaults) {
            auto it = std::find_if(localFaults.begin(), localFaults.end(),
                [&ccmFault](const FaultInfo& localFault) {
                    return localFault.faultName == ccmFault.faultName;
                });
            if (it != localFaults.end()) {
                matchedFaults.push_back(*it);
            } else {
                EDM_LOGE(MODULE_SERVICE,
                    "domain %{public}s and faultName %{public}s has been configured in CCMMap but not configured in "
                    "localMap",
                    domain.c_str(), ccmFault.faultName.c_str());
            }
        }
        if (matchedFaults.size() < localFaults.size()) {
            EDM_LOGE(MODULE_SERVICE, "localConfig has different faults with CCM for domain %{public}s", domain.c_str());
        }
        if (!matchedFaults.empty()) {
            resMap[domain] = std::move(matchedFaults);
        }
        tempMap.erase(localIt);
    }
    if (!tempMap.empty()) {
        EDM_LOGE(MODULE_SERVICE, "localMap contains %{public}lu domains that are not present in CCMMap",
            static_cast<unsigned long>(tempMap.size()));
    }
    return resMap;
}

bool EventConfig::ParseJsonFile()
{
    peripheralFaultsMap_.clear();
    DomainFaultsMap ccmMap;
    DomainFaultsMap localMap;
    bool bRet = ParseJsonFile(SYS_PROD_PERIPHERAL_FAULT_NOTIFIER_CONFIG_PATH, ccmMap);
    if (!bRet || ccmMap.size() == 0) {
        EDM_LOGE(MODULE_SERVICE, "Failed to parse the CCM configuration file");
        return false;
    }
    bRet = ParseJsonFile(SYSTEM_PERIPHERAL_FAULT_NOTIFIER_CONFIG_PATH, localMap);
    if (!bRet || localMap.size() == 0) {
        EDM_LOGE(MODULE_SERVICE, "Failed to parse the local configuration file");
        return false;
    }
    peripheralFaultsMap_ = FillFaultsMap(ccmMap, localMap);
    if (peripheralFaultsMap_.empty()) {
        EDM_LOGE(MODULE_SERVICE, "Failed to ParseJsonFile and peripheralFaultsMap_ is empty");
        return false;
    }
    return true;
}

FaultInfo EventConfig::GetFaultInfo(const std::string &domain, const std::string &faultName) const
{
    FaultInfo faultInfo;
    auto it = peripheralFaultsMap_.find(domain);
    if (it != peripheralFaultsMap_.end()) {
        for (const auto &fault : it->second) {
            if (fault.faultName == faultName) {
                return fault;
            }
        }
    }
    return faultInfo;
}
} // namespace ExternalDeviceManager
} // namespace OHOS
