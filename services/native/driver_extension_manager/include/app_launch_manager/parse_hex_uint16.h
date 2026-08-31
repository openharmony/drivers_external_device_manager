/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef OHOS_EXTERNAL_DEVICE_MANAGER_PARSE_HEX_UINT16_H
#define OHOS_EXTERNAL_DEVICE_MANAGER_PARSE_HEX_UINT16_H

#include <charconv>
#include <cstdint>
#include <cstring>
#include <string>
#include <system_error>

namespace OHOS {
namespace ExternalDeviceManager {
inline bool ParseHexUint16(const char *text, uint16_t &out)
{
    if (text == nullptr || *text == '\0') {
        return false;
    }
    uint16_t value = 0;
    const char *last = text + std::strlen(text);
    auto result = std::from_chars(text, last, value, 16);
    if (result.ec != std::errc() || result.ptr != last) {
        return false;
    }
    out = value;
    return true;
}

inline bool ParseHexUint16(const std::string &text, uint16_t &out)
{
    return ParseHexUint16(text.c_str(), out);
}
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // OHOS_EXTERNAL_DEVICE_MANAGER_PARSE_HEX_UINT16_H
