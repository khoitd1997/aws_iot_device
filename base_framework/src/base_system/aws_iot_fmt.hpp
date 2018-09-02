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

/**
 * @brief power controller devices reply format
 */
#define PWR_CTRL_FMT "{%Q: %Q, %Q: %Q, %Q: %Q}"
/**
 * @brief power controller total arguments
 */
#define PWR_CTRL_TOTAL_ARG 6

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