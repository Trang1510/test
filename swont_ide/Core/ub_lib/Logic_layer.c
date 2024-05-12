/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Logic_layer.c
  * @brief          : Logic functions for SoftOnt project
  ******************************************************************************
  * @attention
  *
  * Created on: May 8, 2024
  *    Author: jackt
  * test
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

#define DEBUG

//array for testing
char command[] = "rechthoek,10,10,150,100,licht blauw,0";

char* ParsedData[MAX_VARS];
char* data;

void LL_recieve(void)
{
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

	// Example of checking values
	// Assuming the command structure is known
	if (strcmp(ParsedData[0], "rechthoek") == 0)
	{
		//print test
		printf("banger");
		volatile int x = atoi(ParsedData[1]);
		volatile int y = atoi(ParsedData[2]);
		// Check other values and proceed accordingly
	}
	return;
}
