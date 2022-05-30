#include <asf.h>

#define BufferSize 16

static uint8_t Buffer[BufferSize];

int main (void)
{
	system_init_flash(CHIP_FREQ_CPU_MAX);
	
	//Oscillator_Crystal();
	
	
	Oscillator_PLLB();

	//sysclk_init();
	//board_init();
	

	delay_us(2);

	Init_SPI_2();
	
	delay_us(2);
	
	PDC_init(); 
	
	
	for (uint8_t i = 0; i < BufferSize; i++) { //Remplissage buffer
		Buffer[i] = i;
	}
	
		pdc_packet_t pdc_spi_packet1;
		pdc_spi_packet1.ul_addr = (uint32_t)(&Buffer[0]); //Fonction PDC pour envoyer buffer
		pdc_spi_packet1.ul_size = BufferSize;
		pdc_tx_init(PDC_SPI0, &pdc_spi_packet1, NULL);	
 		pdc_enable_transfer(PDC_SPI0, PERIPH_PTCR_TXTEN);
		
		do{

        			//nothing
			
		}while(1);
		
	
}
