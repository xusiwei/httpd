/*
 * Copyright (c) 2020, HiHope Community.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_at.h"

#include "httpd.h"
#include "net_params.h"
#include "wifi_connecter.h"

static int g_netId = -1;

static void HttpdTask(void* arg)
{
    (void) arg;
    WifiDeviceConfig config = {0};

    // 准备AP的配置参数
    strcpy(config.ssid, PARAM_HOTSPOT_SSID);
    strcpy(config.preSharedKey, PARAM_HOTSPOT_PSK);
    config.securityType = PARAM_HOTSPOT_TYPE;

    g_netId = ConnectToHotspot(&config);
    printf("netId = %d\r\n", g_netId);

    httpd_init();
}

static void HttpdEntry(void)
{
    osThreadAttr_t attr = {0};

    attr.name = "HttpdTask";
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;

    if (osThreadNew(HttpdTask, NULL, &attr) == NULL) {
        printf("[HttpdEntry] create HttpdTask failed!\n");
    }
}
SYS_RUN(HttpdEntry);
