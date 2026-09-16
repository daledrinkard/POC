/* generated common source file - do not edit */
#include "common_data.h"
icu_instance_ctrl_t g_SW1_ctrl;

/** External IRQ extended configuration for ICU HAL driver */
const icu_extended_cfg_t g_SW1_ext_cfg =
{
    .filter_src         = EXTERNAL_IRQ_DIGITAL_FILTER_PCLK_DIV,
};

const external_irq_cfg_t g_SW1_cfg =
{
    .channel             = 10,
    .trigger             = EXTERNAL_IRQ_TRIG_FALLING,
    .filter_enable       = true,
    .clock_source_div            = EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
    .p_callback          = SW1_cb,
    /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
    .p_context           = (void *) &NULL,
#endif
    .p_extend            = (void *)&g_SW1_ext_cfg,
    .ipl                 = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ10)
    .irq                 = VECTOR_NUMBER_ICU_IRQ10,
#else
    .irq                 = FSP_INVALID_VECTOR,
#endif
};
/* Instance structure to use this module. */
const external_irq_instance_t g_SW1 =
{
    .p_ctrl        = &g_SW1_ctrl,
    .p_cfg         = &g_SW1_cfg,
    .p_api         = &g_external_irq_on_icu
};
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
        {
            .p_api = &g_ioport_on_ioport,
            .p_ctrl = &g_ioport_ctrl,
            .p_cfg = &g_bsp_pin_cfg,
        };
void g_common_init(void) {
}
