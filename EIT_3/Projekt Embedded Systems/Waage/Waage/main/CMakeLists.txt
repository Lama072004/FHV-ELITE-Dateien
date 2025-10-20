idf_component_register(
    SRCS "hello_world_main.c"
    INCLUDE_DIRS "."      # Header im gleichen Ordner
    REQUIRES esp_adc      # Benötigt ADC-Komponente
)
