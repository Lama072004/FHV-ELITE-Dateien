idf_component_register(SRCS "main.c"
                       "sensor_init.c"
                       "pulse.c"
                       "spo2.c"
                      
                       "heart_condition.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES driver)
