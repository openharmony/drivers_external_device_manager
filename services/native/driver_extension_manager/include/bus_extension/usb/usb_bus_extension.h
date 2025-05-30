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

#ifndef USB_BUS_EXTENSION_H
#define USB_BUS_EXTENSION_H
#include <iproxy_broker.h>
#include <iremote_object.h>

#include "ibus_extension.h"
#include "usb_dev_subscriber.h"
#include "usb_device_info.h"
#include "usb_driver_change_callback.h"
#include "v1_1/iusb_ddk.h"
#ifdef EXTDEVMGR_USB_PASS_THROUGH
#include "v2_0/iusb_host_interface.h"
#else
#include "v1_0/iusb_interface.h"
#endif // EXTDEVMGR_USB_PASS_THROUGH
namespace OHOS {
namespace ExternalDeviceManager {
using namespace std;
#ifdef EXTDEVMGR_USB_PASS_THROUGH
using namespace OHOS::HDI::Usb::V2_0;
#else
using namespace OHOS::HDI::Usb::V1_0;
#endif // EXTDEVMGR_USB_PASS_THROUGH
using namespace OHOS::HDI::Usb::Ddk;
class UsbBusExtension : public IBusExtension {
public:
    UsbBusExtension();
    ~UsbBusExtension();
    int32_t SetDevChangeCallback(shared_ptr<IDevChangeCallback> callback) override;
    bool MatchDriver(const DriverInfo &driver, const DeviceInfo &device, const std::string &type = "") override;
    shared_ptr<DriverInfoExt> ParseDriverInfo(const map<string, string> &metadata) override;
    shared_ptr<DriverInfoExt> GetNewDriverInfoExtObject() override;
#ifdef EXTDEVMGR_USB_PASS_THROUGH
    void SetUsbInferface(sptr<IUsbHostInterface> iusb);
#else
    void SetUsbInferface(sptr<IUsbInterface> iusb);
#endif // EXTDEVMGR_USB_PASS_THROUGH
    void SetUsbDdk(sptr<V1_1::IUsbDdk> iUsbDdk);
    BusType GetBusType() override;
    shared_ptr<IDriverChangeCallback> AcquireDriverChangeCallback() override;

private:
    sptr<UsbDevSubscriber> subScriber_ = nullptr;
    sptr<V1_1::IUsbDdk> iUsbDdk_ = nullptr;
    vector<uint16_t> ParseCommaStrToVectorUint16(const string &str);
#ifdef EXTDEVMGR_USB_PASS_THROUGH
    sptr<IUsbHostInterface> usbInterface_ = nullptr;
#else
    class UsbdDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        void OnRemoteDied(const wptr<IRemoteObject> &object) override;
    };
    sptr<IUsbInterface> usbInterface_ = nullptr; // in usb HDI;
    sptr<IRemoteObject::DeathRecipient> recipient_;
#endif // EXTDEVMGR_USB_PASS_THROUGH
};
}
}
#endif