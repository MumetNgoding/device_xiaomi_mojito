/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
   Copyright (C) 2016 The CyanogenMod Project.
   Copyright (C) 2019 The LineageOS Project.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/properties.h>
#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::base::SetProperty;
using std::string;

void property_override(string prop, string value)
{
    auto pi = (prop_info*) __system_property_find(prop.c_str());

    if (pi != nullptr)
        __system_property_update(pi, value.c_str(), value.size());
    else
        __system_property_add(prop.c_str(), prop.size(), value.c_str(), value.size());
}

void vendor_load_properties()
{
    string device, model;

    string hwname = GetProperty("ro.boot.hwname", "");

    string fingerprint = "google/redfin/redfin:11/RQ2A.210505.003/7255357:user/release-keys";
    string description = "redfin-user 11 RQ2A.210505.003 7255357 release-keys";

    if (hwname == "sunny") {
        device = "sunny";
        model = "M2101K7AG";
    } else {
        device = "mojito";
        model = "M2101K7AG";
    }

    // Override all partitions' props
    string prop_partitions[] = { "", "odm.", "product.", "system.", "system_ext.", "vendor." };
    for (const string &prop : prop_partitions) {
        property_override(string("ro.product.") + prop + string("board"), device);
        property_override(string("ro.product.") + prop + string("device"), device);
        property_override(string("ro.product.") + prop + string("name"), device);
        property_override(string("ro.product.") + prop + string("model"), model);
        property_override(string("ro.") + prop + string("build.product"), device);
    }

    // Set hardware SKU prop
    property_override("ro.boot.product.hardware.sku", device);
}
