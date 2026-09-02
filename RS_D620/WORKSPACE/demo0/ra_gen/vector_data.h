/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (7)
        #endif
        /* ISR prototypes */
        void fcu_frdyi_isr(void);
        void fcu_fiferr_isr(void);
        void gpt_counter_overflow_isr(void);
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 0) /* FCU FRDYI (Flash ready interrupt) */
        #define FCU_FRDYI_IRQn          ((IRQn_Type) 0) /* FCU FRDYI (Flash ready interrupt) */
        #define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 1) /* FCU FIFERR (Flash access error interrupt) */
        #define FCU_FIFERR_IRQn          ((IRQn_Type) 1) /* FCU FIFERR (Flash access error interrupt) */
        #define VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW ((IRQn_Type) 2) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #define GPT2_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 2) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 3) /* SCI0 RXI (Receive data full) */
        #define SCI0_RXI_IRQn          ((IRQn_Type) 3) /* SCI0 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 4) /* SCI0 TXI (Transmit data empty) */
        #define SCI0_TXI_IRQn          ((IRQn_Type) 4) /* SCI0 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 5) /* SCI0 TEI (Transmit end) */
        #define SCI0_TEI_IRQn          ((IRQn_Type) 5) /* SCI0 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 6) /* SCI0 ERI (Receive error) */
        #define SCI0_ERI_IRQn          ((IRQn_Type) 6) /* SCI0 ERI (Receive error) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (7)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */