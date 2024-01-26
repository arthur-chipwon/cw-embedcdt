/**
 * @file uart_interface.c
 * @brief An application interface for UART bus.
 * @copyright Copyright (c) 2023 ChipWon Technology. All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "uart_interface.h"
#include "clock_gating.h"

/*******************************************************************************
 * Private macros, defines, enumerations, structures(with typedefs)
 ******************************************************************************/
#define DEC_NOTATION (10) // Ah(10)
#define HEX_NOTATION (16) // 10h(16)

#define PREFIX_SCHEMA     (1 << 6)
#define POSTFIX_SCHEMA    (1 << 7)
#define CIRCUMFIX_SCHEMA  (PREFIX_SCHEMA | POSTFIX_SCHEMA)
#define AFFIX_SYMBOL_MASK (~CIRCUMFIX_SCHEMA)
#define FIX_SPACE         (1)
#define FIX_NEW_LINE      (2)
#define FIX_HEX           (3)
#define FIX_0X            (4)
#define AFFIX_NONE        (0x00)
#define PREFIX_SPACE      (PREFIX_SCHEMA | FIX_SPACE)
#define POSTFIX_NEW_LINE  (POSTFIX_SCHEMA | FIX_NEW_LINE)
#define POSTFIX_HEX       (POSTFIX_SCHEMA | FIX_HEX)

/*******************************************************************************
 * Private typedefs
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/
uartif_state_t g_uartif_state = {0u};

#warning "UART baud rate is currently configured as 115200."
const uartif_config_t g_uartif_init_config = {.baud_rate          = 115200u,
                                              .transfer_type      = LPUART_USING_INTERRUPTS,
                                              .parity_mode        = LPUART_PARITY_DISABLED,
                                              .stop_bit_count     = LPUART_ONE_STOP_BIT,
                                              .bit_count_per_char = LPUART_8_BITS_PER_CHAR,
                                              .rx_dma_channel     = 0u,
                                              .tx_dma_channel     = 0u};

/*******************************************************************************
 * Static variables
 ******************************************************************************/
/**
 * @brief Pointer to uartif runtime state structure.
 */
static uartif_state_t *sp_uartif_state_ptr = NULL;

static const char s_hex_tbl[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static uint8_t s_digit_buf[10u] = {0u};

/*******************************************************************************
 * Static function prototypes(including static inline function prototypes)
 ******************************************************************************/
static void uartif_isr(void);
static void uartif_set_baud_rate(uint32_t desired_baud_rate, clock_names_t inst_clk_name);
static bool uartif_tx_ring_buf_full(void);
static bool uartif_tx_ring_buf_empty(void);
static bool uartif_rx_ring_buf_full(void);
static void uartif_tx_put_number(uint8_t notation, uint32_t num);
static void uartif_tx_put_affix_u8(uint8_t affix, uint8_t val);
static void uartif_tx_put_affix_u16(uint8_t affix, uint16_t val);
static void uartif_tx_put_affix_u32(uint8_t affix, uint32_t val);

/*******************************************************************************
 * Global function definitions(excluding global inline function definitions,
 * C99 compliance)
 ******************************************************************************/
#if (INST_UARTIF == INST_LPUART0)
/**
 * @brief This function is the implementation of LPUART0 handler named in startup
 * code.
 */
void
LPUART0_IRQHandler(void)
{
    uartif_isr();
}
#endif /* (INST_UARTIF == INST_LPSPI0) */

#if (INST_UARTIF == INST_LPUART1)
/**
 * @brief This function is the implementation of LPUART1 handler named in startup
 * code.
 */
void
LPUART1_IRQHandler(void)
{
    uartif_isr();
}
#endif /* (INST_UARTIF == INST_LPSPI1) */

status_t
uartif_init(uint32_t instance, uartif_state_t *p_state_ptr, const uartif_config_t *p_user_config)
{
    DEV_ASSERT(LPUART_INSTANCE_COUNT > instance);
    DEV_ASSERT(NULL != p_state_ptr);
    DEV_ASSERT(NULL != p_user_config);
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL == sp_uartif_state_ptr);

    LPUART_t *p_base = NULL;
    uint32_t  idx;
    uint32_t  source_clk_freq;

    /** Clear the state struct for this instance. */
    memset((void *)p_state_ptr, 0x00u, sizeof(uartif_state_t));
    /** Save runtime structure pointer. */
    sp_uartif_state_ptr = p_state_ptr;
    p_state_ptr->p_base = p_base = gp_lpuart_base[instance];
    p_state_ptr->p_tx_rx_irq_id  = &g_lpuart_rx_tx_irq_id[instance];
    p_state_ptr->p_clk_name      = &g_lpuart_clk_names[instance];

    /* Get the LPUART clock as configured in the clock manager */
    (void)clock_get_freq(*(p_state_ptr->p_clk_name), &source_clk_freq);
    /* Check if current instance is clock gated off. */
    DEV_ASSERT(0u < source_clk_freq);

    /* Save the transfer information for runtime retrieval */
    p_state_ptr->transfer_type      = p_user_config->transfer_type;
    p_state_ptr->bit_count_per_char = p_user_config->bit_count_per_char;

    /* initialize the LPUART instance */
    lpuart_init_set(p_base);

    /* initialize the parameters of the LPUART config structure with desired data */
#if 0 /**< Marked by Arthur 20230317 */
    (void)lpuart_set_baud_rate(instance, p_user_config->baud_rate);
#endif
    uartif_set_baud_rate(p_user_config->baud_rate, *(p_state_ptr->p_clk_name));

    if (p_user_config->parity_mode != LPUART_PARITY_DISABLED)
    {
        lpuart_set_bit_count_per_char(p_base, p_user_config->bit_count_per_char, true);
    }
    else
    {
        lpuart_set_bit_count_per_char(p_base, p_user_config->bit_count_per_char, false);
    }
    lpuart_set_parity_mode(p_base, p_user_config->parity_mode);
    lpuart_set_stop_bit_count(p_base, p_user_config->stop_bit_count);

    /** Enable the vector interrupt. */
    int_enable_irq(*(p_state_ptr->p_tx_rx_irq_id));

    return STATUS_SUCCESS;
}

