#include <asf.h>

#define BufferSize 16

#define ActivationSize 2

#define VerySmallBufferSize 4

static uint8_t Buffer[BufferSize];

static uint8_t Buffer2[BufferSize];

static uint8_t VerySmallBuffer[VerySmallBufferSize];

static int8_t PlaceToStore[4];

static  unsigned int activationSPCKPourAquisition[4] = {0x000E0000,0x000E0000,0x000D0000,0x000D0000};

uint8_t is_write_success;
uint8_t is_read_success;

uint32_t Measure0, Measure1, Measure2;

int main (void)
{
	
	SystemInit();
	board_init();

	//pmc_osc_disable_fastrc();
	
	//pmc_switch_mainck_to_fastrc(12);
	


	//pmc_switch_mainck_to_xtal(PMC_OSC_XTAL, pmc_us_to_moscxtst(BOARD_OSC_STARTUP_US, OSC_SLCK_32K_RC_HZ));
	
	//EEFC_FMR_CLOE
	
	//enable external crystal		
	PMC->CKGR_MOR = PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCRCEN);
	
	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN
	| CKGR_MOR_MOSCXTST(pmc_us_to_moscxtst(BOARD_OSC_STARTUP_US, OSC_SLCK_32K_RC_HZ));
	
	//wait for crystal to become ready		
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));
	
	//select crystal for main clock
	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD;

	//wait for the MOSCSEL bit to become 1 so that the crystal oscillator is selected
	while (!(pmc_osc_is_ready_mainck()));

	//master clock source selection - choose main clock
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_MAIN_CLK;
	
	//wait until main clock ready
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	
	//select processer prescaler (0 - no divisor)
	PMC->PMC_MCKR |= PMC_MCKR_PRES_CLK_1;
	
	//select processer prescaler (div 4) 12mhz/4 = 3mhz
	//PMC->PMC_MCKR |= PMC_MCKR_PRES_CLK_4;
	//wait until main clock ready
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));


	//pmc_enable_pllbck(9, PLLB_ID, 1);

	pmc_disable_pllbck();
	pll_enable_config_defaults(PLLB_ID);
	//12MHz * (9+1) = 120MHz
	PMC->CKGR_PLLBR |= CKGR_PLLBR_MULB(9);
	PMC->CKGR_PLLBR |= CKGR_PLLBR_DIVB(1);
	//PMC->CKGR_PLLBR |= CKGR_PLLBR_PLLBCOUNT(PLL_COUNT);
	
	while ((PMC->PMC_SR & PMC_SR_LOCKB) == 0);

	//select PLLB as the master clock
	PMC->PMC_MCKR |= PMC_MCKR_CSS_PLLB_CLK;
	
	
	Init_SPI_2();
	
	PDC_init(); 
	
	/*for (uint8_t i = 0; i < BufferSize; i++) {
		Buffer[i] = i;
	}*/
	
	for (uint8_t i = 0; i < VerySmallBufferSize; i++) {
		VerySmallBuffer[i] = i;
	}
	/*iVal = SPI->SPI_SR & SPI_SR_TDRE;*/
	
		pdc_packet_t pdc_spi_packet0;
		pdc_packet_t pdc_spi_packet1;
				
		pdc_spi_packet0.ul_addr = VerySmallBuffer;
		pdc_spi_packet0.ul_size = VerySmallBufferSize;

		pdc_spi_packet1.ul_addr = (uint32_t)(&Buffer[0]);
		pdc_spi_packet1.ul_size = BufferSize;
		
		pdc_tx_init(PDC_SPI0, &pdc_spi_packet1, NULL);
		pdc_tx_init(PDC_SPI1, &pdc_spi_packet0, &pdc_spi_packet1);
				
		pdc_enable_transfer(PDC_SPI0, PERIPH_PTCR_TXTEN);
		//spi_enable_interrupt(SPI, SPI_IER_TDRE);
		pdc_enable_transfer(PDC_SPI1, PERIPH_PTCR_TXTEN);
		//usart_enable_interrupt(USART1, )
		
		do{

        			//nothing
			
		}while(1);
		
		
	
	
}
