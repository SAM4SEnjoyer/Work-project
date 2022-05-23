/*
 * spi_functions.c
 *
 * Created: 26/04/2022 14:56:04
 *  Author: Etudiant
 */ 
#include "SPI_Init.h"

/// Pio pins to configure for SPI
//Pin
//USART SPI1
#define SPI1_MISO IOPORT_CREATE_PIN(PIOA, 21)
#define SPI1_MOSI IOPORT_CREATE_PIN(PIOA, 22)
#define SPI1_CLOCK IOPORT_CREATE_PIN(PIOA, 23)
#define SPI1_NPCS0 IOPORT_CREATE_PIN(PIOA, 24)
#define SPI1_NPCS1 IOPORT_CREATE_PIN(PIOA, 25)

//SPI0
#define SPI0_MISO IOPORT_CREATE_PIN(PIOA, 12)
#define SPI0_MOSI IOPORT_CREATE_PIN(PIOA, 13)
#define SPI0_CLOCK IOPORT_CREATE_PIN(PIOA, 14)
#define SPI0_NPCS0 IOPORT_CREATE_PIN(PIOA, 11)
#define SPI0_NPCS3 IOPORT_CREATE_PIN(PIOA, 3)

//Instances SPI
#define SPI0 SPI
#define SPI1 USART1

//baudrate
#define spi_baudrate 96000

//SPI1 Configuration
usart_spi_opt_t USART_SPI_Configuration = {
	.baudrate = spi_baudrate,
	.spi_mode = SPI_MODE_0,
	.channel_mode = US_MR_CHMODE_NORMAL,
	.char_length = US_MR_CHRL_8_BIT
};

struct spi_device SPI_SLAVE0 = {
	
	.id = 0
	
};

struct spi_device SPI_SLAVE3 = {
	
	.id = 3
	
};

struct usart_spi_device USART_SLAVE0 = {
	
	.id = 0
	
};

void init_SPI(void) //I used this function as i was learning, the one i use now is Init_SPI_2();
{
    /*
    * SPI1 pour le DAC et SPI0 pour le ADC
    */
	
    // Configuration des pins d'I/O
      pio_configure_pin(SPI1_MISO, PIO_TYPE_PIO_PERIPH_A); //pin USART mode SPI ; SPI1
      pio_configure_pin(SPI1_MOSI, PIO_TYPE_PIO_PERIPH_A);
      pio_configure_pin(SPI1_CLOCK, PIO_TYPE_PIO_PERIPH_A);
      pio_configure_pin(SPI1_NPCS0, PIO_TYPE_PIO_PERIPH_A);
      pio_configure_pin(SPI1_NPCS1, PIO_TYPE_PIO_PERIPH_A);
	  
	  pio_configure_pin(SPI0_MISO, PIO_TYPE_PIO_PERIPH_A); // pin SPI0
	  pio_configure_pin(SPI0_MOSI, PIO_TYPE_PIO_PERIPH_A);
	  pio_configure_pin(SPI0_CLOCK, PIO_TYPE_PIO_PERIPH_A);
	  pio_configure_pin(SPI0_NPCS0, PIO_TYPE_PIO_PERIPH_A);
	  pio_configure_pin(SPI0_NPCS3, PIO_TYPE_PIO_PERIPH_B);
	
	//Enabling clocks
	pmc_enable_periph_clk(ID_USART1); //SPI1
	pmc_enable_periph_clk(ID_SPI); //SPI0
	
	//SPI1 initialization
	usart_spi_init(SPI1);
	usart_init_spi_master(SPI1, &USART_SPI_Configuration, 500000000);
	
	//SPI0 initialization
	spi_master_init(SPI0);
	
    // Disable all IT on SPI0
	  spi_disable_interrupt(SPI, SPI_IDR_RXBUFF);
	  spi_disable_interrupt(SPI, SPI_IDR_ENDRX);
	  
    // Configuration CR + MR of SPI0 and SPI1

	  SPI0->SPI_MR = (0x10<<24)|SPI_MR_PCS(0x3)|SPI_MR_PS/*|SPI_MR_PS_VARIABLE*/|SPI_MR_MSTR;

	  SPI1->US_MR = SPI_MR_PCS(0x3)|(0x0 << 2)|0/*SPI_MR_PS_FIXED*/|SPI_MR_MSTR;
	  
    // Configuration of chips selected SPI_CSR -> CPOL+NCPHA+CSAAT+BITS +DLYBCT +DLYBS +SCBR
    // 3.0 MHz au max pour le ADC 1 et 2 (5Mhz max en théorie)
	spi_master_setup_device(SPI0, &SPI_SLAVE0, SPI_MODE_2, 500000000, 0);
	spi_master_setup_device(SPI0, &SPI_SLAVE3, SPI_MODE_2, 500000000, spi_get_pcs(3));

	SPI0->SPI_CSR[4] = SPI_CSR_CPOL | SPI_CSR_NCPHA | SPI_CSR_CSAAT | SPI_CSR_BITS_16_BIT;
	
	//spi_set_bits_per_transfer(SPI0, spi_get_pcs(0), SPI_CSR_BITS_8_BIT);
    // 20MHz pour le DAC (30Mhz max)
	
	usart_spi_setup_device(SPI1, &USART_SLAVE0, SPI_MODE_1, 300000000, 0);
	
	// Enable SPI0 and SPI1
    spi_enable(SPI0);
    usart_spi_enable(SPI1);

    // Enable the interrupt on the interrupt controller
	
	NVIC_SetPriority(SPI_IRQn, 1);
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_EnableIRQ(SPI_IRQn);
	
}

