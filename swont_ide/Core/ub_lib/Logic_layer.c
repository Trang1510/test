/******************************************************************************
*   Includes                                                                  *
******************************************************************************/
#include "Logic_Layer.h"
#include <string.h>
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
#else
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
		LOGD("data bit %d: %s\n", j, ParsedData[j]);
	}

	LL_exec(ParsedData[0]);
	return;
}

void LL_exec(char* parsedData)
{
    assert_param(parsedData);
}

