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
void
pa_proplist_free(pa_proplist* p)
{
    trace_info("F %s p=%p\n", __func__, p);

    g_hash_table_destroy(p->ht);
    free(p);
}

static
void
string_destroy_func(gpointer data)
{
    free(data);
}

APULSE_EXPORT
pa_proplist *
pa_proplist_new(void)
{
    trace_info("F %s (void)\n", __func__);

    pa_proplist *p = calloc(1, sizeof(pa_proplist));
    p->ht = g_hash_table_new_full(g_str_hash, g_str_equal,
                                  string_destroy_func, string_destroy_func);
    return p;
}

APULSE_EXPORT
int
pa_proplist_sets(pa_proplist *p, const char *key, const char *value)
{
    trace_info("F %s p=%p, key=%s, value=%s\n", __func__, p, key, value);

    g_hash_table_insert(p->ht, strdup(key), strdup(value));
    return 0;
}

APULSE_EXPORT
void
pa_proplist_clear(pa_proplist *p)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_proplist_update(pa_proplist *p, pa_update_mode_t mode, const pa_proplist *other)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
int
pa_proplist_contains(pa_proplist *p, const char *key)
{
    trace_info("F %s p=%p, key=%s\n", __func__, p, key);

    return g_hash_table_lookup(p->ht, key) != NULL;
}

APULSE_EXPORT
const char *
pa_proplist_gets(pa_proplist *p, const char *key)
{
    trace_info("F %s p=%p, key=%s\n", __func__, p, key);
    return g_hash_table_lookup(p->ht, key);
}
