#ifndef PDC_FUNCTIONS_H_
#define PDC_FUNCTIONS_H_


#include <asf.h>
Pdc* PDC_SPI0;
Pdc* PDC_SPI1;
void PDC_init(void);
uint8_t spi_pdc_write(Pdc *spi_pdc, void *DataToTransmit, uint32_t Datasize);
uint8_t spi_pdc_read(Pdc *spi_pdc, void *PlaceToStore, uint32_t Datasize);
void spi_master_transfer(void *p_tbuf, uint32_t tsize, void *p_rbuf, uint32_t rsize);

#endif /* PDC_FUNCTIONS_H_ */
