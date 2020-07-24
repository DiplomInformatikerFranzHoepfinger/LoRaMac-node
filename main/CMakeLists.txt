# Edit following two lines to set component requirements (see docs)


set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 
	"../src/apps/LoRaMac/periodic-uplink-lpp/heltec_wifi_lora_32_V2/main.c"
	"../src/boards/heltec_wifi_lora_32_V2/board.c"

)
  
set(COMPONENT_ADD_INCLUDEDIRS 
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
  


  
