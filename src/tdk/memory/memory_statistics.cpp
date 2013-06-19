#include "stdafx.h"
#include "memory_statistics.hpp"

namespace tdk { namespace memory {

statistics* statistics::tc_statistics = new statistics[ALLOC_SIZE_TABLE_COUNT];
statistics* statistics::base_statistics = new statistics[ALLOC_SIZE_TABLE_COUNT];


}}