/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      global.h
 * \brief     Library to talk to Tiny-CAN devices
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Library to talk to Tiny-CAN devices.
 * You find the latest versions at http://www.tiny-can.com/
 */

#pragma once

#define MHS_STR_SIZE 255

#ifdef WIN32
#define __WIN32__
#endif

#define DLL_NAME "mhsbmcfg.dll"

#ifdef __WIN32__
// ****** Windows
#include <windows.h>

#define int32_t __int32
#define uint32_t unsigned __int32
#define int16_t __int16
#define uint16_t unsigned __int16

#define CALLBACK_TYPE CALLBACK
#define DRV_LOCK_TYPE CRITICAL_SECTION
#define DRV_LOCK_INIT(x) InitializeCriticalSection((x))
#define DRV_LOCK_DESTROY(x) DeleteCriticalSection((x))
#define DRV_LOCK_ENTER(x) EnterCriticalSection((x))
#define DRV_LOCK_LEAVE(x) LeaveCriticalSection((x))
#define mhs_sleep(x) Sleep(x)

#define get_tick() GetTickCount()

#else
// ****** Linux
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>

#define CALLBACK_TYPE
#define DRV_LOCK_TYPE pthread_mutex_t
#define DRV_LOCK_INIT(x) pthread_mutex_init((x), NULL);
#define DRV_LOCK_DESTROY(x) pthread_mutex_destroy((x))
#define DRV_LOCK_ENTER(x) pthread_mutex_lock((x))
#define DRV_LOCK_LEAVE(x) pthread_mutex_unlock((x))
#define mhs_sleep(x) usleep((x) * 1000)
#endif
