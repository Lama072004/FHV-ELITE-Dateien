/*
                                        AUFGABENSTELLUNG
Realisieren Sie ein Modul `sensors.*`, das es dem User ermöglicht, folgendes Hauptprogramm zu schreiben:

int main()
{
    struct Sensor   *sensorptrs[] = {\
        sensor_create( 0),
        sensor_create( 1),
        sensor_create( 2),
        sensor_create( 3),
        sensor_create( 4),
        NULL
    };
    
    printf( "%d sensors created.", sensor_count());

    for (struct Sensor *p_sensor = sensorptrs; p_sensor; p_sensor++)
    {
        sensor_write_to_hardware( ...);
        sensor_read_from_hardware( ...);
        printf( "Value of sensor %d: %lf.", sensor_id( ...), sensor_value( ...);
    }

    for (struct Sensor *p_sensor = sensorptrs; p_sensor; p_sensor++)
    {
        sensor_destroy( &p_sensor);
        assert( p_sensor == NULL);
    }
    
    return 0;
}

Natürlich sind die `...` durch entsprechende Aktualparameter zu ersetzen.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sensors.h"

int main(int argc, char **argv)
{
    struct Sensor   *sensorptrs[] = {\
        sensor_create( 0),
        sensor_create( 1),
        sensor_create( 2),
        sensor_create( 3),
        sensor_create( 4),
        //NULL // Variante 1
    };
    
    printf( "%d sensors created.", sensor_count());

    //for (int i = 0; sensorptrs[ i]; i++) // Variante 1
    for (int i = 0; i < sizeof( sensorptrs)/sizeof( Sensor *); i++) // Variante 2
    {
        Sensor *p_sensor = sensorptrs[ i];
        assert( p_sensor);
        sensor_write_to_hardware( p_sensor);
        sensor_read_from_hardware( p_sensor);
        printf( "Value of sensor %d: %lf.\n", sensor_id( p_sensor), sensor_value( p_sensor));
    }

    //for (int i = 0; sensorptrs[ i]; i++) // Variante 1
    for (int i = 0; i < sizeof( sensorptrs)/sizeof( Sensor *); i++) // Variante 2
    {
        Sensor *p_sensor = sensorptrs[ i];
        assert( p_sensor);
        sensor_destroy( &p_sensor);
        assert( p_sensor == NULL);
    }
    
	return 0;
}
