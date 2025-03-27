#include "hstx.h"




// Format: cmd length (including cmd byte), post delay in units of 5 ms, then cmd payload
// Note the delays have been shortened a little
#define SCREEN_W 128
#define SCREEN_H 128
#define COLOR_ORDER 0
static const uint8_t ssd1351_init_seq[] = {
        2, 1, 0xFD, 0x12,               // Unlock IC MCU interface
        2, 1, 0xFD, 0xB1,               // A2,B1,B3,BB,BE,C1
        1, 1, 0xAE,                     // Display off
        4, 1, 0xB2, 0xA4, 0x00, 0x00,   // DISPLAY_ENHANCEMENT
        2, 1, 0xB3, 0xF1,               // Clock divider F1 or F0
        2, 1, 0xCA, 0x7F,               // Mux Ratio
        2, 1, 0xA0, 0x74,               // Segment remapping
        2, 1, 0xA1, 0x00,               // Set display start line
        2, 1, 0xA2, 0x00,               // Set display offset
        2, 1, 0xB5, 0x00,               // Set GPIO
        2, 1, 0xAB, 0x01,               // Function Select
        2, 1, 0xB1, 0x32,               // Precharge 
        2, 1, 0xBB, 0x1F,               // Precharge Level
        2, 1, 0xBE, 0x05,               // Set VcomH Charge
        1, 1, 0xA6,                     // Normal Display
        2, 1, 0xC7, 0x0A,               // Contrast Master
        4, 1, 0xC1, 0xFF, 0xFF, 0xFF,   // Constrast RGB
        4, 1, 0xB4, 0xA0, 0xB5, 0x55,   // Set segment low voltage
        2, 1, 0xB6, 0x01,               // Precharge 2
        1, 1, 0xA6,
        1, 1, 0xAF,                     // Display ON
};



static inline void lcd_init(const uint8_t *init_seq) {
    const uint8_t *cmd = init_seq;
    while (*cmd) {
        lcd_write_cmd(cmd + 2, *cmd);
        sleep_ms(*(cmd + 1) * 5);
        cmd += *cmd + 2;
    }
}

void init_hstx(){
    // Switch HSTX to USB PLL (presumably 48 MHz) because clk_sys is probably
    // running a bit too fast for this example -- 48 MHz means 48 Mbps on
    // PIN_DIN. Need to reset around clock mux change, as the AUX mux can
    // introduce short clock pulses:
    reset_block(RESETS_RESET_HSTX_BITS);
    hw_write_masked(
        &clocks_hw->clk[clk_hstx].ctrl,
        CLOCKS_CLK_HSTX_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB << CLOCKS_CLK_HSTX_CTRL_AUXSRC_LSB,
        CLOCKS_CLK_HSTX_CTRL_AUXSRC_BITS
    );
    hw_set_bits(
        &clocks_hw->clk[clk_hstx].div, 
        2<<CLOCKS_CLK_HSTX_DIV_INT_LSB // keep the hstx clock div as 2  as the max spi clock is 24mhz 
    );
    /*hw_write_masked(
		&clocks_hw->clk[clk_hstx].div,
		3 << CLOCKS_CLK_HSTX_DIV_INT_LSB,
		CLOCKS_CLK_HSTX_DIV_INT_BITS
	);*/
   
    unreset_block_wait(RESETS_RESET_HSTX_BITS);
    
    gpio_init(PIN_RESET);
    //gpio_init(PIN_BL);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    //gpio_set_dir(PIN_BL, GPIO_OUT);
    gpio_put(PIN_RESET, 1);
    //gpio_put(PIN_BL, 1);

    hstx_ctrl_hw->bit[PIN_SCK - FIRST_HSTX_PIN] =
        HSTX_CTRL_BIT0_CLK_BITS;

    hstx_ctrl_hw->bit[PIN_DIN - FIRST_HSTX_PIN] =
        (7u << HSTX_CTRL_BIT0_SEL_P_LSB) |
        (7u << HSTX_CTRL_BIT0_SEL_N_LSB);

    hstx_ctrl_hw->bit[PIN_CS - FIRST_HSTX_PIN] =
        (27u << HSTX_CTRL_BIT0_SEL_P_LSB) |
        (27u << HSTX_CTRL_BIT0_SEL_N_LSB);

    hstx_ctrl_hw->bit[PIN_DC - FIRST_HSTX_PIN] =
        (17u << HSTX_CTRL_BIT0_SEL_P_LSB) |
        (17u << HSTX_CTRL_BIT0_SEL_N_LSB) |
        (HSTX_CTRL_BIT0_INV_BITS);

    // We have packed 8-bit fields, so shift left 1 bit/cycle, 8 times.
    hstx_ctrl_hw->csr =
        HSTX_CTRL_CSR_EN_BITS |
        (31u << HSTX_CTRL_CSR_SHIFT_LSB) |
        (8u << HSTX_CTRL_CSR_N_SHIFTS_LSB) |
        (1u << HSTX_CTRL_CSR_CLKDIV_LSB);

    gpio_set_function(PIN_SCK, 0/*GPIO_FUNC_HSTX*/);
    gpio_set_function(PIN_DIN, 0/*GPIO_FUNC_HSTX*/);
    gpio_set_function(PIN_CS,  0/*GPIO_FUNC_HSTX*/);
    gpio_set_function(PIN_DC,  0/*GPIO_FUNC_HSTX*/);

    lcd_init(ssd1351_init_seq);
}