status_t
uartif_deinit(void)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;

    /** Disable the vector interrupt. */
    int_disable_irq(*(p_state_ptr->p_tx_rx_irq_id));
    /** Clear the state pointer. */
    sp_uartif_state_ptr = NULL;

    return STATUS_SUCCESS;
}

void
uartif_install_tx_ring_buffer(uint8_t *p_buf, uint32_t buf_size)
{
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    DEV_ASSERT(NULL != p_buf);
    DEV_ASSERT(2u <= buf_size);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    LPUART_t       *p_base      = p_state_ptr->p_base;

    /* Check if current instance is already initialized. */
    p_state_ptr->b_tx_int_mute  = false;
    p_state_ptr->tx_size_alloc  = buf_size;
    p_state_ptr->p_tx_ring_buf  = p_buf;
    p_state_ptr->p_tx_front_ptr = p_state_ptr->p_tx_rear_ptr = p_buf;
    p_state_ptr->p_tx_bottom_ptr                             = p_buf;
    p_state_ptr->p_tx_top_ptr                                = p_buf + (buf_size - 1u);

    /* Disable tx empty interrupt */
    lpuart_set_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY, false);
    /* Disable the LPUART transmitter */
    lpuart_set_transmitter_cmd(p_base, true);
}

void
uartif_install_rx_ring_buffer(uint8_t *p_buf, uint32_t buf_size)
{
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    DEV_ASSERT(NULL != p_buf);
    DEV_ASSERT(2u <= buf_size);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    LPUART_t       *p_base      = p_state_ptr->p_base;

    /* Check if current instance is already initialized. */
    p_state_ptr->b_rx_int_mute  = false;
    p_state_ptr->rx_size_alloc  = buf_size;
    p_state_ptr->p_rx_ring_buf  = p_buf;
    p_state_ptr->p_rx_front_ptr = p_state_ptr->p_rx_rear_ptr = p_buf;
    p_state_ptr->p_rx_bottom_ptr                             = p_buf;
    p_state_ptr->p_rx_top_ptr                                = p_buf + (buf_size - 1u);

    /* Enable the receiver */
    lpuart_set_receiver_cmd(p_base, true);
    /* Enable receive data full interrupt */
    lpuart_set_int_mode(p_base, LPUART_INT_RX_DATA_REG_FULL, true);
}

