#include <stdio.h>
#include "CanIf.h"
#include "main.h"
#include "Platform_Types.h"

uint8 arr[] = {'a', 'h', 'm', 'e', 'd', '\n'};

const PduInfoType PduInfoPtr = {
		.SduDataPtr = arr,
		.SduLength = 6,
		.MetaDataPtr = NULL
};

int main(void)
{
	CanIf_Init(&CanIf_Config);
	CanIf_SetControllerMode(CANIF_CHL_0, CAN_CS_STARTED);
	CanIf_Transmit(15, &PduInfoPtr);
	printf("Hello world!");

	while(1)
	{

	}

    return 0;
}

void address15TxCanIf(PduIdType myID)
{
	printf("myID: %d\n", myID);
}

void address20TxCanIf(PduIdType myID)
{
	printf("myID: %d\n", myID);
}
