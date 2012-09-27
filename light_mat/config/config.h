/**
 * @file config.h
 *
 * @brief The configuration file for Basic Computation Supporting Library
 *
 * @author dhlin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHTMAT_CONFIG_H
#define LIGHTMAT_CONFIG_H

#include <light_mat/config/user_config.h>
#include <light_mat/config/platform_config.h>


#if (LMAT_CHECK_LEVEL >= 1)
#define LMAT_EXPR_SIZE_CHECKING
#endif

#if (LMAT_CHECK_LEVEL >= 3)
#define LMAT_INDEX_CHECKING
#endif

#endif

