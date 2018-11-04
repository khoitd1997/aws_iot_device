/** @defgroup base Base Framework */

/** @addtogroup base
 *
 * @{
 */

/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mgos.h"

#include "device_config.hpp"
#include "master_handler.hpp"

/**
 * @brief the starting point of the program register the master handler as well as any interrupts
 * define by the user
 *
 * @return enum mgos_app_init
 */
enum mgos_app_init_result mgos_app_init(void) {
  REGISTER_INTERRUPT();
  mgos_mqtt_add_global_handler(handleAllReq, NULL);
  return MGOS_APP_INIT_SUCCESS;
}

/** @} */