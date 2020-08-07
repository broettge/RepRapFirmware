//Author: sdavi

//SBC connected to SSP0
#include "Hardware/SharedSpi/SpiMode.h"
#include "HardwareSPI.h"
static HardwareSPI *spiDevice;

volatile bool dataReceived = false, transferReadyHigh = false;
volatile unsigned int spiTxUnderruns = 0, spiRxOverruns = 0;
void InitSpi() noexcept;

// interrupt handler
void SpiInterrupt(HardwareSPI *spi) noexcept
{
    dataReceived = true;
}



void setup_spi(void *inBuffer, const void *outBuffer, size_t bytesToTransfer)
{
    spiDevice->flushRx();
    spiDevice->startTransfer((const uint8_t *)outBuffer, (uint8_t *)inBuffer, bytesToTransfer, SpiInterrupt);
   
    // Begin transfer
    transferReadyHigh = !transferReadyHigh;
    digitalWrite(SbcTfrReadyPin, transferReadyHigh);
}

void disable_spi()
{
    spiDevice->disable();
}
    
// Set up the SPI system
void InitSpi() noexcept
{
    spiDevice = &HardwareSPI::SSP2;
    spiDevice->configureDevice(SPI_MODE_SLAVE, 8, (uint8_t)SPI_MODE_0, 100000000, true);
}
