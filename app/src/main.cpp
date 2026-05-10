#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

static const struct device *led_sensor = DEVICE_DT_GET_ANY(zephyr_my_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    struct sensor_value dummy;

    if (!device_is_ready(led_sensor)) {
        return 0;
    }

    while (1) {

        if(led_state) {
            // Apaga el LED
            sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &dummy);
        }
        else {
            // Enciende el LED
            sensor_sample_fetch(led_sensor);
        }
        led_state = !led_state;        
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
