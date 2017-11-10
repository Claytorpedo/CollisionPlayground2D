#pragma once
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H
#include "catch.hpp"
#include "../Constants.h"

#define ApproxEps(x) Approx(x).margin(constants::EPSILON)

#endif // _DEFINITIONS_H