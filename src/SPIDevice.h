#pragma once

#include <SPI.h>

class SPIDevice {
private:
    SPIClass* spiBus;
    SPISettings settings;
    uint8_t slaveSelectPin;

protected:
    SPIDevice(SPIClass* spi, SPISettings settings, uint8_t slaveSelectPin);
    virtual ~SPIDevice() = default;

    void write8(uint8_t reg, uint8_t val);
    void write16(uint8_t reg, uint16_t val);
#if defined(__arm__) && defined(TEENSYDUINO) && (defined(__IMXRT1052__) || defined(__IMXRT1062__))
    void write32(uint8_t reg, uint32_t val);
#endif

    uint8_t read8(uint8_t reg);
    void readBuffer(uint8_t startReg, uint8_t* buf, size_t count);
};
