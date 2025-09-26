#include <stdio.h>
#include <stdlib.h>

#include "sensors.h"

static int Sensorcount = 0;

Sensor *sensor_create( int id)
{
    Sensor *p = malloc( sizeof( Sensor));
    if (!p)
        return NULL;
        
    p->id = id;
    p->value = 0.0;

    Sensorcount++;
    return p;
}


int sensor_id( Sensor *p_sensor) // Liefert die Id des Sensors
{
    //return (*p_sensor).id;
    return p_sensor->id;
}


double sensor_value( Sensor *p_sensor)
{
    return p_sensor->value;
}


void sensor_write_to_hardware( Sensor *p_sensor)
{
    return;}


void sensor_read_from_hardware( Sensor *p_sensor)
{
    p_sensor->value = 42;
}

int sensor_count()
{
    return Sensorcount;
}

void sensor_destroy( Sensor **pp_sensor)
{
    free( *pp_sensor);
    *pp_sensor = NULL;
    Sensorcount--;
    return;
}

