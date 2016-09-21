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

#include "../../ControllerGateway/include/types.h"
#include "../../ControllerGateway/include/constants.h"

// Pin assignment
#define RELAY_1_PIN			10
#define RELAY_2_PIN			11

void setup()
{
	Initialize();

	// Set network parameters
	SetAddress(RS485_RELAYS_1_ADDR, VNET_SUBNET_MASK, RS485_GATEWAY_ADDR);

	// Define inputs, outputs pins and pulldown
	pinMode(RELAY_1_PIN, OUTPUT);
	pinMode(RELAY_2_PIN, OUTPUT);

	Set_T11(RELAYS_MODULE_RELAY_1_SLOT);
	Set_T11(RELAYS_MODULE_RELAY_2_SLOT);

	pinMode(13, OUTPUT); // Debug

}

void loop()
{
	// Here we start to play
	EXECUTEFAST() {
		UPDATEFAST();

		FAST_50ms() {	// Process  logic and relevant input and output every 50 milliseconds

			// Test communications
			button_address button;
			uint8_t len;
			if (subscribedata(BUTTON_PUSHED_EVENT, (uint8_t*)&button, &len)) {
				if (button.button_panel_addr == RS485_BUTTONS_PANEL_1_ADDR && button.button == 1) {
					digitalWrite(13, LOW);
				}
				if (button.button_panel_addr == RS485_BUTTONS_PANEL_1_ADDR && button.button == 2) {
					digitalWrite(13, HIGH);
				}
			}

			// Execute the logic
			Logic_T11(RELAYS_MODULE_RELAY_1_SLOT);
			Logic_T11(RELAYS_MODULE_RELAY_2_SLOT);

			// Drive the Relays
			DigOut(RELAY_1_PIN, Souliss_T1n_Coil, RELAYS_MODULE_RELAY_1_SLOT);
			DigOut(RELAY_2_PIN, Souliss_T1n_Coil, RELAYS_MODULE_RELAY_2_SLOT);

			DigOut(13, Souliss_T1n_Coil, RELAYS_MODULE_RELAY_1_SLOT);
		}

		// Process the communication, this include the command that are coming from SoulissApp
		// or from the push-button located on the other node
		FAST_PeerComms();

	}

	EXECUTESLOW() {
		UPDATESLOW();
	}

}
