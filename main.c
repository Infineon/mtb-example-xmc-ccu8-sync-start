/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the XMC MCU: CCU8 Timer synchronous
*              start Example for ModusToolbox.
*
* Related Document: See README.md
*
******************************************************************************
*
* $ Copyright 2015-2022 Infineon Technologies AG $
*                                                                              
*****************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"
#include <stdio.h>
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/

/*Define macros for XMC13x Boot kit*/
#if (UC_SERIES == XMC13)
#define INTERRUPT_PRIORITY_NODE_ID              ERU0_0_IRQn
#define INTERRUPT_EVENT_PRIORITY                (3U)
#define ERU_EXTERNAL_EVENT_HANDLER              ERU0_0_IRQHandler
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     3
#define TIMER_1_PERIOD_MATCH_EVENT_PRIORITY     3
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN
#define TIMER_1_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR1_INTERRUPT_HANDLER
#define TIMER_1_PERIOD_MATCH_EVENT_IRQN         CCU80_SR1_IRQN
#endif

/*Define macros for XMC14x Boot kit*/
#if (UC_SERIES == XMC14)
#define INTERRUPT_PRIORITY_NODE_ID              IRQ3_IRQn
#define INTERRUPT_EVENT_PRIORITY                (3U)
#define ERU_EXTERNAL_EVENT_HANDLER              IRQ_Hdlr_3
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     3
#define TIMER_1_PERIOD_MATCH_EVENT_PRIORITY     3
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN
#define TIMER_1_PERIOD_MATCH_EVENT_HANDLER      CCU81_SR0_INTERRUPT_HANDLER
#define TIMER_1_PERIOD_MATCH_EVENT_IRQN         CCU81_SR0_IRQN
#endif

/*Define macros for XMC42x Relax kit*/
#if (UC_SERIES == XMC42)
#define INTERRUPT_PRIORITY_NODE_ID              ERU1_0_IRQn
#define INTERRUPT_EVENT_PRIORITY                (63U)
#define ERU_EXTERNAL_EVENT_HANDLER              ERU1_0_IRQHandler
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_1_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN
#define TIMER_1_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR1_INTERRUPT_HANDLER
#define TIMER_1_PERIOD_MATCH_EVENT_IRQN         CCU80_SR1_IRQN
#endif

/*Define macros for XMC43x Relax kit*/
#if (UC_SERIES == XMC43)
#define INTERRUPT_PRIORITY_NODE_ID              ERU0_0_IRQn
#define INTERRUPT_EVENT_PRIORITY                (63U)
#define ERU_EXTERNAL_EVENT_HANDLER              ERU0_0_IRQHandler
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_1_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR0_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR0_IRQN
#define TIMER_1_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR1_INTERRUPT_HANDLER
#define TIMER_1_PERIOD_MATCH_EVENT_IRQN         CCU80_SR1_IRQN
#endif

/*Definition of macros for XMC45x, XMC47x, XMC48x Relax kit, XMC44x kit */
#if (UC_SERIES == XMC44) || (UC_SERIES == XMC45) || \
    (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
#define INTERRUPT_PRIORITY_NODE_ID              ERU1_0_IRQn
#define INTERRUPT_EVENT_PRIORITY                (63U)
#define ERU_EXTERNAL_EVENT_HANDLER              IRQ_Hdlr_5
#define TIMER_0_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_1_PERIOD_MATCH_EVENT_PRIORITY     61
#define TIMER_0_PERIOD_MATCH_EVENT_HANDLER      CCU80_SR1_INTERRUPT_HANDLER
#define TIMER_0_PERIOD_MATCH_EVENT_IRQN         CCU80_SR1_IRQN
#define TIMER_1_PERIOD_MATCH_EVENT_HANDLER      CCU81_SR1_INTERRUPT_HANDLER
#define TIMER_1_PERIOD_MATCH_EVENT_IRQN         CCU81_SR1_IRQN
#endif

/* Define macro to enable/disable printing of debug messages */
#define ENABLE_XMC_DEBUG_PRINT                  (0)

/* Define macro to check if loop is entered once */
#if ENABLE_XMC_DEBUG_PRINT
static bool ENTER_LOOP1 = false;
static bool ENTER_LOOP2 = false;
#endif

/*******************************************************************************
* Global Variables
*******************************************************************************/
static volatile bool timer0_interrupt_flag = false;
static volatile bool timer1_interrupt_flag = false;

/*******************************************************************************
* Function Name: TIMER_0_PERIOD_MATCH_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CCU8 timer period match interrupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void TIMER_0_PERIOD_MATCH_EVENT_HANDLER(void)
{
    timer0_interrupt_flag = true;
}
/*******************************************************************************
* Function Name: TIMER_1_PERIOD_MATCH_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CCU81 timer period match interrupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void TIMER_1_PERIOD_MATCH_EVENT_HANDLER(void)
{
    timer1_interrupt_flag = true;
}

/*******************************************************************************
* Function Name: configure_eru
********************************************************************************
* Summary:
* This is the function which initializes all the parameters needed for the ERU.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void configure_eru(void)
{
#if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    /*Set Priority for IRQ*/
    NVIC_SetPriority(INTERRUPT_PRIORITY_NODE_ID, INTERRUPT_EVENT_PRIORITY);
#endif

#if (UC_SERIES == XMC42) || (UC_SERIES == XMC43) || (UC_SERIES == XMC44) || (UC_SERIES == XMC45) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
    /*Set Priority for IRQ*/
    NVIC_SetPriority(INTERRUPT_PRIORITY_NODE_ID, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), INTERRUPT_EVENT_PRIORITY, 0));
