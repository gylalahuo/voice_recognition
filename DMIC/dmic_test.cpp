#include "dmic_test.h"
extern "C"
{
void CTIMER3_IRQHandler()
{
	if (CTIMER_GetStatusFlags(CTIMER3) & kCTIMER_Match1Flag) {
	CTIMER_ClearStatusFlags(CTIMER3, kCTIMER_Match1Flag);	
	flag = 1;
	}
}	
}



extern "C"
{
void HWVAD0_IRQHandler(void)
{
	time1=1;
	flag3=1;
	int i;
	if (flag == 0) {
		DMIC_CtrlClrIntrHwvad(DMIC0, 1);
		for(i=0; i<=20; i++);
		DMIC_CtrlClrIntrHwvad(DMIC0, 0);
		return;
	} else {
		DMA_EnableChannelInterrupts(DMA0, DMAREQ_DMIC0);
		DMA_EnableChannel(DMA0, DMAREQ_DMIC0);
		DMIC_FifoClearStatus(DMIC0, 0, 0);
		DMIC_FifoChannel(DMIC0, 0, 15, true, true);
		NVIC_EnableIRQ(DMA0_IRQn);
		if (flag == 1) {
			CTIMER_Deinit(CTIMER3);
			NVIC_ClearPendingIRQ(CTIMER3_IRQn);
			NVIC_DisableIRQ(CTIMER3_IRQn);
			flag = 2;
			DMA_StartTransfer(&g_dmicRxDmaHandle);
		}		
	}
	DMIC_CtrlClrIntrHwvad(DMIC0, 1);
	for(i=0; i<=20; i++);
	DMIC_CtrlClrIntrHwvad(DMIC0, 0);
}
}



extern "C"
{
void DMIC_UserCallback(struct _dma_handle *handle, void *userData, bool transferDone, uint32_t intmode)
{
   
	static uint8_t time2=1;
	time2++;
   	 if (intmode == kDMA_IntB)
    	 {							
	 	flag1=1;						
   	 }
   	 if (intmode == kDMA_IntA)
    	 {			  					
		 flag1=2;			
   	 }

}
}


