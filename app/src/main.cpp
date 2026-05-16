#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

// Header file de mi driver
#include <my_sensor/my_sensor.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

static const struct device *led_sensor = DEVICE_DT_GET_ANY(zephyr_my_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// Handler para el sub comando sensor fetch
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    sensor_sample_fetch(led_sensor);
    shell_print(sh,"LED state: ON");
    return 0;
}

// Handler para el sub comando sensor read
static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    
    struct sensor_value dummy;
    
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &dummy);
    shell_print(sh,"LED state: OFF");
    return 0;
}

// Handler para el sub comando sensor info
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    
    bool isReady = device_is_ready(led_sensor);
    shell_print(sh, "Device name: %s, is ready ?: %s", led_sensor->name, isReady ? "YES" : "NO" );
    return 0;
}

int main(void)
{
    if (!device_is_ready(led_sensor)) {
        return 0;
    }

    while (1) {
            // Se eliminó el código del while(1) porque ahora el LED se controla desde los comandos de la shell
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}

// Se define la lista de subcomandos
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Turns LED ON", cmd_sensor_fetch),
    SHELL_CMD(read , NULL, "Turns LED OFF", cmd_sensor_read),
    SHELL_CMD(info , NULL, "Imprime el nombre del dispositivo y el ready state", cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

// Se registra el comando padre
SHELL_CMD_REGISTER(sensor, &sub_sensor, "Comandos para interactuar con devices compatible con el driver my_sensor", NULL);