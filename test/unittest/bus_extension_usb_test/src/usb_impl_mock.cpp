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

#include "usb_impl_mock.h"

namespace OHOS {
namespace USB {
using namespace OHOS;
using namespace OHOS::HDI;

std::vector<uint8_t> g_descBuf {
    0x12, 0x01, 0x20, 0x03, 0x00, 0x00, 0x00, 0x09, 0x07, 0x22, 0x18, 0x00, 0x23, 0x02, 0x01, 0x02, 0x03, 0x01, 0x09,
    0x02, 0x5D, 0x00, 0x02, 0x01, 0x04, 0xC0, 0x3E, 0x08, 0x0B, 0x00, 0x02, 0x02, 0x02, 0x01, 0x07, 0x09, 0x04, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x01, 0x05, 0x05, 0x24, 0x00, 0x10, 0x01, 0x05, 0x24, 0x01, 0x00, 0x01, 0x04, 0x24, 0x02,
    0x02, 0x05, 0x24, 0x06, 0x00, 0x01, 0x07, 0x05, 0x81, 0x03, 0x0A, 0x00, 0x09, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0A, 0x00, 0x02, 0x06, 0x07, 0x05, 0x82, 0x02, 0x00, 0x04, 0x00, 0x06, 0x30, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x05, 0x01, 0x02, 0x00, 0x04, 0x00, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00
};

std::vector<uint8_t> g_descBufErr {
    0x10, 0x01, 0x20, 0x03, 0x00, 0x00, 0x00, 0x09, 0x07, 0x22, 0x18, 0x00, 0x23, 0x02, 0x01, 0x02, 0x03, 0x01, 0x09,
    0x02, 0x5D, 0x00, 0x02, 0x01, 0x04, 0xC0, 0x3E, 0x08, 0x0B, 0x00, 0x02, 0x02, 0x02, 0x01, 0x07, 0x09, 0x04, 0x00,
    0x00, 0x01, 0x02, 0x02, 0x01, 0x05, 0x05, 0x24, 0x00, 0x10, 0x01, 0x05, 0x24, 0x01, 0x00, 0x01, 0x04, 0x24, 0x02,
    0x02, 0x05, 0x24, 0x06, 0x00, 0x01, 0x07, 0x05, 0x81, 0x03, 0x0A, 0x00, 0x09, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x09, 0x04, 0x01, 0x00, 0x02, 0x0A, 0x00, 0x02, 0x06, 0x07, 0x05, 0x82, 0x02, 0x00, 0x04, 0x00, 0x06, 0x30, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x05, 0x01, 0x02, 0x00, 0x04, 0x00, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00
};

UsbImplMock::UsbImplMock() { }

UsbImplMock::~UsbImplMock() { }

int32_t UsbImplMock::GetRawDescriptor(const UsbDev &dev, std::vector<uint8_t> &descriptor)
{
    if ((BUS_NUM_OK != dev.busNum) && (BUS_NUM_OK_2 != dev.busNum)) {
        return HDF_DEV_ERR_NO_DEVICE;
    }
    if ((DEV_ADDR_OK !=dev.devAddr) \
        && (DEV_ADDR_OK_2 != dev.devAddr)\
        && (DEV_ADDR_OK_ERR_DESC != dev.devAddr)\
        && (DEV_ADDR_OK_NULL_DESC != dev.devAddr)\
        && (DEV_ADDR_INTERFACE_ERR != dev.devAddr)) {
        return HDF_DEV_ERR_NO_DEVICE;
    }
    if (dev.devAddr == DEV_ADDR_OK_ERR_DESC) {
        descriptor = g_descBufErr;
    } else if (dev.devAddr == DEV_ADDR_OK_NULL_DESC) {
        // do nothing
    } else {
        descriptor = g_descBuf;
    }
    return HDF_SUCCESS;
}

int32_t UsbImplMock::OpenDevice(const UsbDev &dev)
{
    if (dev.busNum == BUS_NUM_ERR && dev.devAddr == DEV_ADDR_ERR) {
        return HDF_DEV_ERR_NO_DEVICE;
    }
    return HDF_SUCCESS;
}

int32_t UsbImplMock::GetDeviceDescriptor(const UsbDev& dev, std::vector<uint8_t>& descriptor)
{
    return GetRawDescriptor(dev, descriptor);
}
int32_t UsbImplMock::GetStringDescriptor(const UsbDev &dev, uint8_t descId, std::vector<uint8_t> &decriptor)
{
    (void)descId;
    if ((BUS_NUM_OK != dev.busNum) || (DEV_ADDR_OK != dev.devAddr)) {
        return HDF_DEV_ERR_NO_DEVICE;
    }
    decriptor = g_descBuf;
    return HDF_SUCCESS;
}

int32_t UsbImplMock::QueryPort(int32_t &portId, int32_t &powerRole, int32_t &dataRole, int32_t &mode)
{
    return HDF_SUCCESS;
}

int32_t UsbImplMock::BindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
{
    subscriber_ = subscriber;
    return HDF_SUCCESS;
}

int32_t UsbImplMock::UnbindUsbdSubscriber(const sptr<IUsbdSubscriber> &subscriber)
{
    (void)subscriber;
    subscriber_ = nullptr;
    return HDF_SUCCESS;
}

int32_t UsbImplMock::SetPortRole(int32_t portId, int32_t powerRole, int32_t dataRole)
{
    PortInfo info;
    info.dataRole = dataRole;
    info.portId = portId;
    info.powerRole = powerRole;
    return this->subscriber_->PortChangedEvent(info);
}

int32_t UsbImplMock::SubscriberDeviceEvent(const USBDeviceInfo &info)
{
    auto ret = subscriber_->DeviceEvent(info);
    return ret;
}
int32_t UsbImplMock::GetConfig(const UsbDev &dev, uint8_t &configIndex)
{
    (void)dev;
    configIndex = 1;
    return HDF_SUCCESS;
}
} // namespace USB
} // namespace OHOS