void
uartif_print_format(const char *p_fmt, ...)
{
    va_list  instance; /**<  an instance is about a list for subsequent use. */
    uint32_t arg_u32;
    uint8_t  arg_u8;

    va_start(instance, p_fmt);
    for (;;)
    {
        if ('\0' == *p_fmt)
        {
            break;
        }
        if ('%' == *p_fmt)
        {
            p_fmt++;
            switch (*p_fmt)
            {
                case 'd':
                case 'D':
                {
                    arg_u32 = va_arg(instance, uint32_t);
                    uartif_tx_put_number(DEC_NOTATION, arg_u32);
                }
                break;
                case 'x':
                case 'X':
                {
                    arg_u32 = va_arg(instance, uint32_t);
                    if ('x' == *p_fmt)
                    {
                        uartif_tx_put_number(HEX_NOTATION, arg_u32);
                    }
                    else
                    {
                        uartif_tx_put_affix_u32(AFFIX_NONE, arg_u32);
                    }
                }
                break;
                case 'c':
                case 'C':
                {
                    arg_u8 = (uint8_t)va_arg(instance, uint32_t);
                    uartif_tx_put_char(arg_u8);
                }
                break;
                default:
                {
                    uartif_tx_put_char('%');
                    uartif_tx_put_char((uint8_t)*p_fmt);
                }
                break;
            }
            p_fmt++;
            continue;
        }
        uartif_tx_put_char((uint8_t)*p_fmt);
        if ('\n' == *p_fmt)
        {
            break;
        }
        else if ('\x16' == *p_fmt)
        {
            uartif_print_await_done();
        }
        p_fmt++;
    }
    va_end(instance);
}

void
uartif_print_buffer(const uint8_t *p_buf, uint32_t buf_size)
{
    uint16_t       local_addr;
    uint32_t       tmp_cntr;
    uint32_t       skip_byte_cntr;
    uint32_t       present_len;
    const uint8_t *p_bur_ptr;

    DEV_ASSERT(NULL != p_buf);
    DEV_ASSERT(0u < buf_size);
    DEV_ASSERT(1024u >= buf_size);
    tmp_cntr       = (uint32_t)p_buf;
    skip_byte_cntr = tmp_cntr & 0x0000000Fu;
    present_len    = buf_size;
    if (skip_byte_cntr)
    {
        tmp_cntr -= skip_byte_cntr;
        present_len = buf_size + skip_byte_cntr;
    }
    uartif_tx_put_affix_u32(POSTFIX_HEX, tmp_cntr & 0xFFFFFF00u);
    /* show row number in hexadecimal format */
    for (tmp_cntr = 0u; sizeof(s_hex_tbl) > tmp_cntr; tmp_cntr++)
    {
        if (!tmp_cntr)
        {
            uartif_tx_put_char('\n');
            uartif_tx_put_char('+');
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
        }
        if (!(tmp_cntr & 0x00000007u))
        {
            uartif_tx_put_char(' ');
        }
        uartif_tx_put_char(' ');
        uartif_tx_put_char(' ');
        uartif_tx_put_char(s_hex_tbl[tmp_cntr]);
    }
    for (tmp_cntr = 0u, local_addr = (uint16_t)((uint32_t)p_buf & 0x000000F0u), p_bur_ptr = p_buf;
         tmp_cntr < present_len;
         tmp_cntr++, local_addr++)
    {
        if (!(tmp_cntr & 0x0000000Fu))
        {
            uartif_tx_put_char('\n');
            uartif_tx_put_affix_u16(POSTFIX_HEX, local_addr);
            uartif_tx_put_char(' ');
        }
        if (!(tmp_cntr & 0x00000007u))
        {
            uartif_tx_put_char(' ');
        }
        if (skip_byte_cntr)
        {
            skip_byte_cntr--;
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
            uartif_tx_put_char(' ');
            continue;
        }
        uartif_tx_put_affix_u8(PREFIX_SPACE, *p_bur_ptr);
        p_bur_ptr++;
    }
    uartif_tx_put_char('\n');
}

void
uartif_print_await_done(void)
{
    for (;;)
    {
        if (uartif_tx_ring_buf_empty())
        {
            break;
        }
    }
}

