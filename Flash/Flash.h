#ifndef _FLASH_H_
#define _FLASH_H_
 
#include "mbed.h"
#include  "ctype.h"
#include "fsl_spi.h"
#include "spi_api.h"
/**
 * Macronix Serial Flash Low Power Memories
 * SPI_MX25R Series SPI-Flash Memory
 */
#define CS_LOW         0    // SPI CS# (Chip Select) Setting 
#define CS_HIGH        1    // SPI CS# (Chip Select) Setting 
#define DUMMY          0x00 // Dummy byte which can be changed to any value
/**
 * MX25R Series Register Command Table. 
 * x2 and x4 commands not currently supported with FRDM K64F platform
 */
#define CMD_READ      0x03  // x1 Normal Read Data Byte 
#define CMD_FREAD     0x0B  // x1 Fast Read Data Byte
#define CMD_2READ     0xBB  // x2 2READ 
#define CMD_DREAD     0x3B  // x2 DREAD 
#define CMD_4READ     0xEB  // x4 4READ 
#define CMD_QREAD     0x6B  // x4 QREAD 
#define CMD_PP        0x02  // Page Program 
#define CMD_4PP       0x38  // x4 PP
#define CMD_SE        0x20  // 4KB Sector Erase 
#define CMD_32KBE     0x52  // 32KB Block Erase 
#define CMD_BE        0xD8  // 64KB Block Erase 
#define CMD_CE        0xC7  // Chip Erase 
#define CMD_RDSFDP    0x5A  // Read SFDP 
#define CMD_WREN      0x06  // Write Enable 
#define CMD_WRDI      0x04  // Write Disable
#define CMD_RDSR      0x05  // Read Status Register 
#define CMD_RDCR      0x15  // Read Configuration Register 
#define CMD_WRSR      0x01  // Write Status Register
#define CMD_PESUS     0xB0  // Program/Erase Suspend 
#define CMD_PERES     0x30  // Program/Erase Resume
#define CMD_DP        0xB9  // Enter Deep Power Down 
#define CMD_SBL       0xC0  // Set Burst Length 
#define CMD_RDID      0x9F  // Read Manufacturer and JDEC Device ID 
#define CMD_REMS      0x90  // Read Electronic Manufacturer and Device ID
#define CMD_RES       0xAB  // Read Electronic ID
#define CMD_ENSO      0xB1  // Enter Secure OTP
#define CMD_EXSO      0xC1  // Exit Secure OTP
#define CMD_RDSCUR    0x2B  // Read Security Register
#define CMD_WRSCUR    0x2F  // Write Security Register
#define CMD_NOP       0x00  // No Operation
#define CMD_RSTEN     0x66  // Reset Enable 
#define CMD_RST       0x99  // Reset 
#define CMD_RRE       0xFF  // Release Read Enhanced Mode

class SPI_MX25R
{
public:
/**
 * Macronix MX25R Low Power and Wide Vcc SPI-Flash Memory Family 
 *
 * @param SI/SIO0 SPI_MOSI pin
 * @param SO/SI01 SPI_MISO pin
 * @param SCLK    SPI_CLK pin
 * @param CSb     SPI_CS  pin
 */
 SPI_MX25R(PinName mosi, PinName miso, PinName sclk, PinName cs) ;
 
 ~SPI_MX25R() ;
 
 SPI m_spi;
 DigitalOut m_cs ;
 int _mode ;
 uint8_t *data1;
 
/// Write Enable
  void writeEnable(void) ;
  
/// Write Disable
  void writeDisable(void) ;
  
/// Reset Enable
  void resetEnable(void) ;
  
/// Reset 
  void reset(void) ;
 
/// Program or Erase Suspend
  void pgmersSuspend(void) ;
 
/// Program or Erase Resume
  void pgmersResume(void) ;
 
/// Enter Deep Power Down
  void deepPowerdown(void) ;
 
/// Set Burst Length 
  void setBurstlength(void) ;
 
/// Release from Read Enhanced Mode 
  void releaseReadenhaced(void) ;
 
/// No Operation 
  void noOperation(void) ;
 
/// Enter OTP Area 
  void enterSecureOTP(void) ;
 
/// Exit OTP Area 
  void exitSecureOTP(void) ;
 
/// Chip Erase
  void chipErase(void) ;
  
/// Write Status and Configuration Reg 1 and 2
  void writeStatusreg(int addr) ;
  
/// Write Security Reg
  void writeSecurityreg(int addr) ;
  
/** Page Program
 *
 * @param int addr start address
 * @param uint8_t *data data buffer
 * @param int numData the number of data to be written
 */
  void programPage(int addr, uint8_t *data, int numData) ;
  
/** Sector Erase
 *
 * @param int addr specify the sector to be erased
 */
  void sectorErase(int addr) ;
  
/** Block Erase
 *
 * @param int addr specify the sector to be erased
 */
  void blockErase(int addr) ;
  
/** 32KB Block Erase
 *
 * @param int addr specify the sector to be erased
 */
  void blockErase32KB(int addr) ;
  
/** Read Status Register
 *
 * @returns uint8_t status register value
 */
  uint8_t readStatus(void) ;
  
/** Read Security Register
 *
 * @returns uint8_t security register value
 */
  uint8_t readSecurity(void) ;

/** Read Manufacturer and JEDEC Device ID
 *
 * @returns uint32_t Manufacturer ID, Mem Type, Device ID
 */
  uint32_t readID(void) ;
  
/** Read Electronic Manufacturer and Device ID
 *
 * @returns uint32_t Manufacturer ID, Device ID
 */
  uint32_t readREMS(void) ;
  
/** Read Electronic ID
 *
 * @returns uint8_t Device ID
 */
  uint8_t readRES(void) ;
  
/** Read Configuration Register
 *
 * @returns uint32_t configuration register value
 */
  uint32_t readConfig(void) ;
  uint8_t readSFDP(int addr) ;        
  uint8_t readFREAD(int addr) ; 
	uint8_t * SPI_MX25R::read2K(int addr,int n) ;
  uint8_t read8(int addr) ;
  void write8(int addr, uint8_t data) ;
  private:
 
} ;
#endif // _SPI_MX25R_H_