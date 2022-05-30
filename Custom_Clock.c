#include "Custom_Clock.h"

void Oscillator_RC(void){
	
	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_MOSCRCEN | CKGR_MOR_KEY_PASSWD; //bit MOSCRCEN à 1 pour activer l'oscillateur RC
	
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD; //bit MOSCSEL à 0 pour sélectionner l'oscillateur RC
	
	PMC->CKGR_MOR = PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTEN); //On désactive l'oscillateur à cristal
	
	PMC->PMC_MCKR |= PMC_MCKR_CSS_MAIN_CLK; //On sélectionne la clock principale en clock maitre
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
	PMC->PMC_MCKR |= PMC_MCKR_PRES_CLK_1; //On met le prescaler à 1 (division de la clock par X)
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
}

void Oscillator_Crystal(void){
	
	PMC->CKGR_MOR = PMC->CKGR_MOR |  CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD; //bit MOSCXTEN à 1 pour activer l'oscillateur à cristal
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)); //On attend que l'oscillateur à cristal soit prêt
	
	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD; //bit MOSCSEL à 1 pour sélectionner l'oscillateur à cristal
	
	PMC->CKGR_MOR = PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCRCEN); //On désactive le Bypass et l'oscillateur RC

	PMC->PMC_MCKR |= PMC_MCKR_CSS_MAIN_CLK; //On sélectionne la clock principale en clock maitre
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
	PMC->PMC_MCKR |= PMC_MCKR_PRES_CLK_1; //On met le prescaler à 1 (division de la clock par X)
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
}

void Oscillator_PLLA(void){
	
	PMC->CKGR_MOR = PMC->CKGR_MOR |  CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD; //bit MOSCXTEN à 1 pour activer l'oscillateur à cristal (Le plus adapté pour utiliser le PLL)
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)); //On attend que l'oscillateur à cristal soit prêt
	
	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD; //bit MOSCSEL à 1 pour sélectionner l'oscillateur à cristal
	PMC->CKGR_MOR = PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCRCEN); //On désactive le Bypass et l'oscillateur RC
	


	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0); //On désactive les deux PLL (Ici PLLA)
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0); //désactive PLLB
	
	PMC->CKGR_PLLAR = PMC->CKGR_PLLAR | CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(2/*+1 une fois dans le registre*/) | CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_PLLACOUNT(PLL_COUNT); //On rentre notre configuration de division et de multiplication
	while (!(PMC->PMC_SR & PMC_SR_LOCKA)); //On attend que le PLL soit prêt
	
	
	
	PMC->PMC_MCKR = PMC->PMC_MCKR & ~PMC_MCKR_PLLADIV2 & ~PMC_MCKR_PLLBDIV2; //On désactive la division par 2
	
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | PMC_MCKR_PRES_CLK_1; //On met le prescaler à 1 (division de la clock par X) (Pour mettre le PLL en clock maitre il faut d'abord initialiser le prescaler)
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLA_CLK; //On sélectionne la clock PLLA en clock maitre
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
}

void Oscillator_PLLB(void){
	
	PMC->CKGR_MOR = PMC->CKGR_MOR |  CKGR_MOR_MOSCXTEN | CKGR_MOR_KEY_PASSWD; //bit MOSCXTEN à 1 pour activer l'oscillateur à cristal (Le plus adapté pour utiliser le PLL)
	while (!(PMC->PMC_SR & PMC_SR_MOSCXTS)); //On attend que l'oscillateur à cristal soit prêt

	PMC->CKGR_MOR = PMC->CKGR_MOR | CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_PASSWD; //bit MOSCSEL à 1 pour sélectionner l'oscillateur à cristal
	PMC->CKGR_MOR = PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTBY | CKGR_MOR_MOSCRCEN); //On désactive le Bypass et l'oscillateur RC
	
	

	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0); //On désactive les deux PLL (Ici PLLA)
	PMC->CKGR_PLLBR = CKGR_PLLBR_MULB(0); //désactive PLLB

	PMC->CKGR_PLLBR = PMC->CKGR_PLLBR | CKGR_PLLBR_MULB(9/*+1 une fois dans le registre*/) | CKGR_PLLBR_DIVB(1) | CKGR_PLLBR_PLLBCOUNT(PLL_COUNT) ; //On rentre notre configuration de division et de multiplication
	
	while (!(PMC->PMC_SR & PMC_SR_LOCKB)); //On attend que le PLL soit prêt



	PMC->PMC_MCKR = PMC->PMC_MCKR & ~PMC_MCKR_PLLADIV2 & ~PMC_MCKR_PLLBDIV2; //On désactive la division par 2

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | PMC_MCKR_PRES_CLK_1; //On met le prescaler à 1 (division de la clock par X) (Pour mettre la clock PLL en clock maitre il faut d'abord initialiser le prescaler)
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête

	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLB_CLK; //On sélectionne la clock PLLB en clock maitre
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY)); //On attend que la configuration soit prête
	
}
