/*
 * SPI_MX25R Series SPI-Flash Memory
 * Macronix Low Power Serial NOR Flash
 * (x2, and x4 I/O modes not implemented)
 */
 
#include "Flash.h"

 
SPI_MX25R::SPI_MX25R(PinName mosi, PinName miso, PinName sclk, PinName cs) :
        m_spi(mosi, miso, sclk), m_cs(cs) {

      m_spi.frequency(48*1000*1000);
				}
 
SPI_MX25R::~SPI_MX25R() { }
 
void SPI_MX25R::writeEnable(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_WREN) ;
    m_cs = CS_HIGH ;
}
 
void SPI_MX25R::writeDisable(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_WRDI) ;
    m_cs = CS_HIGH ;
}    
 
void SPI_MX25R::resetEnable(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_RSTEN) ;
    m_cs = CS_HIGH ;
}  
 
void SPI_MX25R::reset(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_RST) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::pgmersSuspend(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_PESUS) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::pgmersResume(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_PERES) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::deepPowerdown(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_DP) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::setBurstlength(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_SBL) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::releaseReadenhaced(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_RRE) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::noOperation(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_NOP) ;
    m_cs = CS_HIGH ;
} 
 
void SPI_MX25R::enterSecureOTP(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_ENSO) ;
    m_cs = CS_HIGH ;
}
 
void SPI_MX25R::exitSecureOTP(void)
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_EXSO) ;
    m_cs = CS_HIGH ;
} 
 
uint8_t SPI_MX25R::readStatus(void)
{   
    uint8_t data ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_RDSR) ;
    data = m_spi.write(DUMMY) ;                     // dummy
    m_cs = CS_HIGH ;
    return( data ) ;
}
  
uint32_t SPI_MX25R::readConfig(void)
{   
    uint8_t data;
    uint32_t config32 = 0 ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_RDCR) ;                         // send 15h
    data= m_spi.write(DUMMY)  ;                     // dumy to get 1st Byte out
    config32 = config32 | data ;                    // put in 32b reg
    data= m_spi.write(DUMMY) ;                      // dummy to get 2nd Byte out
    config32 = (config32 << 8) | data ;             // shift and put in reg
    m_cs = CS_HIGH ;
    return( config32 ) ;  
}

uint8_t SPI_MX25R::readSecurity(void)
{   
    uint8_t data ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_RDSCUR) ;                       // send 2Bh
    data = m_spi.write(DUMMY) ;                     // dummy
    m_cs = CS_HIGH ;
    return( data ) ;
}
  
uint32_t SPI_MX25R::readID(void)
{   
    uint8_t data;
    uint32_t data32 = 0 ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_RDID) ;                         // send 9Fh
    data= m_spi.write(DUMMY)  ;                     // dumy to get 1st Byte out
    data32 = data32 | data ;                        // put in 32b reg
    data= m_spi.write(DUMMY) ;                      // dummy to get 2nd Byte out
    data32 = (data32 << 8) | data ;                 // shift and put in reg
    data= m_spi.write(DUMMY)  ;                     // dummy to get 3rd Byte out
    data32 = (data32 << 8) | data ;                 // shift again and put in reg
    m_cs = CS_HIGH ;
    return( data32 ) ;  
}

uint32_t SPI_MX25R::readREMS(void)
{   
    uint8_t data;
    uint32_t data32 = 0 ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_REMS) ;                         // send 90h
    m_spi.write(DUMMY) ;                            // send DUMMY1
    m_spi.write(DUMMY) ;                            // send DUMMY2
    m_spi.write(0) ;                                // send address=0x00 to get Manu ID 1st.
    data= m_spi.write(DUMMY)  ;                     // dumy to get Manufacturer ID= C2h out
    data32 = data32 | data ;                        // put in 32b reg
    data= m_spi.write(DUMMY) ;                      // dummy to get 2nd Byte = Device ID out
    data32 = (data32 << 8) | data ;                 // shift and put in reg
    m_cs = CS_HIGH ;
    return( data32 ) ;  
}

uint8_t SPI_MX25R::readRES(void)
{   
    uint8_t data;
    m_cs = CS_LOW ;
    m_spi.write(CMD_RES) ;                          // send ABh
    m_spi.write(DUMMY) ;                            // send DUMMY1
    m_spi.write(DUMMY) ;                            // send DUMMY2
    m_spi.write(DUMMY) ;                            // send DUMMY3
    data= m_spi.write(DUMMY)  ;                     // dumy to get Electronic Sig. out
    m_cs = CS_HIGH ;
    return( data ) ;  
}
 
void SPI_MX25R::programPage(int addr, uint8_t *data, int numData)
{
    int i ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_PP) ;                           // Program Page 02h
    m_spi.write((addr >> 16)&0xFF) ;                // adr 23:16
    m_spi.write((addr >>  8)&0xFF) ;                // adr 15:8
    m_spi.write(addr & 0xFF) ;                      // adr 7:0
    for (i = 0 ; i < numData ; i++ ) {              // data = 00, 01, 02, .. to FEh, FFh = all 256 Bytes in 1 page. 
        m_spi.write(data[i]) ;
    }
    m_cs = CS_HIGH ;
    // poll in main
}
 
