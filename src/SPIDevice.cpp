#include "SPIDevice.h"

/**
 * @brief Construct a new SPIDevice object
 *
 * @param spiBus
 * @param settings
 * @param slaveSelectPin
 */
SPIDevice::SPIDevice(SPIClass* spiBus, SPISettings settings, uint8_t slaveSelectPin)
    : spiBus(spiBus)
    , settings(settings)
    , slaveSelectPin(slaveSelectPin)
{
    // Set the slaveSelectPin as an output ans high:
    pinMode(slaveSelectPin, OUTPUT);
    digitalWrite(slaveSelectPin, HIGH);
}

/**
 * @brief Write one register
 *
 * @param reg
 * @param val
 */
void SPIDevice::write8(uint8_t reg, uint8_t val)
{
    // Begin SPI transaktion
    spiBus->beginTransaction(settings);
    // Take the SS pin low to select the chip
    digitalWrite(slaveSelectPin, LOW);
    // Transfer the register
    spiBus->transfer(reg);
    // Transfer the value
    spiBus->transfer(val);
    // Take the SS pin high to de-select the chip
    digitalWrite(slaveSelectPin, HIGH);
    // End SPI transaktion
    spiBus->endTransaction();
}

/**
 * @brief Write two register
 *
 * @param reg
 * @param val
 */
void SPIDevice::write16(uint8_t reg, uint16_t val)
{
    // Begin SPI transaktion
    spiBus->beginTransaction(settings);
    // Take the SS pin low to select the chip
    digitalWrite(slaveSelectPin, LOW);
    // Transfer the register
    spiBus->transfer(reg);
    // Transfer the value
    spiBus->transfer16(val);
    // Take the SS pin high to de-select the chip
    digitalWrite(slaveSelectPin, HIGH);
    // End SPI transaktion
    spiBus->endTransaction();
}

// Only 32 bit Teensy 4.x supports the function
#if defined(__arm__) && defined(TEENSYDUINO) && (defined(__IMXRT1052__) || defined(__IMXRT1062__))
/**
 * @brief Write four register
 *
 * @param reg
 * @param val
 */
void SPIDevice::write32(uint8_t reg, uint32_t val)
{
    // Begin SPI transaktion
    spiBus->beginTransaction(settings);
    // Take the SS pin low to select the chip
    digitalWrite(slaveSelectPin, LOW);
    // Transfer the register
    spiBus->transfer(reg);
    // Transfer the value
    spiBus->transfer32(val);
    // Take the SS pin high to de-select the chip
    digitalWrite(slaveSelectPin, HIGH);
    // End SPI transaktion
    spiBus->endTransaction();
}
#endif

/**
 * @brief Read one register
 *
 * @param reg
 * @return uint8_t
 */
uint8_t SPIDevice::read8(uint8_t reg)
{
    // Begin SPI transaktion
    spiBus->beginTransaction(settings);
    // Take the SS pin low to select the chip
    digitalWrite(slaveSelectPin, LOW);
    // Transfer the register
    spiBus->transfer(reg | 0x80);
    // Placeholder
    spiBus->transfer(0x00);
    // Receive the value
    uint8_t val = spiBus->transfer(0x00);
    // Take the SS pin high to de-select the chip
    digitalWrite(slaveSelectPin, HIGH);
    // End SPI transaktion
    spiBus->endTransaction();

    return val;
}

/**
 * @brief Read multiple register into buffer
 *
 * @param startReg
 * @param buf
 * @param count
 */
void SPIDevice::readBuffer(uint8_t startReg, uint8_t* buf, size_t count)
{
    // Begin SPI transaktion
    spiBus->beginTransaction(settings);
    // Take the SS pin low to select the chip
    digitalWrite(slaveSelectPin, LOW);
    // Transfer the register
    spiBus->transfer(startReg | 0x80);
    // Placeholder
    spiBus->transfer(0x00);
    // Recieve buffer
    for (size_t i = 0; i < count; i++) {
        buf[i] = spiBus->transfer(0x00);
    }
    // Take the SS pin high to de-select the chip
    digitalWrite(slaveSelectPin, HIGH);
    // End SPI transaktion
    spiBus->endTransaction();
}
