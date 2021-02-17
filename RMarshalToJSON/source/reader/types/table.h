#pragma once

#include "util/types.h"

#include <memory>

struct Table
{
    i32 x_size;
    i32 y_size;
    i32 z_size;

    i32                    total_size;
    std::shared_ptr<i16[]> data;
};
