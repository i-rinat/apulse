/*
 * Copyright © 2014-2015  Rinat Ibragimov
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

static const char *channel_name[PA_CHANNEL_POSITION_MAX] = {
    [PA_CHANNEL_POSITION_MONO] = "mono",
    [PA_CHANNEL_POSITION_FRONT_CENTER] = "front-center",
    [PA_CHANNEL_POSITION_FRONT_LEFT] = "front-left",
    [PA_CHANNEL_POSITION_FRONT_RIGHT] = "front-right",
    [PA_CHANNEL_POSITION_REAR_CENTER] = "rear-center",
    [PA_CHANNEL_POSITION_REAR_LEFT] = "rear-left",
    [PA_CHANNEL_POSITION_REAR_RIGHT] = "rear-right",
    [PA_CHANNEL_POSITION_LFE] = "lfe",
    [PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER] = "front-left-of-center",
    [PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER] = "front-right-of-center",
    [PA_CHANNEL_POSITION_SIDE_LEFT] = "side-left",
    [PA_CHANNEL_POSITION_SIDE_RIGHT] = "side-right",
    [PA_CHANNEL_POSITION_AUX0] = "aux0",
    [PA_CHANNEL_POSITION_AUX1] = "aux1",
    [PA_CHANNEL_POSITION_AUX2] = "aux2",
    [PA_CHANNEL_POSITION_AUX3] = "aux3",
    [PA_CHANNEL_POSITION_AUX4] = "aux4",
    [PA_CHANNEL_POSITION_AUX5] = "aux5",
    [PA_CHANNEL_POSITION_AUX6] = "aux6",
    [PA_CHANNEL_POSITION_AUX7] = "aux7",
    [PA_CHANNEL_POSITION_AUX8] = "aux8",
    [PA_CHANNEL_POSITION_AUX9] = "aux9",
    [PA_CHANNEL_POSITION_AUX10] = "aux10",
    [PA_CHANNEL_POSITION_AUX11] = "aux11",
    [PA_CHANNEL_POSITION_AUX12] = "aux12",
    [PA_CHANNEL_POSITION_AUX13] = "aux13",
    [PA_CHANNEL_POSITION_AUX14] = "aux14",
    [PA_CHANNEL_POSITION_AUX15] = "aux15",
    [PA_CHANNEL_POSITION_AUX16] = "aux16",
    [PA_CHANNEL_POSITION_AUX17] = "aux17",
    [PA_CHANNEL_POSITION_AUX18] = "aux18",
    [PA_CHANNEL_POSITION_AUX19] = "aux19",
    [PA_CHANNEL_POSITION_AUX20] = "aux20",
    [PA_CHANNEL_POSITION_AUX21] = "aux21",
    [PA_CHANNEL_POSITION_AUX22] = "aux22",
    [PA_CHANNEL_POSITION_AUX23] = "aux23",
    [PA_CHANNEL_POSITION_AUX24] = "aux24",
    [PA_CHANNEL_POSITION_AUX25] = "aux25",
    [PA_CHANNEL_POSITION_AUX26] = "aux26",
    [PA_CHANNEL_POSITION_AUX27] = "aux27",
    [PA_CHANNEL_POSITION_AUX28] = "aux28",
    [PA_CHANNEL_POSITION_AUX29] = "aux29",
    [PA_CHANNEL_POSITION_AUX30] = "aux30",
    [PA_CHANNEL_POSITION_AUX31] = "aux31",
    [PA_CHANNEL_POSITION_TOP_CENTER] = "top-center",
    [PA_CHANNEL_POSITION_TOP_FRONT_CENTER] = "top-front-center",
    [PA_CHANNEL_POSITION_TOP_FRONT_LEFT] = "top-front-left",
    [PA_CHANNEL_POSITION_TOP_FRONT_RIGHT] = "top-front-right",
    [PA_CHANNEL_POSITION_TOP_REAR_CENTER] = "top-rear-center",
    [PA_CHANNEL_POSITION_TOP_REAR_LEFT] = "top-rear-left",
    [PA_CHANNEL_POSITION_TOP_REAR_RIGHT] = "top-rear-right",
};

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init(pa_channel_map *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);
    if (m)
        memset(m, 0, sizeof(*m));
    return m;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_auto(pa_channel_map *m, unsigned channels, pa_channel_map_def_t def)
{
    trace_info_z("Z %s m=%p, channels=%u, def=%u\n", __func__, m, channels, def);

    return NULL;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_mono(pa_channel_map *m)
{
    trace_info_f("F %s\n", __func__);

    pa_channel_map *cm = calloc(1, sizeof(pa_channel_map));
    cm->channels = 1;
    cm->map[0] = PA_CHANNEL_POSITION_MONO;

    return cm;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_stereo(pa_channel_map *m)
{
    trace_info_f("F %s\n", __func__);

    pa_channel_map *cm = calloc(1, sizeof(pa_channel_map));
    cm->channels = 2;
    cm->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
    cm->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;

    return cm;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_extend(pa_channel_map *m, unsigned channels, pa_channel_map_def_t def)
{
    trace_info_z("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
int
pa_channel_map_compatible(const pa_channel_map *map, const pa_sample_spec *ss)
{
    trace_info_z("Z %s\n", __func__);

    return 1;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_parse(pa_channel_map *map, const char *s)
{
    trace_info_f("F %s map=%p, s=%s\n", __func__, map, s);

    pa_channel_map m = {};

    if (strcmp(s, "stereo") == 0) {
        m.channels = 2;
        m.map[0] = PA_CHANNEL_POSITION_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_RIGHT;
    } else if (strcmp(s, "surround-21") == 0) {
        m.channels = 3;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_LFE;
    } else if (strcmp(s, "surround-40") == 0) {
        m.channels = 4;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
        m.map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
    } else if (strcmp(s, "surround-41") == 0) {
        m.channels = 5;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
        m.map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
        m.map[4] = PA_CHANNEL_POSITION_LFE;
    } else if (strcmp(s, "surround-50") == 0) {
        m.channels = 5;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
        m.map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
        m.map[4] = PA_CHANNEL_POSITION_FRONT_CENTER;
    } else if (strcmp(s, "surround-51") == 0) {
        m.channels = 6;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
        m.map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
        m.map[4] = PA_CHANNEL_POSITION_FRONT_CENTER;
        m.map[5] = PA_CHANNEL_POSITION_LFE;
    } else if (strcmp(s, "surround-51") == 0) {
        m.channels = 8;
        m.map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
        m.map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;
        m.map[2] = PA_CHANNEL_POSITION_REAR_LEFT;
        m.map[3] = PA_CHANNEL_POSITION_REAR_RIGHT;
        m.map[4] = PA_CHANNEL_POSITION_FRONT_CENTER;
        m.map[5] = PA_CHANNEL_POSITION_LFE;
        m.map[6] = PA_CHANNEL_POSITION_SIDE_LEFT;
        m.map[7] = PA_CHANNEL_POSITION_SIDE_RIGHT;
    }

    if (m.channels > 0) {
        // it was one of the predefined setups above
        *map = m;
        return map;
    }

    char **p = g_strsplit(s, ",", PA_CHANNELS_MAX);

    int k = 0;
    while (k < PA_CHANNELS_MAX && p[k]) {
        m.channels = k + 1;
        m.map[k] = pa_channel_position_from_string(p[k]);
        k ++;
    }

    g_strfreev(p);
    *map = m;
    return map;
}

APULSE_EXPORT
char *
pa_channel_map_snprint(char *s, size_t l, const pa_channel_map *map)
{
    trace_info_f("F %s s=%p, l=%d, map=%p\n", __func__, s, (int)l, map);
    char *ptr = s;

    if (!pa_channel_map_valid(map)) {
        snprintf(s, l, "(invalid)");
        return s;
    }

    for (int c = 0; c < map->channels && l > 1; c ++) {
        int adv = snprintf(ptr, l, "%s%s", (c == 0) ? "" : ",",
                           pa_channel_position_to_string(map->map[c]));
        ptr += adv;
        l -= adv;
    }

    return s;
}

APULSE_EXPORT
int
pa_channel_map_valid(const pa_channel_map *map)
{
    trace_info_f("F %s map=%p\n", __func__, map);

    const int channel_count_valid = (0 < map->channels && map->channels <= PA_CHANNELS_MAX);
    if (!channel_count_valid)
        return 0;

    for (int k = 0; k < map->channels; k ++) {
        const int channel_valid = (0 < map->map[k] && map->map[k] <= PA_CHANNEL_POSITION_MAX);
        if (!channel_valid)
            return 0;
    }

    return 1;
}

APULSE_EXPORT
pa_channel_position_t
pa_channel_position_from_string(const char *s)
{
    trace_info_f("F %s s=%s\n", __func__, s);

    if (!s)
        return PA_CHANNEL_POSITION_INVALID;

    for (unsigned int k = 0; k < PA_CHANNEL_POSITION_MAX; k ++) {
        if (!channel_name[k])
            continue;
        if (strcmp(channel_name[k], s) == 0)
            return k;
    }

    return PA_CHANNEL_POSITION_INVALID;
}

APULSE_EXPORT
const char *
pa_channel_position_to_string(pa_channel_position_t pos)
{
    trace_info_f("F %s pos=%u\n", __func__, pos);

    if (pos < 0 || pos >= PA_CHANNEL_POSITION_MAX)
        return "unknown";

    const char *s = channel_name[pos];
    if (!s)
        return "unknown";

    return s;
}
