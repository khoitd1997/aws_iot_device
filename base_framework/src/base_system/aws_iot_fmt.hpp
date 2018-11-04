/**
 * @brief used for storing aws iot communication format
 *
 * This file is used for storing reply format for each aws devices as well as their total
 * arugments, the format contains some modifier that's usually not supported by printf like %Q since
 * they are supposed to be fed into the frozen json_printf, which supports them
 *
 * @file aws_iot_fmt.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _AWS_IOT_HPP
#define _AWS_IOT_HPP

#define PAYLOAD_BEGIN "{payload:"
#define PAYLOAD_END "}"

/**
 * @brief power controller devices reply format
 */
#define PWR_CTRL_FMT "{%Q: %Q, %Q: %Q, %Q: %Q}"
/**
 * @brief power controller total arguments
 */
#define PWR_CTRL_TOTAL_ARG 6

#define SPKR_CTRL_FMT "{%Q: %Q, %Q: %Q, %Q: %d}, {%Q: %Q, %Q: %Q, %Q: %B}"
#define SPKR_CTRL_TOTAL_ARG 12
#define SPKR_ADJUST_VOL_PAYLOAD_FMT PAYLOAD_BEGIN "{volume:%d, volumeDefault:%B}" PAYLOAD_END
#define SPKR_ADJUST_VOL_PAYLOAD_TOTAL_ARG 2
#define SPKR_SET_MUTE_PAYLOAD_FMT PAYLOAD_BEGIN "{mute:%B}" PAYLOAD_END
#define SPKR_SET_MUTE_PAYLOAD_TOTAL_ARG 1
#define SPKR_SET_VOL_PAYLOAD_FMT PAYLOAD_BEGIN "{volume:%d}" PAYLOAD_END
#define SPKR_SET_VOL_PAYLOAD_TOTAL_ARG 1

/**
 * @brief temperature controller devices reply format
 *
 */
#define TEMP_SENSOR_FMT "{%Q: %Q, %Q: %Q, %Q: {%Q: %.1f, %Q: %Q}}"
/**
 * @brief temperature sensor reply total arguments
 */
#define TEMP_SENSOR_TOTAL_ARG 9

#endif