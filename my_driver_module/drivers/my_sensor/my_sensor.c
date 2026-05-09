#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h> // Incluyo este porque me estoy pasando en la API de sensor
#include <zephyr/gpio.h>
#include <zephyr/logging.h>

// Header file de mi driver
#include <my_sensor.h>

// Habilita el uso de las macros tipo DT_INST_ para referernciar nodos con este compatible 
#define DT_DRV_COMPAT my_sensor

LOG_MODULE_REGISTER(my_sensor, LOG_LEVEL_INF);

// 1) Se crea la estructura de configuración estática del driver
struct my_sensor_config {

    // El GPIO asignado para cada dispositivo que trabaje con este driver
    struct gpio_dt_spec m_gpio;
};

// 2) Se crea la estructura de datos del driver. Vendrían a ser los atributos de la clase, que
// son dinámicos, es decir, se almacenan en RAM y pueden leerse/escribirse durante la 
// ejecución de la aplicación
struct my_sensor_data {
    // Estado del sensor (LED en este caso)
    int m_state;
};

// 3) Mapeo de las funciones del driver con las de la API de un sensor. Si bien son varias
// las funciones que están definidas en la API para sensores de Zephyr, solamente se
// asignan 2 que son las que se van a utilizar en este driver.
static DEVICE_API(sensor, my_sensor_api) = {
    // sample_fetch y channel_get pertenecen a la API para sensores que define
    // zephyr. Acá se mapean esas funciones a las que se definen para este driver
    .sample_fetch = my_sensor_sample_fetch;
    .channel_get  = my_sensor_channel_get;
};

// 4) Definiciones de las funciones del driver de my_sensor.

// Función de inicialización: siempre tiene que haber una función de inicialización.
static int my_sensor_init(const struct device *dev) {
    const struct my_sensor_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&(cfg->m_gpio))) {
		return -ENODEV;
	}
    gpio_pin_configure_dt(&(cfg->m_gpio));

    return 0;
}

// El prototipo de sample_fetch es:
// typedef int(* sensor_sample_fetch_t) (const struct device *dev, enum sensor_channel chan)
// Enciende el LED
static int my_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    // dev es un puntero a un device. Cuando se use este driver, el elemento dev->config
    // tendrá la estructura del tipo my_sensor_config definida en este archivo. El puntero
    // cfg apunta a la estructura para la instancia correspondiente del dispositivo que
    // utiliza este driver.
    const struct my_sensor_config *cfg = dev->config;

    // Se enciende el LED
    return gpio_pin_set_dt(&(cfg->m_gpio), 1);
}

// El prototipo de channel_get es:
// typedef int(* sensor_channel_get_t) (const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
// Apaga el LED
static int my_sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct my_sensor_config *cfg = dev->config;

    // Se apaga el LED
    return gpio_pin_set_dt(&(cfg->m_gpio), 0);
}

#define MY_SENSOR_DEFINE(inst)                                      \
static struct my_sensor_data my_sensor_data##inst;               \
static const struct my_sensor_config my_sensor_config##inst = { \
    .m_gpio = GPIO_DT_SPEC_INST_GET(inst, gpio),                        \
};

// inst: instance number
// init_fn = my_sensor_init
// pm = NULL, no se usa
// data = my_sensor_data
// config = my_sensor_config
// initialization level = POST_KERNEL
// prio = CONFIG_SENSOR_INIT_PRIORITY
// api = my_sensor_api
DEVICE_DT_INST_DEFINE(inst,             \
            my_sensor_init,             \
            NULL,                       \
            &my_sensor_init_##inst,     \
            &my_sensor_data_##inst,     \
            &my_sensor_config_##inst,   \
            POST_KERNEL, 				\
            CONFIG_SENSOR_INIT_PRIORITY,\
            &my_sensor_api);

// Esta macro declara una estructura por cada nodo del dts que usa este driver
DT_INST_FOREACH_STATUS_OKAY(MY_SENSOR_DEFINE)