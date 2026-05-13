#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// Header file de mi driver
#include <my_sensor/my_sensor.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

static const struct device *led_sensor = DEVICE_DT_GET_ANY(zephyr_my_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    struct sensor_value dummy;
    int count = 0;

    if (!device_is_ready(led_sensor)) {
        return 0;
    }

    while (1) {

        if(led_state) {
            // Apaga el LED
            sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &dummy);
            count++;
            my_sensor_set_count(led_sensor, count);
        }
        else {
            // Enciende el LED
            sensor_sample_fetch(led_sensor);
        }
        led_state = !led_state;        
        LOG_INF("LED state: %s, count: %d", led_state ? "ON" : "OFF", count);
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