void Init_SPI_2(void)
{
	// Configuration des pins d'I/O
	pio_configure_pin(SPI1_MISO, PIO_TYPE_PIO_PERIPH_A); //pin USART mode SPI ; SPI1
	pio_configure_pin(SPI1_MOSI, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI1_CLOCK, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI1_NPCS0, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI1_NPCS1, PIO_TYPE_PIO_PERIPH_A);
	
	pio_configure_pin(SPI0_MISO, PIO_TYPE_PIO_PERIPH_A); // pin SPI0
	pio_configure_pin(SPI0_MOSI, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI0_CLOCK, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI0_NPCS0, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(SPI0_NPCS3, PIO_TYPE_PIO_PERIPH_B);
	//
	
	spi_reset(SPI);
	pmc_enable_periph_clk(ID_SPI);
	
	//MR - Mode Register
	spi_set_master_mode(SPI);
	spi_set_variable_peripheral_select(SPI);
	spi_set_peripheral_chip_select_value(SPI, 0);
	spi_disable_mode_fault_detect(SPI);
	spi_disable_loopback(SPI);
	//
	
	//CSR - Chip Select Register
	spi_set_bits_per_transfer(SPI, 0, SPI_CSR_BITS_16_BIT);
	//int16_t baud_div = spi_calc_baudrate_div(24000000, sysclk_get_peripheral_hz());
	spi_set_baudrate_div(SPI, 0, 1/*baud_div*/);
	spi_configure_cs_behavior(SPI, 0, SPI_CS_KEEP_LOW/*SPI_CSR_CSAAT*/);
	spi_set_clock_polarity(SPI, 0, /*SPI_CSR_CPOL*/SPI_MODE_0 >> 1);
	spi_set_clock_phase(SPI, 0, /*SPI_CSR_NCPHA*/((SPI_MODE_0 & 0x1) ^ 0x1));
	//
	
	//CR - Control Register
	//spi_set_lastxfer(SPI);
	spi_enable(SPI);
	//
	
	/*pmc_enable_periph_clk(USART1);
	
	usart_reset(USART1);
	
	usart_init_spi_master(SPI1, &USART_SPI_Configuration, Clock_Frequency);
	
	usart_spi_init(USART1);*/

}

/*
void SPI_Handler(){
	
	pdc_tx_init(PDC_SPI0, &pdc_spi_packet0, &pdc_spi_packet1);
	
}*/
