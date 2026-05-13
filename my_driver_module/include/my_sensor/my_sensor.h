#ifndef ZEPHYR_DRIVERS_SENSOR_MY_SENSOR_H_
#define ZEPHYR_DRIVERS_SENSOR_MY_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

int my_sensor_set_count(const struct device *dev, int count);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_DRIVERS_SENSOR_MY_SENSOR_H_ */