bool
uartif_acquire_new_arrival_char(uint32_t buf_size, uint32_t *p_size_acq, uint8_t *p_buf)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr     = sp_uartif_state_ptr;
    LPUART_t       *p_base          = p_state_ptr->p_base;
    bool            b_line_terminal = false;
    uint8_t         end_char        = '\0';
    uint8_t        *p_rx_rear_ptr   = NULL;
    uint8_t        *p_rx_front_ptr  = NULL;
    uint8_t         loc_cntr        = 0u;

    p_state_ptr->b_rx_int_mute = true;
    lpuart_set_int_mode(p_base, LPUART_INT_RX_DATA_REG_FULL, false);
    p_rx_rear_ptr  = (uint8_t *)p_state_ptr->p_rx_rear_ptr;
    p_rx_front_ptr = (uint8_t *)p_state_ptr->p_rx_front_ptr;
    if (p_rx_rear_ptr == p_rx_front_ptr)
    {
        // Do nothing.
    }
    else
    {
        for (; (p_rx_rear_ptr != p_rx_front_ptr) && (loc_cntr < (buf_size - 1u)); loc_cntr++)
        {
            if ('\n' == end_char)
            {
                break;
            }
            end_char = *p_rx_rear_ptr;
            if ('\r' == end_char)
            {
                end_char = '\n';
            }
            else if ('\x7F' == end_char)
            {
                end_char = '\b';
            }
            if ('\n' == end_char)
            {
                if (loc_cntr < (buf_size - 3u))
                {
                    p_buf[loc_cntr] = ' ';
                    loc_cntr++;
                    p_buf[loc_cntr] = '\b';
                    loc_cntr++;
                }
            }
            else if ('\b' == end_char)
            {
                if (loc_cntr < (buf_size - 5u))
                {
                    p_buf[loc_cntr] = ' ';
                    loc_cntr++;
                    p_buf[loc_cntr] = end_char;
                    loc_cntr++;
                }
                if (loc_cntr < (buf_size - 3u))
                {
                    p_buf[loc_cntr] = end_char;
                    loc_cntr++;
                    p_buf[loc_cntr] = ' ';
                    loc_cntr++;
                }
            }
            p_buf[loc_cntr] = end_char;
            p_rx_rear_ptr++;
            if ((p_rx_rear_ptr - 1u) == p_state_ptr->p_rx_top_ptr)
            {
                p_rx_rear_ptr = (uint8_t *)p_state_ptr->p_rx_bottom_ptr;
            }
        }
        p_state_ptr->p_rx_rear_ptr = p_rx_rear_ptr;
    }
    lpuart_set_int_mode(p_base, LPUART_INT_RX_DATA_REG_FULL, true);
    p_state_ptr->b_rx_int_mute = false;
    if ('\n' == end_char)
    {
        b_line_terminal = true;
    }
    p_buf[loc_cntr] = '\0';
    uartif_print_format((const char *)p_buf);
    if (NULL != p_size_acq)
    {
        *p_size_acq = loc_cntr;
    }
    return b_line_terminal;
}

void
uartif_tx_put_char(uint8_t letter)
{
    uartif_state_t *p_state_ptr    = sp_uartif_state_ptr;
    LPUART_t       *p_base         = p_state_ptr->p_base;
    *(p_state_ptr->p_tx_front_ptr) = letter;
    while (uartif_tx_ring_buf_full())
        ;
    p_state_ptr->b_tx_int_mute = true;
    /* Enable tx empty interrupt */
    lpuart_set_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY, false);
    if (uartif_tx_ring_buf_empty())
    {
        /* Transmit the data */
        lpuart_put_char(p_base, letter);
    }
    p_state_ptr->p_tx_front_ptr++;
    if ((p_state_ptr->p_tx_front_ptr - 1u) == p_state_ptr->p_tx_top_ptr)
    {
        p_state_ptr->p_tx_front_ptr = p_state_ptr->p_tx_bottom_ptr;
    }
    /* Enable tx empty interrupt */
    lpuart_set_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY, true);
    p_state_ptr->b_tx_int_mute = false;
}

/*******************************************************************************
 * Static functions(including static inline function definitions)
 ******************************************************************************/
