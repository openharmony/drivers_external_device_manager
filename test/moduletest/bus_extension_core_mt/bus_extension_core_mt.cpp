/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <dlfcn.h>

#include "edm_errors.h"
#include "hilog_wrapper.h"
#include "iostream"
#define private public
#include "etx_device_mgr.h"
#undef private

using namespace OHOS::ExternalDeviceManager;
using namespace std;
static void PrintAllDevice()
{
    ExtDeviceManager &devmgr = ExtDeviceManager::GetInstance();
    cout << "------------------" << endl;
    std::list<std::shared_ptr<Device>> list = devmgr.deviceMap_[BUS_TYPE_USB];
    cout << "usb device size: " << list.size() << endl;
    std::list<std::shared_ptr<Device>>::iterator iter;
    for (iter = list.begin(); iter != list.end(); iter++) {
        std::shared_ptr<Device> device = *iter;
        cout << device->GetDeviceInfo()->GetDeviceDescription().c_str() << endl;
    }
    cout << "------------------" << endl;
}

int main(int argc, char **argv)
{
    std::string libPath = "/system/lib/libbus_extension.z.so";
    void *handler = dlopen(libPath.c_str(), RTLD_LAZY);
    if (handler == nullptr) {
        cout << "dlopen libbus_extension.z.so failed" << endl;
        return -1;
    }
    BusExtensionCore &core = BusExtensionCore::GetInstance();
    size_t size = core.busExtensions_.size();
    cout << "busExtensions_.size: " << size << endl;
    int32_t ret = core.Init();
    if (ret != EDM_OK) {
        cout << "busExtensionCore init failed" << endl;
        return ret;
    }
    while (true) {
        std::string in;
        cin >> in;
        if (in == "q") {
            break;
        } else if (in == "p") {
            PrintAllDevice();
        } else {
            cout << in;
        }
    }
    cout << "exit!" << endl;
    return ret;
}