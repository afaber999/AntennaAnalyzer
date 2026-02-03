
idf_component_register(
    SRCS "main.cpp" "gfx.cpp" "ui.cpp"
    INCLUDE_DIRS "."
    REQUIRES LovyanGFX esp_adc
    PRIV_REQUIRES spi_flash
)

