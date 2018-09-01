/**
 * @brief used for storing aws iot communication format
 *
 * @file aws_iot_fmt.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */
#ifndef _AWS_IOT_HPP
#define _AWS_IOT_HPP

#define PWR_CTRL_FMT "{%Q: %Q, %Q: %Q, %Q: %Q}"
#define PWR_CTRL_TOTAL_ARG 6

#define TEMP_SENSOR_FMT "{%Q: %Q, %Q: %Q, %Q: {%Q: %.1f, %Q: %Q}}"
#define TEMP_SENSOR_TOTAL_ARG 9

#endif