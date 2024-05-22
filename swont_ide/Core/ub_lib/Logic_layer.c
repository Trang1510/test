/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "Logic_Layer.h"
/******************************************************************************
*   #defines                                                                  *
******************************************************************************/

/******************************************************************************
*   Typedefs                                                                  *
******************************************************************************/

/******************************************************************************
*   Globals vars                                                              *
******************************************************************************/
input_vars input;

/******************************************************************************
*   Local vars                                                                *
******************************************************************************/
//array for testing
char command[] = "rechthoek,10,10,150,100,licht blauw,0";
char* ParsedData[MAX_VARS];

/******************************************************************************
*   Local function prototypes                                                 *
******************************************************************************/

/******************************************************************************
*   Global functions                                                          *
******************************************************************************/

/**
  * @brief  Separates values from incoming array
  * @param  input.line_rx_buffer used
  * @retval None currently
  */
void LL_recieve(void)
{
	char* data;
	// Get the first token
#ifdef DEBUG
	data = strtok(command, ",");
#endif
#ifndef DEBUG
	data = strtok(input.line_rx_buffer, ",");
#endif

	// Walk through other tokens
	int i = 0;
	while (data != NULL && i < MAX_VARS)
	{
		ParsedData[i++] = data;
		data = strtok(NULL, ",");
	}

	//DEBUG for data inside the parsed array
	for (int j = 0; j < i; j++)
	{
		printf("data bit %d: %s\n", j, ParsedData[j]);
	}

	LL_exec(ParsedData[]);
	// Example of checking values
	// Assuming the command structure is known
	if (strcmp(ParsedData[0], "rechthoek") == 0)
	{
		//print test
		printf("banger");
	}
	return;
}