#endif

    /*Enable the Interrupt*/
    NVIC_EnableIRQ(INTERRUPT_PRIORITY_NODE_ID);
}

/*******************************************************************************
* Function Name: configure_timer0_interrupt
********************************************************************************
* Summary:
* This is the function which configures the parameters needed for the interrupt that
* will be generated by the period match of the CCU80 timer.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void configure_timer0_interrupt(void)
{

#if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    NVIC_SetPriority(TIMER_0_PERIOD_MATCH_EVENT_IRQN, TIMER_0_PERIOD_MATCH_EVENT_PRIORITY);
#endif

#if (UC_SERIES == XMC42) || (UC_SERIES == XMC43) || (UC_SERIES == XMC44) || (UC_SERIES == XMC45) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
    /*Set Interrupt priority by encoding the priority in the required groups*/
    NVIC_SetPriority(TIMER_0_PERIOD_MATCH_EVENT_IRQN, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TIMER_0_PERIOD_MATCH_EVENT_PRIORITY, 0));
#endif

    /*Enable IRQ*/
    NVIC_EnableIRQ(TIMER_0_PERIOD_MATCH_EVENT_IRQN);
}

/*******************************************************************************
* Function Name: configure_timer1_interrupt
********************************************************************************
* Summary:
* This is the function which configures the parameters needed for the interrupt that
* will be generated by the period match of the CCU81 timer.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void configure_timer1_interrupt(void)
{

#if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    NVIC_SetPriority(TIMER_1_PERIOD_MATCH_EVENT_IRQN, TIMER_1_PERIOD_MATCH_EVENT_PRIORITY);
#endif

#if (UC_SERIES == XMC42) || (UC_SERIES == XMC43) || (UC_SERIES == XMC44) || (UC_SERIES == XMC45) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
    /*Set Interrupt priority by encoding the priority in the required groups*/
    NVIC_SetPriority(TIMER_1_PERIOD_MATCH_EVENT_IRQN, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), TIMER_1_PERIOD_MATCH_EVENT_PRIORITY, 0));
#endif

    /*Enable IRQ*/
    NVIC_EnableIRQ(TIMER_1_PERIOD_MATCH_EVENT_IRQN);
}

/*******************************************************************************
* Function Name: ERU_EXTERNAL_EVENT_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the ERU external interrupt which starts
* the CCU8 timers and configures their respective interrupts for the toggling of LED's
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/

void ERU_EXTERNAL_EVENT_HANDLER(void)
{
    /*Configures the timers to receive the interrupts*/
    configure_timer0_interrupt();
    configure_timer1_interrupt();
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function.It sets up a CCU8 timers to period match interrupt.
* The main while loop checks for the elapsed time due to CCU8 timers period match
* event and toggles 2 LEDs at approximately 0.5Hz.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/

int main(void)
{

    cy_rslt_t result;

    /*Initialize the device and board peripherals*/
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
#if (UC_SERIES == XMC42) || (UC_SERIES == XMC43) || (UC_SERIES == XMC44) || (UC_SERIES == XMC45) || (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
    /*The USER leds on the 4700 kit are active high. The User LED2 is turned on here */
    XMC_GPIO_SetOutputHigh(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
#endif
#if (UC_SERIES == XMC13) || (UC_SERIES == XMC14)
    /*The USER leds on the 1400 kit are active low. The User LED2 is turned on here */
    XMC_GPIO_SetOutputLow(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
#endif

    /*Serial communication initialization function*/
    cy_retarget_io_init(CYBSP_DEBUG_UART_HW);

    #if ENABLE_XMC_DEBUG_PRINT
    printf("Configuration initialization complete\r\n");
    #endif

    /*Configures the Event Register Unit*/
    configure_eru();

    #if ENABLE_XMC_DEBUG_PRINT
    printf("ERU Configured\r\n");
    #endif

    /*Infinite loop*/
    while (1)
    {
        /*Check if timer elapsed (interrupt fired) and toggle the LED*/
        if (timer0_interrupt_flag)
        {
            /*Clear the timer 0 interrupt flag */
            timer0_interrupt_flag = false;
            /* Toggle the USER LED state */
            XMC_GPIO_ToggleOutput(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN);
            #if ENABLE_XMC_DEBUG_PRINT
            if (!ENTER_LOOP1)
            {
                printf("LED 1 Toggled\r\n");
                ENTER_LOOP1 = true;
            }
            #endif
        }
        if (timer1_interrupt_flag)
        {
            /*Clear the timer 1 interrupt flag */
            timer1_interrupt_flag = false;
            /* Toggle the USER LED state */
            XMC_GPIO_ToggleOutput(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
            #if ENABLE_XMC_DEBUG_PRINT
            if (!ENTER_LOOP2)
            {
                printf("LED 2 Toggled\r\n");
                ENTER_LOOP2 = true;
            }
            #endif
        }
    }
}

/* [] END OF FILE */
