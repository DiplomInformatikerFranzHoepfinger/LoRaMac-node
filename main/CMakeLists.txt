# Edit following two lines to set component requirements (see docs)





#---------------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------------

# Allow switching of active region
set(ACTIVE_REGION_LIST   LORAMAC_REGION_EU868 LORAMAC_REGION_US915 LORAMAC_REGION_CN779
    LORAMAC_REGION_EU433 LORAMAC_REGION_AU915 LORAMAC_REGION_AS923 LORAMAC_REGION_CN470
    LORAMAC_REGION_KR920 LORAMAC_REGION_IN865 LORAMAC_REGION_RU864
)
set(ACTIVE_REGION LORAMAC_REGION_EU868 CACHE STRING "Default active region is EU868")
set_property(CACHE ACTIVE_REGION PROPERTY STRINGS ${ACTIVE_REGION_LIST})




set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 
	"../src/apps/LoRaMac/periodic-uplink-lpp/heltec_wifi_lora_32_V2/main.c"
	"../src/boards/heltec_wifi_lora_32_V2/board.c"
	"../src/boards/heltec_wifi_lora_32_V2/spi-board.c"
	"../src/boards/mcu/utilities.c"
	"../src/system/timer.c"
	"../src/apps/LoRaMac/common/LmHandlerMsgDisplay.c"
	"../src/apps/LoRaMac/common/LmHandler/LmHandler.c"

)
  
set(COMPONENT_ADD_INCLUDEDIRS 
	"../src/boards/heltec_wifi_lora_32_V2"
	"../src/mac"
	"../src/boards"
	"../src/system"
	"../src/radio"
	"../src/apps/LoRaMac/common"
	"../src/apps/LoRaMac/common/LmHandler"
	"../src/apps/LoRaMac/common/LmHandler/packages"
	"../src/peripherals/soft-se"
)


  
register_component()
  


  
