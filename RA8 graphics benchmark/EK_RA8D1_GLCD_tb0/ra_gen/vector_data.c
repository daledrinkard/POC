/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = drw_int_isr, /* DRW INT (DRW interrupt) */
            [1] = glcdc_line_detect_isr, /* GLCDC LINE DETECT (Specified line) */
            [2] = mipi_dsi_seq0, /* MIPIDSI SEQ0 (Sequence operation channel 0 interrupt) */
            [3] = mipi_dsi_seq1, /* MIPIDSI SEQ1 (Sequence operation channel 1 interrupt) */
            [4] = mipi_dsi_vin1, /* MIPIDSI VIN1 (Video-Input operation channel1 interrupt) */
            [5] = mipi_dsi_rcv, /* MIPIDSI RCV (DSI packet receive interrupt) */
            [6] = mipi_dsi_ferr, /* MIPIDSI FERR (DSI fatal error interrupt) */
            [7] = mipi_dsi_ppi, /* MIPIDSI PPI (DSI D-PHY PPI interrupt) */
            [8] = sci_b_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [9] = sci_b_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [10] = sci_b_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [11] = sci_b_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [12] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [13] = gpt_counter_overflow_isr, /* GPT2 COUNTER OVERFLOW (Overflow) */
            [14] = r_icu_isr, /* ICU IRQ12 (External pin interrupt 12) */
            [15] = r_icu_isr, /* ICU IRQ13 (External pin interrupt 13) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_DRW_INT,GROUP0), /* DRW INT (DRW interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_GLCDC_LINE_DETECT,GROUP1), /* GLCDC LINE DETECT (Specified line) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_SEQ0,GROUP2), /* MIPIDSI SEQ0 (Sequence operation channel 0 interrupt) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_SEQ1,GROUP3), /* MIPIDSI SEQ1 (Sequence operation channel 1 interrupt) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_VIN1,GROUP4), /* MIPIDSI VIN1 (Video-Input operation channel1 interrupt) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_RCV,GROUP5), /* MIPIDSI RCV (DSI packet receive interrupt) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_FERR,GROUP6), /* MIPIDSI FERR (DSI fatal error interrupt) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_MIPIDSI_PPI,GROUP7), /* MIPIDSI PPI (DSI D-PHY PPI interrupt) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP0), /* SCI9 RXI (Receive data full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP1), /* SCI9 TXI (Transmit data empty) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP2), /* SCI9 TEI (Transmit end) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP3), /* SCI9 ERI (Receive error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP4), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_GPT2_COUNTER_OVERFLOW,GROUP5), /* GPT2 COUNTER OVERFLOW (Overflow) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ12,GROUP6), /* ICU IRQ12 (External pin interrupt 12) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ13,GROUP7), /* ICU IRQ13 (External pin interrupt 13) */
        };
        #endif
        #endif