void dmic_test::DMIC_init1()
{
	 /* Board pin, clock, debug console init */
   	 CLOCK_EnableClock(kCLOCK_InputMux);
  	  /* DMIC uses 12MHz FRO clock */
   	 CLOCK_AttachClk(kFRO12M_to_DMIC);
  	  /*12MHz divided by 15 = 8000KHz PDM clock */
   	 CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 29, false);
	 BOARD_InitPins();
	 /****** enable  ASYNC_APB *******/
	 SYSCON->ASYNCAPBCTRL = 1;
	 CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);
	 /********  close timer   *******/
	ctimer_cfg.mode=kCTIMER_TimerMode;
	ctimer_cfg.prescale=0;
	CTIMER_Init(CTIMER3, &ctimer_cfg);
	CTIMER_Reset(CTIMER3);
	ctimer_match_cfg.enableCounterReset = false;
	ctimer_match_cfg.enableCounterStop = true;
	ctimer_freq = CLOCK_GetAsyncApbClkFreq();
	ctimer_match_cfg.matchValue =  ctimer_freq/400; //trigger 2.5ms
	ctimer_match_cfg.enableInterrupt = true;
	ctimer_match_cfg.outControl = kCTIMER_Output_NoAction;
	ctimer_match_cfg.outPinInitState = 0;
	CTIMER_SetupMatch(CTIMER3, (ctimer_match_t)1, &ctimer_match_cfg);
				
	dmic_channel_cfg.osr=25U;
	dmic_channel_cfg.gainshft=4U;
	dmic_channel_cfg.preac2coef = kDMIC_CompValueZero;
    	dmic_channel_cfg.preac4coef = kDMIC_CompValueZero;
    	dmic_channel_cfg.dc_cut_level = kDMIC_DcCut155;
    	dmic_channel_cfg.post_dc_gain_reduce = 0;
    	dmic_channel_cfg.saturate16bit = 1U;
    	dmic_channel_cfg.sample_rate = kDMIC_PhyFullSpeed;
   	DMIC_Init(DMIC0); 
	   
	DMIC_ConfigIO(DMIC0,kDMIC_PdmDual);
	DMIC_Use2fs(DMIC0, true);
        DMIC_SetOperationMode(DMIC0, kDMIC_OperationModeDma);
   	DMIC_ConfigChannel(DMIC0, APP_DMIC_CHANNEL, kDMIC_Left, &dmic_channel_cfg);
   	DMIC_FifoChannel(DMIC0, APP_DMIC_CHANNEL, FIFO_DEPTH, true, true);
    	DMIC_EnableChannnel(DMIC0, DMIC_CHANEN_EN_CH0(1) );
		
	/****** HWVAD initial ******/
	DMIC_SetGainNoiseEstHwvad(DMIC0, 0x06U);
       /*Gain of the signal estimator */
    	DMIC_SetGainSignalEstHwvad(DMIC0, 0x06U);
    	/* 00 = first filter by-pass, 01 = hpf_shifter=1, 10 = hpf_shifter=4 */
   	 DMIC_SetFilterCtrlHwvad(DMIC0, 0x01U);
    	/*input right-shift of (GAIN x 2 -10) bits (from -10bits (0000) to +14bits (1100)) */
    	DMIC_SetInputGainHwvad(DMIC0, 0x03U);
        DisableDeepSleepIRQ(HWVAD0_IRQn);
	DMA_Init(DMA0);
	/* Request dma channels from DMA manager. */
    	DMA_CreateHandle(&g_dmicRxDmaHandle, DMA0, APP_DMAREQ_CHANNEL);
	DMA_SetCallback(&g_dmicRxDmaHandle,DMIC_UserCallback,NULL);
	dma_xfer_cfg.valid=1;
	dma_xfer_cfg.intA = 1;
	dma_xfer_cfg.intB = 0;
	dma_xfer_cfg.clrtrig = 0;
	dma_xfer_cfg.srcInc = 0;
	dma_xfer_cfg.dstInc = 1;
	dma_xfer_cfg.byteWidth = sizeof(uint16_t);
	dma_xfer_cfg.reload = 1;
	dma_xfer_cfg.swtrig = 1;
	dma_xfer_cfg.transferCount = AUDIO_SAMPLES_STEP_ALGO;
	DMA_PrepareTransfer(&dma_channel_config, (void *)&DMIC0->CHANNEL[kDMIC_Channel0].FIFO_DATA,
						&dmic_buffer[0], sizeof(uint16_t),
						AUDIO_SAMPLES_STEP_ALGO, kDMA_PeripheralToMemory, &dma_descriptor[1]);		
	DMA_SubmitTransfer(&g_dmicRxDmaHandle,&dma_channel_config);	
	DMA_CreateDescriptor(&dma_descriptor[0], &dma_xfer_cfg,
						(void *)&DMIC0->CHANNEL[kDMIC_Channel0].FIFO_DATA,
						&dmic_buffer[0], &dma_descriptor[1]);
	
	dma_xfer_cfg.intA = 0;
  	dma_xfer_cfg.intB = 1;
	DMA_CreateDescriptor(&dma_descriptor[1], &dma_xfer_cfg,
						(void *)&DMIC0->CHANNEL[kDMIC_Channel0].FIFO_DATA,
						&dmic_buffer[AUDIO_SAMPLES_STEP_ALGO], &dma_descriptor[0]);
	NVIC_ClearPendingIRQ(HWVAD0_IRQn);
	NVIC_EnableIRQ(HWVAD0_IRQn);
	CTIMER_StartTimer(CTIMER3);		
}

int8_t dmic_test::DoVoiceTriggerProcess(void)
{
   	if (DMIC0->HWVADST10 == 1) {	
		NVIC_ClearPendingIRQ(HWVAD0_IRQn);
		NVIC_DisableIRQ(DMA0_IRQn);
		DMA_DisableChannelInterrupts(DMA0, DMAREQ_DMIC0);
		DMA_DisableChannel(DMA0, DMAREQ_DMIC0);
		DMIC_FifoClearStatus(DMIC0, 0, 0);
		DMIC_CtrlClrIntrHwvad(DMIC0, 0);
			
	}
	return 0;
}
