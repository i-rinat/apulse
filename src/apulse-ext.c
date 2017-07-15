/*
 * Copyright © 2017  Rinat Ibragimov
 *
 * This file is part of "apulse" project.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "apulse.h"
#include "trace.h"
#include <pulse/ext-device-manager.h>
#include <pulse/ext-device-restore.h>
#include <pulse/ext-stream-restore.h>

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_test(pa_context *c, pa_ext_device_manager_test_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_read(pa_context *c, pa_ext_device_manager_read_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_set_device_description(pa_context *c, const char *device,
                                             const char *description, pa_context_success_cb_t cb,
                                             void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_delete(pa_context *c, const char *const s[], pa_context_success_cb_t cb,
                             void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_enable_role_device_priority_routing(pa_context *c, int enable,
                                                          pa_context_success_cb_t cb,
                                                          void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_reorder_devices_for_role(pa_context *c, const char *role,
                                               const char **devices, pa_context_success_cb_t cb,
                                               void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_manager_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb,
                                void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void
pa_ext_device_manager_set_subscribe_cb(pa_context *c, pa_ext_device_manager_subscribe_cb_t cb,
                                       void *userdata)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
pa_operation *
pa_ext_device_restore_test(pa_context *c, pa_ext_device_restore_test_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_restore_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb,
                                void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void
pa_ext_device_restore_set_subscribe_cb(pa_context *c, pa_ext_device_restore_subscribe_cb_t cb,
                                       void *userdata)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
pa_operation *
pa_ext_device_restore_read_formats_all(pa_context *c,
                                       pa_ext_device_restore_read_device_formats_cb_t cb,
                                       void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_restore_read_formats(pa_context *c, pa_device_type_t type, uint32_t idx,
                                   pa_ext_device_restore_read_device_formats_cb_t cb,
                                   void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_device_restore_save_formats(pa_context *c, pa_device_type_t type, uint32_t idx,
                                   uint8_t n_formats, pa_format_info **formats,
                                   pa_context_success_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_stream_restore_test(pa_context *c, pa_ext_stream_restore_test_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_stream_restore_read(pa_context *c, pa_ext_stream_restore_read_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_stream_restore_write(pa_context *c, pa_update_mode_t mode,
                            const pa_ext_stream_restore_info data[], unsigned n,
                            int apply_immediately, pa_context_success_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_ext_stream_restore_delete(pa_context *c, const char *const s[], pa_context_success_cb_t cb,
                             void *userdata);

APULSE_EXPORT
pa_operation *
pa_ext_stream_restore_subscribe(pa_context *c, int enable, pa_context_success_cb_t cb,
                                void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void
pa_ext_stream_restore_set_subscribe_cb(pa_context *c, pa_ext_stream_restore_subscribe_cb_t cb,
                                       void *userdata)
{
    trace_info_z("Z %s\n", __func__);
}
