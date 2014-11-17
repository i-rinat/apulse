/*
 * Copyright © 2014  Rinat Ibragimov
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


APULSE_EXPORT
void pa_mainloop_api_once(pa_mainloop_api*m, void (*callback)(pa_mainloop_api*m, void *userdata), void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
size_t pa_sample_size_of_format(pa_sample_format_t f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_sample_spec* pa_sample_spec_init(pa_sample_spec *spec)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_sample_spec_equal(const pa_sample_spec*a, const pa_sample_spec*b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_sample_format_t pa_parse_sample_format(const char *format)
{
    trace_info("Z %s\n", __func__);
    return 0;
}


APULSE_EXPORT
char* pa_bytes_snprint(char *s, size_t l, unsigned v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_sample_format_is_le(pa_sample_format_t f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_sample_format_is_be(pa_sample_format_t f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_key_valid(const char *key)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_setp(pa_proplist *p, const char *pair)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_setf(pa_proplist *p, const char *key, const char *format, ...)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_set(pa_proplist *p, const char *key, const void *data, size_t nbytes)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_get(pa_proplist *p, const char *key, const void **data, size_t *nbytes)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_unset(pa_proplist *p, const char *key)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_unset_many(pa_proplist *p, const char * const keys[])
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
const char *pa_proplist_iterate(pa_proplist *p, void **state)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_proplist_to_string(pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_proplist_to_string_sep(pa_proplist *p, const char *sep)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_proplist *pa_proplist_from_string(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_proplist* pa_proplist_copy(const pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
unsigned pa_proplist_size(pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_isempty(pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_proplist_equal(pa_proplist *a, pa_proplist *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_channel_map* pa_channel_map_init(pa_channel_map *m)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const char* pa_channel_position_to_string(pa_channel_position_t pos)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_channel_position_t pa_channel_position_from_string(const char *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
const char* pa_channel_position_to_pretty_string(pa_channel_position_t pos)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char* pa_channel_map_snprint(char *s, size_t l, const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_channel_map *pa_channel_map_parse(pa_channel_map *map, const char *s)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_channel_map_equal(const pa_channel_map *a, const pa_channel_map *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_channel_map_valid(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_channel_map_superset(const pa_channel_map *a, const pa_channel_map *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_channel_map_can_balance(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_channel_map_can_fade(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
const char* pa_channel_map_to_name(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const char* pa_channel_map_to_pretty_name(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_channel_map_has_position(const pa_channel_map *map, pa_channel_position_t p)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_channel_position_mask_t pa_channel_map_mask(const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
const char *pa_encoding_to_string(pa_encoding_t e)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_encoding_t pa_encoding_from_string(const char *encoding)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_format_info* pa_format_info_new(void)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_format_info* pa_format_info_copy(const pa_format_info *src)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void pa_format_info_free(pa_format_info *f)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
int pa_format_info_valid(const pa_format_info *f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_is_pcm(const pa_format_info *f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_is_compatible(const pa_format_info *first, const pa_format_info *second)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
char *pa_format_info_snprint(char *s, size_t l, const pa_format_info *f)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_format_info* pa_format_info_from_string(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_format_info* pa_format_info_from_sample_spec(const pa_sample_spec *ss, const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_format_info_to_sample_spec(const pa_format_info *f, pa_sample_spec *ss, pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_prop_type_t pa_format_info_get_prop_type(const pa_format_info *f, const char *key)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_get_prop_int(const pa_format_info *f, const char *key, int *v)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_get_prop_int_range(const pa_format_info *f, const char *key, int *min, int *max)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_get_prop_int_array(const pa_format_info *f, const char *key, int **values, int *n_values)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_get_prop_string(const pa_format_info *f, const char *key, char **v)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_format_info_get_prop_string_array(const pa_format_info *f, const char *key, char ***values, int *n_values)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
void pa_format_info_free_string_array(char **values, int n_values)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_prop_int(pa_format_info *f, const char *key, int value)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_prop_int_array(pa_format_info *f, const char *key, const int *values, int n_values)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_prop_int_range(pa_format_info *f, const char *key, int min, int max)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_prop_string(pa_format_info *f, const char *key, const char *value)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_prop_string_array(pa_format_info *f, const char *key, const char **values, int n_values)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_sample_format(pa_format_info *f, pa_sample_format_t sf)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_rate(pa_format_info *f, int rate)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_channels(pa_format_info *f, int channels)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_format_info_set_channel_map(pa_format_info *f, const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_operation_set_state_callback(pa_operation *o, pa_operation_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_context_set_event_callback(pa_context *p, pa_context_event_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
int pa_context_is_pending(pa_context *c)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_operation* pa_context_exit_daemon(pa_context *c, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_default_sink(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_default_source(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_context_is_local(pa_context *c)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_operation* pa_context_set_name(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const char* pa_context_get_server(pa_context *c)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
uint32_t pa_context_get_protocol_version(pa_context *c)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
uint32_t pa_context_get_server_protocol_version(pa_context *c)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_operation *pa_context_proplist_update(pa_context *c, pa_update_mode_t mode, pa_proplist *p, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *pa_context_proplist_remove(pa_context *c, const char *const keys[], pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
uint32_t pa_context_get_index(pa_context *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_time_event* pa_context_rttime_new(pa_context *c, pa_usec_t usec, pa_time_event_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void pa_context_rttime_restart(pa_context *c, pa_time_event *e, pa_usec_t usec)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
size_t pa_context_get_tile_size(pa_context *c, const pa_sample_spec *ss)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_context_load_cookie_from_file(pa_context *c, const char *cookie_file_path)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_cvolume_equal(const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_init(pa_cvolume *a)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_cvolume_snprint(char *s, size_t l, const pa_cvolume *c)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_sw_cvolume_snprint_dB(char *s, size_t l, const pa_cvolume *c)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_cvolume_snprint_verbose(char *s, size_t l, const pa_cvolume *c, const pa_channel_map *map, int print_dB)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_volume_snprint(char *s, size_t l, pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_sw_volume_snprint_dB(char *s, size_t l, pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_volume_snprint_verbose(char *s, size_t l, pa_volume_t v, int print_dB)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_avg_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_max(const pa_cvolume *a)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_max_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_min(const pa_cvolume *a)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_min_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_cvolume_valid(const pa_cvolume *v)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_cvolume_channels_equal_to(const pa_cvolume *a, pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t pa_sw_volume_multiply(pa_volume_t a, pa_volume_t b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume *pa_sw_cvolume_multiply(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *pa_sw_cvolume_multiply_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t pa_sw_volume_divide(pa_volume_t a, pa_volume_t b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume *pa_sw_cvolume_divide(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *pa_sw_cvolume_divide_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t pa_sw_volume_from_dB(double f)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
double pa_sw_volume_to_dB(pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_volume_t pa_sw_volume_from_linear(double v)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
double pa_sw_volume_to_linear(pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume *pa_cvolume_remap(pa_cvolume *v, const pa_channel_map *from, const pa_channel_map *to)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_cvolume_compatible(const pa_cvolume *v, const pa_sample_spec *ss)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_cvolume_compatible_with_channel_map(const pa_cvolume *v, const pa_channel_map *cm)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
float pa_cvolume_get_balance(const pa_cvolume *v, const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_set_balance(pa_cvolume *v, const pa_channel_map *map, float new_balance)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
float pa_cvolume_get_fade(const pa_cvolume *v, const pa_channel_map *map)
{
    trace_info("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_set_fade(pa_cvolume *v, const pa_channel_map *map, float new_fade)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_scale(pa_cvolume *v, pa_volume_t max)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_scale_mask(pa_cvolume *v, pa_volume_t max, pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_set_position(pa_cvolume *cv, const pa_channel_map *map, pa_channel_position_t t, pa_volume_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t pa_cvolume_get_position(pa_cvolume *cv, const pa_channel_map *map, pa_channel_position_t t)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_merge(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_inc_clamp(pa_cvolume *v, pa_volume_t inc, pa_volume_t limit)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_inc(pa_cvolume *v, pa_volume_t inc)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume* pa_cvolume_dec(pa_cvolume *v, pa_volume_t dec)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_stream *pa_stream_new_extended(pa_context *c, const char *name, pa_format_info * const * formats, unsigned int n_formats, pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_context* pa_stream_get_context(pa_stream *p)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const char *pa_stream_get_device_name(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_stream_is_suspended(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_stream_is_corked(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_stream_begin_write(pa_stream *p, void **data, size_t *nbytes)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_stream_cancel_write(pa_stream *p)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
void pa_stream_set_overflow_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
int64_t pa_stream_get_underflow_index(pa_stream *p)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
void pa_stream_set_started_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_stream_set_moved_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_stream_set_suspended_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_stream_set_event_callback(pa_stream *p, pa_stream_event_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void pa_stream_set_buffer_attr_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
pa_operation* pa_stream_prebuf(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_stream_get_time(pa_stream *s, pa_usec_t *r_usec)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
const pa_channel_map* pa_stream_get_channel_map(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const pa_format_info* pa_stream_get_format_info(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *pa_stream_set_buffer_attr(pa_stream *s, const pa_buffer_attr *attr, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *pa_stream_update_sample_rate(pa_stream *s, uint32_t rate, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *pa_stream_proplist_update(pa_stream *s, pa_update_mode_t mode, pa_proplist *p, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation *pa_stream_proplist_remove(pa_stream *s, const char *const keys[], pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_stream_set_monitor_stream(pa_stream *s, uint32_t sink_input_idx)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
uint32_t pa_stream_get_monitor_stream(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_operation* pa_context_get_sink_info_by_name(pa_context *c, const char *name, pa_sink_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sink_info_by_index(pa_context *c, uint32_t idx, pa_sink_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sink_info_list(pa_context *c, pa_sink_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_volume_by_index(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_volume_by_name(pa_context *c, const char *name, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_mute_by_index(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_mute_by_name(pa_context *c, const char *name, int mute, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_suspend_sink_by_name(pa_context *c, const char *sink_name, int suspend, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_suspend_sink_by_index(pa_context *c, uint32_t idx, int suspend, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_port_by_index(pa_context *c, uint32_t idx, const char*port, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_sink_port_by_name(pa_context *c, const char*name, const char*port, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_source_info_by_name(pa_context *c, const char *name, pa_source_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_source_info_list(pa_context *c, pa_source_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_volume_by_name(pa_context *c, const char *name, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_mute_by_index(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_mute_by_name(pa_context *c, const char *name, int mute, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_suspend_source_by_name(pa_context *c, const char *source_name, int suspend, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_suspend_source_by_index(pa_context *c, uint32_t idx, int suspend, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_port_by_index(pa_context *c, uint32_t idx, const char*port, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_port_by_name(pa_context *c, const char*name, const char*port, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_server_info(pa_context *c, pa_server_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_module_info(pa_context *c, uint32_t idx, pa_module_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_module_info_list(pa_context *c, pa_module_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_load_module(pa_context *c, const char*name, const char *argument, pa_context_index_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_unload_module(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_client_info(pa_context *c, uint32_t idx, pa_client_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_client_info_list(pa_context *c, pa_client_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_kill_client(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_card_info_by_index(pa_context *c, uint32_t idx, pa_card_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_card_info_by_name(pa_context *c, const char *name, pa_card_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_card_info_list(pa_context *c, pa_card_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_card_profile_by_index(pa_context *c, uint32_t idx, const char*profile, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_card_profile_by_name(pa_context *c, const char*name, const char*profile, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_port_latency_offset(pa_context *c, const char *card_name, const char *port_name, int64_t offset, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sink_input_info_list(pa_context *c, pa_sink_input_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_move_sink_input_by_name(pa_context *c, uint32_t idx, const char *sink_name, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_move_sink_input_by_index(pa_context *c, uint32_t idx, uint32_t sink_idx, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_kill_sink_input(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_source_output_info_list(pa_context *c, pa_source_output_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_move_source_output_by_name(pa_context *c, uint32_t idx, const char *source_name, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_move_source_output_by_index(pa_context *c, uint32_t idx, uint32_t source_idx, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_output_volume(pa_context *c, uint32_t idx, const pa_cvolume *volume, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_set_source_output_mute(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_kill_source_output(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_stat(pa_context *c, pa_stat_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sample_info_by_name(pa_context *c, const char *name, pa_sample_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sample_info_by_index(pa_context *c, uint32_t idx, pa_sample_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_sample_info_list(pa_context *c, pa_sample_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_autoload_info_by_name(pa_context *c, const char *name, pa_autoload_type_t type, pa_autoload_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_autoload_info_by_index(pa_context *c, uint32_t idx, pa_autoload_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_get_autoload_info_list(pa_context *c, pa_autoload_info_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_add_autoload(pa_context *c, const char *name, pa_autoload_type_t type, const char *module, const char*argument, pa_context_index_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_remove_autoload_by_name(pa_context *c, const char *name, pa_autoload_type_t type, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_remove_autoload_by_index(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void* userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_stream_connect_upload(pa_stream *s, size_t length)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_stream_finish_upload(pa_stream *s)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_operation* pa_context_remove_sample(pa_context *c, const char *name, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_play_sample(pa_context *c, const char *name, const char *dev, pa_volume_t volume, pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_operation* pa_context_play_sample_with_proplist(pa_context *c, const char *name, const char *dev, pa_volume_t volume, pa_proplist *proplist, pa_context_play_sample_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void *pa_xrealloc(void *ptr, size_t size)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_xstrdup(const char *s)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_xstrndup(const char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void* pa_xmemdup(const void *p, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_utf8_valid(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_ascii_valid(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_utf8_filter(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_ascii_filter(const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char* pa_utf8_to_locale (const char *str)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void pa_threaded_mainloop_accept(pa_threaded_mainloop *m)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
int pa_threaded_mainloop_get_retval(pa_threaded_mainloop *m)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
void pa_threaded_mainloop_set_name(pa_threaded_mainloop *m, const char *name)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
char *pa_get_user_name(char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_get_host_name(char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_get_fqdn(char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_get_home_dir(char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *pa_get_binary_name(char *s, size_t l)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int pa_msleep(unsigned long t)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
struct timeval *pa_gettimeofday(struct timeval *tv)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_usec_t pa_timeval_diff(const struct timeval *a, const struct timeval *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int pa_timeval_cmp(const struct timeval *a, const struct timeval *b)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_usec_t pa_timeval_age(const struct timeval *tv)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
struct timeval* pa_timeval_add(struct timeval *tv, pa_usec_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
struct timeval* pa_timeval_sub(struct timeval *tv, pa_usec_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
struct timeval* pa_timeval_store(struct timeval *tv, pa_usec_t v)
{
    trace_info("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_usec_t pa_timeval_load(const struct timeval *tv)
{
    trace_info("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_usec_t pa_rtclock_now(void)
{
    trace_info("Z %s\n", __func__);
    return 0;
}
