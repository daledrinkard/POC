/* generated vector source file - do not edit */
        #include "bsp_api.h"
        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [1] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
            [2] = gpt_counter_overflow_isr, /* GPT2 COUNTER OVERFLOW (Overflow) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP0), /* FCU FRDYI (Flash ready interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,GROUP1), /* FCU FIFERR (Flash access error interrupt) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_GPT2_COUNTER_OVERFLOW,GROUP2), /* GPT2 COUNTER OVERFLOW (Overflow) */
        };
        #endif
        #endif