/**
 * $Id: $
 *
 * @brief Red Pitaya library Generate module interface
 *
 * @Author Red Pitaya
 *
 * (c) Red Pitaya  http://www.redpitaya.com
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rp.h"
#include "common.h"
#include "generate.h"

// Base Health address
static const int GENERATE_BASE_ADDR = 0x40200000;
static const int GENERATE_BASE_SIZE = 0x30000;

typedef struct ch_properties {
	unsigned int amplitudeScale		:14;
	unsigned int 					:2;
	unsigned int amplitudeOffset    :14;
	unsigned int 					:2;
	uint32_t counterWrap;
	uint32_t startOffset;
	uint32_t counterStep;
    uint32_t reserved_ReadPointer1  :2;
	uint32_t buffReadPointer        :14;
    uint32_t reserved_ReadPointer2  :16;
	int8_t   unused[10];
} ch_properties_t;

typedef struct generate_control_s {
	unsigned int AtriggerSelector   :4;
	unsigned int ASM_WrapPointer    :1;
	unsigned int ASM_OneTimeTrigger :1;
	unsigned int ASM_reset 		   	:1;
	unsigned int AsetOutputTo0 	   	:1;
	unsigned int 				   	:8;

	unsigned int BtriggerSelector   :4;
	unsigned int BSM_WrapPointer    :1;
	unsigned int BSM_OneTimeTrigger :1;
	unsigned int BSM_reset 		   	:1;
	unsigned int BsetOutputTo0 	   	:1;
	unsigned int 				   	:8;

	ch_properties_t properties_chA;
	ch_properties_t properties_chB;
} generate_control_t;

static volatile generate_control_t *generate = NULL;
static volatile int32_t *data_chA = NULL;
static volatile int32_t *data_chB = NULL;


int generate_Init() {
	ECHECK(cmn_Init());
	ECHECK(cmn_Map(GENERATE_BASE_SIZE, GENERATE_BASE_ADDR, (void **) &generate));
	data_chA = (int32_t *) ((char *) generate + (CHA_DATA_OFFSET));
	data_chB = (int32_t *) ((char *) generate + (CHB_DATA_OFFSET));
	return RP_OK;
}

int generate_Release() {
	ECHECK(cmn_Unmap(GENERATE_BASE_SIZE, (void **) &generate));
	ECHECK(cmn_Release());
	data_chA = NULL;
	data_chB = NULL;
	return RP_OK;
}

int getChannelPropertiesAddress(volatile ch_properties_t **ch_properties, rp_channel_t channel) {
	CHECK_OUTPUT(*ch_properties = &generate->properties_chA,
			     *ch_properties = &generate->properties_chB)
	return RP_OK;
}

int generate_setOutputDisable(rp_channel_t channel, bool disable) {
	if (channel == RP_CH_1) {
		generate->ASM_WrapPointer = 0;
		generate->AsetOutputTo0 = disable ? 1 : 0;
	}
	else if (channel == RP_CH_2) {
		generate->BSM_WrapPointer = 0;
		generate->BsetOutputTo0 = disable ? 1 : 0;
	}
	else {
		return RP_EPN;
	}
	return RP_OK;
}

int generate_setAmplitude(rp_channel_t channel, float amplitude) {
	volatile ch_properties_t *ch_properties;
	ECHECK(getChannelPropertiesAddress(&ch_properties, channel));
	ch_properties->amplitudeScale = cmn_CnvVToCnt(DATA_BIT_LENGTH, amplitude, AMPLITUDE_MAX, 0, 0);
	return RP_OK;
}

int generate_GenTrigger(rp_channel_t channel) {
	if (channel == RP_CH_1) {
		generate->ASM_OneTimeTrigger = 1;
		generate->AtriggerSelector = 1;
		return RP_OK;
	}
	else if (channel == RP_CH_2) {
		generate->BSM_OneTimeTrigger = 1;
		generate->BtriggerSelector = 1;
		return RP_OK;
	}
	else {
		return RP_EPN;
	}
}

int generate_setDCOffset(rp_channel_t channel, float offset) {
	volatile ch_properties_t *ch_properties;
	ECHECK(getChannelPropertiesAddress(&ch_properties, channel));
	ch_properties->amplitudeOffset = cmn_CnvVToCnt(DATA_BIT_LENGTH, offset, (float) (OFFSET_MAX/2), 0, 0);
	return RP_OK;
}

int generate_setFrequency(rp_channel_t channel, float frequency) {
	volatile ch_properties_t *ch_properties;
	ECHECK(getChannelPropertiesAddress(&ch_properties, channel));
	ch_properties->counterStep = (uint32_t) round(65536 * frequency / DAC_FREQUENCY * BUFFER_LENGTH);
	return RP_OK;
}

int generate_setWrapCounter(rp_channel_t channel, uint32_t size) {
	CHECK_OUTPUT(generate->properties_chA.counterWrap = 65536 * (size-1),
			     generate->properties_chB.counterWrap = 65536 * (size-1))
	return RP_OK;
}

int generate_setTriggerSource(rp_channel_t channel, unsigned short value) {
	CHECK_OUTPUT(generate->AtriggerSelector = value,
			     generate->BtriggerSelector = value)
	return RP_OK;
}

int generate_setOneTimeTrigger(rp_channel_t channel, uint32_t value) {
	CHECK_OUTPUT(generate->ASM_OneTimeTrigger = value,
			     generate->BSM_OneTimeTrigger = value)
	return RP_OK;
}

int generate_Synchronise() {
    // Both channels must be reset simultaneously
    ECHECK(cmn_SetBits((uint32_t *) generate, 0x00400040, 0xFFFFFFFF));
    ECHECK(cmn_UnsetBits((uint32_t *) generate, 0x00400040, 0xFFFFFFFF));
    return RP_OK;
}

int generate_writeData(rp_channel_t channel, float *data, uint32_t start, uint32_t length) {
	volatile int32_t *dataOut;
	CHECK_OUTPUT(dataOut = data_chA,
			     dataOut = data_chB)

	volatile ch_properties_t *properties;
	ECHECK(getChannelPropertiesAddress(&properties, channel));
	generate_setWrapCounter(channel, length);

	uint32_t i;
	for(i = start; i < start+BUFFER_LENGTH; i++) {
		dataOut[i % BUFFER_LENGTH] = cmn_CnvVToCnt(DATA_BIT_LENGTH, data[i-start], AMPLITUDE_MAX, 0, 0);
	}
	return RP_OK;
}