static void
uartif_isr(void)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    LPUART_t       *p_base      = p_state_ptr->p_base;
    uint32_t        status_bm;

    status_bm = lpuart_get_status_bm(p_base);
    lpuart_clear_status_bm(p_base, status_bm & (uint32_t)LPUART_ALL_STATUS);
    /* Handle receive data full interrupt */
    if (lpuart_get_int_mode(p_base, LPUART_INT_RX_DATA_REG_FULL) &&
        (status_bm & LPUART_STAT_RDRF_MASK))
    {
        status_bm &= ~LPUART_STAT_RDRF_MASK;
        /* Receive the data */
        lpuart_get_char(p_base, (uint8_t *)p_state_ptr->p_rx_front_ptr);
        if (!uartif_rx_ring_buf_full())
        {
            p_state_ptr->p_rx_front_ptr++;
            if ((p_state_ptr->p_rx_front_ptr - 1u) == p_state_ptr->p_rx_top_ptr)
            {
                p_state_ptr->p_rx_front_ptr = p_state_ptr->p_rx_bottom_ptr;
            }
        }
        else
        {
            // Do nothing.
        }
    }

    /* Handle transmitter data register empty interrupt */
    if (lpuart_get_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY) &&
        (status_bm & LPUART_STAT_TDRE_MASK))
    {
        status_bm &= ~LPUART_STAT_TDRE_MASK;
        if (!uartif_tx_ring_buf_empty())
        {
            p_state_ptr->p_tx_rear_ptr++;
            if ((p_state_ptr->p_tx_rear_ptr - 1u) == p_state_ptr->p_tx_top_ptr)
            {
                p_state_ptr->p_tx_rear_ptr = p_state_ptr->p_tx_bottom_ptr;
            }
            if (p_state_ptr->p_tx_rear_ptr == p_state_ptr->p_tx_front_ptr)
            {
                /* If the transfer is aborted or timed out, disable tx empty interrupt */
                lpuart_set_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY, false);
            }
            else
            {
                uint8_t letter = *(p_state_ptr->p_tx_rear_ptr);
                lpuart_put_char(p_base, letter);
            }
        }
        else
        {
            /* If the transfer is aborted or timed out, disable tx empty interrupt */
            lpuart_set_int_mode(p_base, LPUART_INT_TX_DATA_REG_EMPTY, false);
        }
    }

    /* Handle transmission complete interrupt */
    if (lpuart_get_int_mode(p_base, LPUART_STAT_TC_MASK) && (status_bm & LPUART_STAT_TC_MASK))
    {
        status_bm &= ~LPUART_STAT_TC_MASK;
        if (lpuart_get_status_flag(p_base, LPUART_TX_COMPLETE))
        {
            // Do nothing.
        }
    }
}

static void
uartif_set_baud_rate(uint32_t desired_baud_rate, clock_names_t inst_clk_name)
{
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    LPUART_t       *p_base      = p_state_ptr->p_base;
    uint16_t        sbr, sbr_temp, idx;
    uint32_t        osr, temp_diff, calculated_baud, baud_diff, max_osr;
    uint32_t        lpuart_source_clock;

    /* Get the LPUART clock as configured in the clock manager */
    (void)clock_get_freq(inst_clk_name, &lpuart_source_clock);

    /* Check if current instance is clock gated off. */
    DEV_ASSERT(0u < lpuart_source_clock);
    /* Check if the desired baud rate can be configured with the current
     * protocol clock. */
    DEV_ASSERT(lpuart_source_clock >= (desired_baud_rate * 4u));

    /* This lpuart instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, osr is typically hard-set to 16 in other lpuart instantiations
     * First calculate the baud rate using the minimum OSR possible (4) */
    osr             = 4u;
    sbr             = (uint16_t)(lpuart_source_clock / (desired_baud_rate * osr));
    calculated_baud = (lpuart_source_clock / (osr * sbr));
    if (calculated_baud > desired_baud_rate)
    {
        baud_diff = calculated_baud - desired_baud_rate;
    }
    else
    {
        baud_diff = desired_baud_rate - calculated_baud;
    }
    /* find maximum osr */
    max_osr = lpuart_source_clock / desired_baud_rate;
    if (32u < max_osr)
    {
        max_osr = 32u;
    }
    /* loop to find the best osr value possible, one that generates minimum
     * baud_diff iterate through the rest of the supported values of osr */
    if (5u <= max_osr)
    {
        for (idx = 5u; idx <= max_osr; idx++)
        {
            /* calculate the temporary sbr value   */
            sbr_temp = (uint16_t)(lpuart_source_clock / (desired_baud_rate * idx));
            /* calculate the baud rate based on the temporary osr and sbr values
             */
            calculated_baud = (lpuart_source_clock / (idx * sbr_temp));

            if (calculated_baud > desired_baud_rate)
            {
                temp_diff = calculated_baud - desired_baud_rate;
            }
            else
            {
                temp_diff = desired_baud_rate - calculated_baud;
            }

            if (temp_diff <= baud_diff)
            {
                baud_diff = temp_diff;
                osr       = idx;      /* update and store the best osr value calculated */
                sbr       = sbr_temp; /* update store the best sbr value calculated */
            }
        }
    }
    /* Check if osr is between 4x and 7x oversampling.
     * If so, then "BOTHEDGE" sampling must be turned on */
    if (8u > osr)
    {
        lpuart_enable_both_edge_sampling(p_base);
    }

    /* program the osr value (bit value is one less than actual value) */
    lpuart_set_oversampling_ratio(p_base, (osr - 1u));

    /* write the sbr value to the BAUD registers */
    lpuart_set_baud_rate_divisor(p_base, sbr);
}

