// Buttons Panel

// Let the IDE point to the Souliss framework
#include <SoulissFramework.h>

// Configure the framework
#include "bconf/StandardArduino.h"				// Use a standard Arduino
#include "conf/usart.h"							// USART RS485

/*************/
// Use the following if you are using an RS485 transceiver with
// transmission enable pin, otherwise delete this section.
//
#define USARTDRIVER_INSKETCH
#define USART_TXENABLE			1
#define USART_TXENPIN			3
#define USARTDRIVER				Serial
/*************/

// Include framework code and libraries
#include <SPI.h>
#include <Souliss.h>

#include "../../ControllerGateway/include/constants.h"

// Pin assignment
#define RELAY_1_PIN			A0
#define RELAY_2_PIN			A1

void setup()
{
	Initialize();

	// Set network parameters
	SetAddress(RS485_RELAYS_1_ADDR, VNET_SUBNET_MASK, RS485_GATEWAY_ADDR);

	// Define inputs, outputs pins and pulldown
	pinMode(RELAY_1_PIN, OUTPUT);
	pinMode(RELAY_2_PIN, OUTPUT);

	Set_T11(RELAYS_1_RELAY_1_SLOT);
	Set_T11(RELAYS_1_RELAY_2_SLOT);
}

void loop()
{
	// Here we start to play
	EXECUTEFAST() {
		UPDATEFAST();

		FAST_50ms() {	// Process  logic and relevant input and output every 50 milliseconds

			// Execute the logic
			Logic_T11(RELAYS_1_RELAY_1_SLOT);
			Logic_T11(RELAYS_1_RELAY_2_SLOT);

			// Drive the Relays
			DigOut(RELAY_1_PIN, Souliss_T1n_Coil, RELAYS_1_RELAY_1_SLOT);
			DigOut(RELAY_2_PIN, Souliss_T1n_Coil, RELAYS_1_RELAY_2_SLOT);
		}

		// Process the communication, this include the command that are coming from SoulissApp
		// or from the push-button located on the other node
		FAST_PeerComms();

	}

	EXECUTESLOW() {
		UPDATESLOW();
	}

}
