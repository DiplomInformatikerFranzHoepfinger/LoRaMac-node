# Edit following two lines to set component requirements (see docs)


set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS 

	#---------------------------------------------------------------------------------------
	# Target
	#---------------------------------------------------------------------------------------
	"../src/apps/LoRaMac/periodic-uplink-lpp/heltec_wifi_lora_32_V2/main.c"
	"../src/boards/heltec_wifi_lora_32_V2/board.c"
	"../src/boards/heltec_wifi_lora_32_V2/spi-board.c"
	"../src/boards/heltec_wifi_lora_32_V2/rtc-board.c"
	"../src/boards/heltec_wifi_lora_32_V2/gpio-board.c"
	"../src/boards/heltec_wifi_lora_32_V2/sx1276-board.c"
	"../src/boards/heltec_wifi_lora_32_V2/delay-board.c"
	"../src/boards/mcu/utilities.c"
	#---------------------------------------------------------------------------------------
	# Target
	#---------------------------------------------------------------------------------------
	"../src/system/timer.c"
	"../src/system/systime.c"
	"../src/system/delay.c"
	"../src/system/gpio.c"
	
	#---------------------------------------------------------------------------------------
	# SE
	#---------------------------------------------------------------------------------------
	"../src/peripherals/soft-se/aes.c"
	"../src/peripherals/soft-se/cmac.c"
	"../src/peripherals/soft-se/soft-se.c"
	"../src/peripherals/soft-se/soft-se-hal.c"
    #---------------------------------------------------------------------------------------
    # Application common features handling
    #---------------------------------------------------------------------------------------
    "../src/apps/LoRaMac/common/CayenneLpp.c"
    "../src/apps/LoRaMac/common/LmHandlerMsgDisplay.c"
    "../src/apps/LoRaMac/common/NvmCtxMgmt.c"
	
	#---------------------------------------------------------------------------------------
    # Application LoRaMac handler
    #---------------------------------------------------------------------------------------
	"../src/apps/LoRaMac/common/LmHandler/LmHandler.c"
	
    #---------------------------------------------------------------------------------------
    # LoRaMac handler applicative packages
    #---------------------------------------------------------------------------------------
    "../src/apps/LoRaMac/common/LmHandler/packages/FragDecoder.c"
    "../src/apps/LoRaMac/common/LmHandler/packages/LmhpClockSync.c"
    "../src/apps/LoRaMac/common/LmHandler/packages/LmhpCompliance.c"
    "../src/apps/LoRaMac/common/LmHandler/packages/LmhpFragmentation.c"
    "../src/apps/LoRaMac/common/LmHandler/packages/LmhpRemoteMcastSetup.c"
    
    #---------------------------------------------------------------------------------------
	# Target
	#---------------------------------------------------------------------------------------
	"../src/mac/LoRaMac.c"
	"../src/mac/LoRaMacAdr.c"
	"../src/mac/LoRaMacClassB.c"
	"../src/mac/LoRaMacCommands.c"
	"../src/mac/LoRaMacConfirmQueue.c"
	"../src/mac/LoRaMacCrypto.c"
	"../src/mac/LoRaMacParser.c"
	"../src/mac/LoRaMacSerializer.c"
	"../src/mac/region/Region.c"
	"../src/mac/region/RegionAS923.c"
	"../src/mac/region/RegionAU915.c"
	"../src/mac/region/RegionBaseUS.c"
	"../src/mac/region/RegionCN470.c"
	"../src/mac/region/RegionCN470A20.c"
	"../src/mac/region/RegionCN470A26.c"
	"../src/mac/region/RegionCN470B20.c"
	"../src/mac/region/RegionCN470B26.c"
	"../src/mac/region/RegionCN779.c"
	"../src/mac/region/RegionCommon.c"
	"../src/mac/region/RegionEU433.c"
	"../src/mac/region/RegionEU868.c"
	"../src/mac/region/RegionIN865.c"
	"../src/mac/region/RegionKR920.c"
	"../src/mac/region/RegionRU864.c"
	"../src/mac/region/RegionUS915.c"
	
    #---------------------------------------------------------------------------------------
	# sx1276 Radio
	#---------------------------------------------------------------------------------------	
	"../src/radio/sx1276/sx1276.c"

)
  
set(COMPONENT_ADD_INCLUDEDIRS 
	"../src/boards/heltec_wifi_lora_32_V2"
	"../src/mac"
	"../src/mac/region"
	"../src/boards"
	"../src/system"
	"../src/radio"
	"../src/apps/LoRaMac/common"
	"../src/apps/LoRaMac/common/LmHandler"
	"../src/apps/LoRaMac/common/LmHandler/packages"
	"../src/peripherals/soft-se"
)


  
register_component()
  


  
