#ifndef _DMIC_TEST_H
#define _DMIC_TEST_H
#include "mbed.h"
#include  "fsl_usart.h"
#include "fsl_dmic.h"
#include "fsl_dma.h"
#include "fsl_ctimer.h"
#include "fsl_clock.h"
//#include "clock_config.h"
#include "stdlib.h"
#include "pin_mux.h"
#include  "Flash.h"
#define APP_DMAREQ_CHANNEL DMAREQ_DMIC0
#define APP_DMIC_CHANNEL kDMIC_Channel0
#define APP_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH0(1)
#define DMAREQ_DMIC0 16U
#define FIFO_DEPTH  15U
#define BUFFER_LENGTH 32U
#define AUDIO_SAMPLES_STEP_ALGO 1024
extern TCPSocket tcpsocket;
extern void rdRange(unsigned long rd_start_address, unsigned long rd_end_address);
extern uint16_t dmic_buffer[AUDIO_SAMPLES_STEP_ALGO*2];
extern int flag;
extern int flag1;
extern int flag2;
extern int flag3;
extern int time1; 

	__align(16) static dma_descriptor_t dma_descriptor[2];
static dma_handle_t g_dmicRxDmaHandle;
class dmic_test
{
public:
			dmic_channel_config_t dmic_channel_cfg;
			dma_transfer_config_t dma_channel_config;
			dma_xfercfg_t dma_xfer_cfg;
			ctimer_config_t ctimer_cfg;
			ctimer_match_config_t ctimer_match_cfg;
			uint32_t ctimer_freq;
public:	
		void DMIC_init1();
	
		int8_t DoVoiceTriggerProcess(void);
		void DMIC_get_data();

};
#endif