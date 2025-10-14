#include <stdbool.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
struct EmsySensor {
    const char* p_id;
    bool is_set_up;

    int (*fp_set_up)(struct EmsySensor* p_this);
    int (*fp_write_to_hardware)(struct EmsySensor* p_this);
    int (*fp_read_from_hardware)(struct EmsySensor* p_this);
};

/////////////////////////////////////////////////////////////////////////////
struct EmsySensorUss {
    struct EmsySensor base;
};

int uss_set_up(struct EmsySensor* p_this) {
    printf("Setting up USS sensor (%s)\n", p_this->p_id);
    p_this->is_set_up = true;
    return 0;
}

int uss_write_to_hardware(struct EmsySensor* p_this) {
    printf("Writing to USS sensor (%s)\n", p_this->p_id);
    return 0;
}

int uss_read_to_hardware(struct EmsySensor* p_this) {
    printf("Reading from USS sensor (%s)\n", p_this->p_id);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
struct EmsySensorIrs {
    struct EmsySensor base;
};

int irs_set_up(struct EmsySensor* p_this) {
    printf("Setting up IRS sensor (%s)\n", p_this->p_id);
    p_this->is_set_up = true;
    return 0;
}

int irs_write_to_hardware(struct EmsySensor* p_this) {
    printf("Writing to IRS sensor (%s)\n", p_this->p_id);
    return 0;
}

int irs_read_to_hardware(struct EmsySensor* p_this) {
    printf("Reading from IRS sensor (%s)\n", p_this->p_id);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define SENSOR(ID, SETUP, WRITE, READ)         \
    {                                          \
        .base = {                              \
            .p_id = ID,                        \
            .is_set_up = false,                \
            .fp_set_up = SETUP,                \
            .fp_write_to_hardware = WRITE,     \
            .fp_read_from_hardware = READ      \
        }                                      \
    }

#define SENSOR_SET_UP(sensor) \
    (sensor).base.fp_set_up(&(sensor).base)

#define SENSOR_WRITE_TO_HARDWARE(sensor) \
    (sensor).base.fp_write_to_hardware(&(sensor).base)

#define SENSOR_READ_TO_HARDWARE(sensor) \
    (sensor).base.fp_read_from_hardware(&(sensor).base)


int main(void)
{
    struct EmsySensorUss sensors[] =
    {
        SENSOR("USS_1", uss_set_up, uss_write_to_hardware, uss_read_to_hardware),
        SENSOR("IRS_1", irs_set_up, irs_write_to_hardware, irs_read_to_hardware),
    };

    int num_sensors = sizeof(sensors) / sizeof(sensors[0]);

    printf("=== Sensor Setup ===\n");
    for (int i = 0; i < num_sensors; i++)
        SENSOR_SET_UP(sensors[i]);

    printf("\n=== Sensor Operations ===\n");
    for (int i = 0; i < num_sensors; i++) {
        SENSOR_WRITE_TO_HARDWARE(sensors[i]);
        SENSOR_READ_TO_HARDWARE(sensors[i]);
        printf("\n");
    }

    printf("Press ENTER to exit...");
    getchar();
    return 0;
}