static bool
uartif_tx_ring_buf_full(void)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    if (p_state_ptr->p_tx_rear_ptr == p_state_ptr->p_tx_bottom_ptr)
    {
        if (p_state_ptr->p_tx_front_ptr == p_state_ptr->p_tx_top_ptr)
        {
            return true;
        }
        return false;
    }
    if ((p_state_ptr->p_tx_rear_ptr - 1u) == p_state_ptr->p_tx_front_ptr)
    {
        return true;
    }
    return false;
}

static bool
uartif_tx_ring_buf_empty(void)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    if (p_state_ptr->p_tx_rear_ptr == p_state_ptr->p_tx_front_ptr)
    {
        return true;
    }
    return false;
}

static bool
uartif_rx_ring_buf_full(void)
{
    /* Check if current instance is already initialized. */
    DEV_ASSERT(NULL != sp_uartif_state_ptr);
    uartif_state_t *p_state_ptr = sp_uartif_state_ptr;
    if (p_state_ptr->p_rx_rear_ptr == p_state_ptr->p_rx_bottom_ptr)
    {
        if (p_state_ptr->p_rx_front_ptr == p_state_ptr->p_rx_top_ptr)
        {
            return true;
        }
        return false;
    }
    if ((p_state_ptr->p_rx_rear_ptr - 1u) == p_state_ptr->p_rx_front_ptr)
    {
        return true;
    }
    return false;
}

static void
uartif_tx_put_number(uint8_t notation, uint32_t num)
{
    uint8_t tmp_cntr;

    for (tmp_cntr = 0u;; tmp_cntr++)
    {
        s_digit_buf[tmp_cntr] = s_hex_tbl[num % notation];
        num /= notation;
        if (!num)
        {
            break;
        }
    }
    for (; (uint8_t)((int8_t)(-1)) != tmp_cntr; tmp_cntr--)
    {
        uartif_tx_put_char(s_digit_buf[tmp_cntr]);
    }
    if (notation == DEC_NOTATION)
    {
        uartif_tx_put_char('\'');
    }
}

static void
uartif_tx_put_affix_u8(uint8_t affix, uint8_t val)
{
    if (affix & PREFIX_SCHEMA)
    {
        uartif_tx_put_char(' ');
    }
    uartif_tx_put_char(s_hex_tbl[val >> 4u]);
    uartif_tx_put_char(s_hex_tbl[val & 0x0Fu]);
    if (affix & POSTFIX_SCHEMA)
    {
        switch (affix & AFFIX_SYMBOL_MASK)
        {
            case FIX_HEX:
            {
                uartif_tx_put_char('h');
            }
            break;
            case FIX_NEW_LINE:
            {
                uartif_tx_put_char('\n');
            }
            break;
            default:
            {
                // Do nothing.
            }
            break;
        }
    }
}

static void
uartif_tx_put_affix_u16(uint8_t affix, uint16_t val)
{
    if (affix & PREFIX_SCHEMA)
    {
        uartif_tx_put_char(' ');
    }
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 1)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 1)) & 0x0Fu]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val)) & 0x0Fu]);
    if (affix & POSTFIX_SCHEMA)
    {
        switch (affix & AFFIX_SYMBOL_MASK)
        {
            case FIX_HEX:
            {
                uartif_tx_put_char('h');
            }
            break;
            case FIX_NEW_LINE:
            {
                uartif_tx_put_char('\n');
            }
            break;
            default:
            {
                // Do nothing.
            }
            break;
        }
    }
}

static void
uartif_tx_put_affix_u32(uint8_t affix, uint32_t val)
{
    if (affix & PREFIX_SCHEMA)
    {
        uartif_tx_put_char(' ');
    }
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 3u)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 3u)) & 0x0Fu]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 2u)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 2u)) & 0x0F]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 1u)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val + 1u)) & 0x0Fu]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val)) >> 4u]);
    uartif_tx_put_char(s_hex_tbl[(*((uint8_t *)&val)) & 0x0Fu]);
    if (affix & POSTFIX_SCHEMA)
    {
        switch (affix & AFFIX_SYMBOL_MASK)
        {
            case FIX_HEX:
            {
                uartif_tx_put_char('h');
            }
            break;
            case FIX_NEW_LINE:
            {
                uartif_tx_put_char('\n');
            }
            break;
            default:
            {
                // Do nothing.
            }
            break;
        }
    }
}

/*** end of file ***/
