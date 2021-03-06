/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#pragma once

typedef enum {
    ONE2ONE = 1, /* one signal, one filter   */
    MANY2ONE,    /* many signal, one filter  */
    MANY2MANY,   /* many signal, many filter */
    ONE2ALL,    /* one signal, many filter  */
} ConvolveBatchKind;