void SPI_MX25R::writeStatusreg(int addr)            // Write SR cmd 01h + 3B data
{   
    m_cs = CS_LOW ;
    m_spi.write(CMD_WRSR) ;                         // Write SR cmd 01h
    m_spi.write((addr >> 16)&0xFF) ;                // address
    m_spi.write((addr >>  8)&0xFF) ;
    m_spi.write(addr & 0xFF) ;
    m_cs = CS_HIGH ;
}

void SPI_MX25R::writeSecurityreg(int addr)          // WRSCUR cmd 2Fh + 1B data
{   
    m_cs = CS_LOW ;
    m_spi.write(CMD_WRSCUR) ;                         // Write SR cmd 01h
    m_spi.write(addr & 0xFF) ;
    m_cs = CS_HIGH ;
}

void SPI_MX25R::blockErase(int addr)                // 64KB Block Erase
{
    uint8_t data[3] ;
    data[0] = (addr >> 16) & 0xFF ;
    data[1] = (addr >> 8) & 0xFF ;
    data[2] = (addr & 0xFF) ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_BE) ;
    for (int i = 0 ; i < 3 ; i++ ) {                // Address setting
        m_spi.write(data[i]) ;
    }
    m_cs = CS_HIGH ;
    // poll in main
}
 
void SPI_MX25R::blockErase32KB(int addr)            // 32KB Block Erase
{
    uint8_t data[3] ;
    data[0] = (addr >> 16) & 0xFF ;
    data[1] = (addr >> 8) & 0xFF ;
    data[2] = (addr & 0xFF) ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_32KBE) ;
    for (int i = 0 ; i < 3 ; i++ ) {                // Address Setting
        m_spi.write(data[i]) ;
    }
    m_cs = CS_HIGH ;
    // poll in main
}
 
void SPI_MX25R::sectorErase(int addr)               //  4KB Sector Erase
{
    uint8_t data[3] ;
    data[0] = (addr >> 16) & 0xFF ;
    data[1] = (addr >> 8) & 0xFF ;
    data[2] = (addr & 0xFF) ;
    m_cs = CS_LOW ;
    m_spi.write(CMD_SE) ;
    for (int i = 0 ; i < 3 ; i++ ) {                // Address Setting
        m_spi.write(data[i]) ;
    }
    m_cs = CS_HIGH ;
    // poll in main
}
 
void SPI_MX25R::chipErase(void)                     // Chip Erase
{
    m_cs = CS_LOW ;
    m_spi.write(CMD_CE) ;
    m_cs = CS_HIGH ;
    // poll in main
}

uint8_t * SPI_MX25R::read2K(int addr,int n)                  // Single Byte Read
{  
	  data1=new uint8_t[16385] ; 
	  uint32_t rx_data;
    m_cs = CS_LOW ;
    m_spi.write(CMD_FREAD) ;                         // send 03h
    m_spi.write((addr >> 16)&0xFF) ;
    m_spi.write((addr >>  8)&0xFF) ;
    m_spi.write(addr & 0xFF); 
	  for(int i=0;i<16385;i++)
	  {
			SPI_WriteData(SPI5, (uint16_t)DUMMY, kSPI_FrameAssert);
			while(!(SPI_GetStatusFlags(SPI5) & kSPI_RxNotEmptyFlag));
			rx_data = SPI_ReadData(SPI5);
			data1[i]=rx_data & 0xffff;
	  }
    m_cs = CS_HIGH ;
    return( data1+1 ) ;  
	
}
 
uint8_t SPI_MX25R::read8(int addr)                  // Single Byte Read
{
    uint8_t data ;    
    m_cs = CS_LOW ;
    m_spi.write(CMD_READ) ;                         // send 03h
    m_spi.write((addr >> 16)&0xFF) ;
    m_spi.write((addr >>  8)&0xFF) ;
    m_spi.write(addr & 0xFF) ;
    data = m_spi.write(DUMMY) ;                     // write data is dummy 
    m_cs = CS_HIGH ;
    return( data ) ;                                // return 1 byte 
}
 
uint8_t SPI_MX25R::readSFDP(int addr)               // Read SFDP
{
    uint8_t data ;    
    m_cs = CS_LOW ;
    m_spi.write(CMD_RDSFDP) ;                       // send cmd 5Ah
    m_spi.write((addr >> 16)&0xFF) ;                // address[23:16]
    m_spi.write((addr >>  8)&0xFF) ;                // address[15:8]
    m_spi.write(addr & 0xFF) ;                      // address[7:0]
    m_spi.write(DUMMY) ;                            // dummy cycle
    data = m_spi.write(DUMMY) ;                     // return 1 byte 
    m_cs = CS_HIGH ;
    return( data ) ;
}
 
uint8_t SPI_MX25R::readFREAD(int addr)              // x1 Fast Read Data Byte
{
    uint8_t data ;    
    m_cs = CS_LOW ;
    m_spi.write(CMD_FREAD) ;                        // send cmd 0BH
    m_spi.write((addr >> 16)&0xFF) ;                // address[23:16]
    m_spi.write((addr >>  8)&0xFF) ;                // address[15:8]
    m_spi.write(addr & 0xFF) ;                      // address[7:0]
    m_spi.write(DUMMY) ;                            // dummy cycle
    data = m_spi.write(DUMMY) ;                     // return 1 byte 
    m_cs = CS_HIGH ;
    return( data ) ;
}




