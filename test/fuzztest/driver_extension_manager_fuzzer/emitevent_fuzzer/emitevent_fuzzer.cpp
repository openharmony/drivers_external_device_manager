/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "emitevent_fuzzer.h"
#include "driver_ext_mgr.h"

namespace OHOS {
namespace ExternalDeviceManager {
static bool EmitEventFuzzTest(const uint8_t *rawData, size_t size)
{
    OHOS::MessageParcel data;
    if (!data.WriteInterfaceToken(IDriverExtMgr::GetDescriptor())) {
        return false;
    }

    if (!data.WriteBuffer(rawData, size)) {
        return false;
    }

    OHOS::MessageParcel reply;
    OHOS::MessageOption option;
    int32_t code = static_cast<uint32_t>(DriverExtMgrInterfaceCode::INPUT_EMIT_EVENT);
    DelayedSingleton<DriverExtMgr>::GetInstance()->OnRemoteRequest(code, data, reply, option);
    return true;
}
} // namespace ExternalDeviceManager
} // namespace OHOS

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    OHOS::ExternalDeviceManager::EmitEventFuzzTest(data, size);
    return 0;
}