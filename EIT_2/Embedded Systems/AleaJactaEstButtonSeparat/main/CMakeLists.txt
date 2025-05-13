idf_component_register(
    SRCS 
        "AleaJactaEst_main.c"
        "../button/button.c"   # Pfad zur button.c relativ zur main/CMakeLists.txt
    INCLUDE_DIRS 
        "."                   # Für lokale Header in main/
        "../button"           # Pfad zum button-Ordner für button.h
)