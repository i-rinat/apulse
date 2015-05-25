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


APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_auto(pa_channel_map *m, unsigned channels, pa_channel_map_def_t def)
{
    trace_info("Z %s m=%p, channels=%u, def=%u\n", __func__, m, channels, def);

    return NULL;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_mono(pa_channel_map *m)
{
    trace_info("F %s\n", __func__);

    pa_channel_map *cm = calloc(1, sizeof(pa_channel_map));
    cm->channels = 1;
    cm->map[0] = PA_CHANNEL_POSITION_MONO;

    return cm;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_stereo(pa_channel_map *m)
{
    trace_info("F %s\n", __func__);

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
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
int
pa_channel_map_compatible(const pa_channel_map *map, const pa_sample_spec *ss)
{
    trace_info("Z %s\n", __func__);

    return 1;
}

APULSE_EXPORT
char *
pa_channel_map_snprint(char *s, size_t l, const pa_channel_map *map)
{
    trace_info("F %s s=%p, l=%d, map=%p\n", __func__, s, (int)l, map);
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
    trace_info("F %s map=%p\n", __func__, map);

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
const char *
pa_channel_position_to_string(pa_channel_position_t pos)
{
    trace_info("F %s pos=%u\n", __func__, pos);

    switch (pos) {
    case PA_CHANNEL_POSITION_MONO:                  return "mono";
    case PA_CHANNEL_POSITION_FRONT_CENTER:          return "front-center";
    case PA_CHANNEL_POSITION_FRONT_LEFT:            return "front-left";
    case PA_CHANNEL_POSITION_FRONT_RIGHT:           return "front-right";
    case PA_CHANNEL_POSITION_REAR_CENTER:           return "rear-center";
    case PA_CHANNEL_POSITION_REAR_LEFT:             return "rear-left";
    case PA_CHANNEL_POSITION_REAR_RIGHT:            return "rear-right";
    case PA_CHANNEL_POSITION_LFE:                   return "lfe";
    case PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER:  return "front-left-of-center";
    case PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER: return "front-right-of-center";
    case PA_CHANNEL_POSITION_SIDE_LEFT:             return "side-left";
    case PA_CHANNEL_POSITION_SIDE_RIGHT:            return "side-right";
    case PA_CHANNEL_POSITION_AUX0:                  return "aux0";
    case PA_CHANNEL_POSITION_AUX1:                  return "aux1";
    case PA_CHANNEL_POSITION_AUX2:                  return "aux2";
    case PA_CHANNEL_POSITION_AUX3:                  return "aux3";
    case PA_CHANNEL_POSITION_AUX4:                  return "aux4";
    case PA_CHANNEL_POSITION_AUX5:                  return "aux5";
    case PA_CHANNEL_POSITION_AUX6:                  return "aux6";
    case PA_CHANNEL_POSITION_AUX7:                  return "aux7";
    case PA_CHANNEL_POSITION_AUX8:                  return "aux8";
    case PA_CHANNEL_POSITION_AUX9:                  return "aux9";
    case PA_CHANNEL_POSITION_AUX10:                 return "aux10";
    case PA_CHANNEL_POSITION_AUX11:                 return "aux11";
    case PA_CHANNEL_POSITION_AUX12:                 return "aux12";
    case PA_CHANNEL_POSITION_AUX13:                 return "aux13";
    case PA_CHANNEL_POSITION_AUX14:                 return "aux14";
    case PA_CHANNEL_POSITION_AUX15:                 return "aux15";
    case PA_CHANNEL_POSITION_AUX16:                 return "aux16";
    case PA_CHANNEL_POSITION_AUX17:                 return "aux17";
    case PA_CHANNEL_POSITION_AUX18:                 return "aux18";
    case PA_CHANNEL_POSITION_AUX19:                 return "aux19";
    case PA_CHANNEL_POSITION_AUX20:                 return "aux20";
    case PA_CHANNEL_POSITION_AUX21:                 return "aux21";
    case PA_CHANNEL_POSITION_AUX22:                 return "aux22";
    case PA_CHANNEL_POSITION_AUX23:                 return "aux23";
    case PA_CHANNEL_POSITION_AUX24:                 return "aux24";
    case PA_CHANNEL_POSITION_AUX25:                 return "aux25";
    case PA_CHANNEL_POSITION_AUX26:                 return "aux26";
    case PA_CHANNEL_POSITION_AUX27:                 return "aux27";
    case PA_CHANNEL_POSITION_AUX28:                 return "aux28";
    case PA_CHANNEL_POSITION_AUX29:                 return "aux29";
    case PA_CHANNEL_POSITION_AUX30:                 return "aux30";
    case PA_CHANNEL_POSITION_AUX31:                 return "aux31";
    case PA_CHANNEL_POSITION_TOP_CENTER:            return "top-center";
    case PA_CHANNEL_POSITION_TOP_FRONT_CENTER:      return "top-front-center";
    case PA_CHANNEL_POSITION_TOP_FRONT_LEFT:        return "top-front-left";
    case PA_CHANNEL_POSITION_TOP_FRONT_RIGHT:       return "top-front-right";
    case PA_CHANNEL_POSITION_TOP_REAR_CENTER:       return "top-rear-center";
    case PA_CHANNEL_POSITION_TOP_REAR_LEFT:         return "top-rear-left";
    case PA_CHANNEL_POSITION_TOP_REAR_RIGHT:        return "top-rear-right";
    default:                                        return "unknown";
    }
}
