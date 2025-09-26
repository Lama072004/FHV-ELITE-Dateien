#ifndef _SENSORS_H
#define _SENSORS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Sensor
{
    int     id;
    double  value;
    
} Sensor;

extern Sensor   *sensor_create( int id);
extern int      sensor_id( Sensor *p_sensor);
extern void     sensor_write_to_hardware( Sensor *p_sensor);
extern void     sensor_read_from_hardware( Sensor *p_sensor);
extern double   sensor_value( Sensor *p_sensor);
extern void     sensor_destroy( Sensor **pp_sensor);
extern int      sensor_count();

#endif // _SENSORS_H
