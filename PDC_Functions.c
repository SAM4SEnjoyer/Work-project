#include "PDC_Functions.h"



/**
 * \brief Perform SPI master transfer.
 *
 * \param pbuf Pointer to buffer to transfer.
 * \param size Size of the buffer. 
 */

uint8_t spi_pdc_write(Pdc *spi_pdc, void *DataToTransmit, uint32_t Datasize)
{
		pdc_packet_t pdc_spi_packet;
	
		if (spi_pdc->PERIPH_TCR == 0)
		{

			pdc_spi_packet.ul_addr = DataToTransmit;
			pdc_spi_packet.ul_size = Datasize;
			pdc_tx_init(spi_pdc, &pdc_spi_packet, NULL);
			/*spi_enable_interrupt(SPI, SPI_IER_TXEMPTY);
			iVal = 0;*/
			pdc_enable_transfer(spi_pdc, PERIPH_PTCR_TXTEN);
			
			return 1;
		}
		else if (spi_pdc->PERIPH_TNCR == 0)
		{
			pdc_spi_packet.ul_addr = DataToTransmit;
			pdc_spi_packet.ul_size = Datasize;
			pdc_tx_init(spi_pdc, NULL, &pdc_spi_packet);
			/*spi_enable_interrupt(SPI, SPI_IER_TXEMPTY);
			iVal = 0;*/
			pdc_enable_transfer(spi_pdc, PERIPH_PTCR_TXTEN);
			
			return 1;
		}
	else{
		if(spi_pdc->PERIPH_TCR == 0)
		{
			pdc_disable_transfer(spi_pdc, PERIPH_PTCR_TXTDIS);
		}
		//spi_pdc->PERIPH_PTCR = PERIPH_PTCR_TXTDIS;
		return 0;
	}
}

uint8_t spi_pdc_read(Pdc *spi_pdc, void *PlaceToStore, uint32_t Datasize)
{
	if ((spi_read_status(SPI) & SPI_SR_RDRF) == SPI_SR_RDRF)
	{
		
		( void )SPI->SPI_RDR;
		pdc_packet_t pdc_spi_packet;
		
		pdc_spi_packet.ul_addr = PlaceToStore;
		pdc_spi_packet.ul_size = Datasize;
		pdc_rx_init(PDC_SPI0, &pdc_spi_packet, NULL);
		
		pdc_enable_transfer(spi_pdc, PERIPH_PTCR_RXTEN);
		
		return 1;
	}
	else{
		//pdc_disable_transfer(spi_pdc, PERIPH_PTCR_RXTDIS);
		return 0;
	}
	
}

void PDC_init(void){
	PDC_SPI0 = spi_get_pdc_base(SPI);
	PDC_SPI1 = spi_get_pdc_base(USART1);

}



void spi_master_transfer(void *p_tbuf, uint32_t tsize, void *p_rbuf,
		uint32_t rsize)
{
	pdc_packet_t pdc_spi_packet;

	pdc_spi_packet.ul_addr = (uint32_t)p_rbuf;
	pdc_spi_packet.ul_size = rsize;
	pdc_rx_init(PDC_SPI0, &pdc_spi_packet, NULL);

	pdc_spi_packet.ul_addr = (uint32_t)p_tbuf;
	pdc_spi_packet.ul_size = tsize;
	pdc_tx_init(PDC_SPI0, &pdc_spi_packet, NULL);
	
	//Enable the RX and TX PDC transfer requests */
	pdc_enable_transfer(PDC_SPI0, PERIPH_PTCR_RXTEN | PERIPH_PTCR_TXTEN);


	/* Waiting transfer done */
	while((spi_read_status(SPI) & SPI_SR_RXBUFF) == 0);
	
	/* Disable the RX and TX PDC transfer requests */
	pdc_disable_transfer(PDC_SPI0, PERIPH_PTCR_RXTDIS |	PERIPH_PTCR_TXTDIS);
}



//Maybe it'll be useful later; From the SAM7 spi driver
/*if(PDC.PERIPH_TCR == 0){
		PDC.PERIPH_TPR = DataToTransmit;
		PDC.PERIPH_TCR = Datasize;
		PDC.PERIPH_PTCR = PERIPH_PTCR_RXTEN;
		return 1;
	}
	else if (PDC.PERIPH_TNCR == 0)
	{
		PDC.PERIPH_TNPR = DataToTransmit;
		PDC.PERIPH_TNCR = Datasize;
		return 1;
	}
	return 0;*/
