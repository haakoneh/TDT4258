        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  	// Exception vector table
  	// This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	///////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  	// The CPU will start executing here after a reset
	//
	///////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset:

	///////////////////////////////////////////////////////////////////////
	//
  	// Setup GPIO clock
	//
	///////////////////////////////////////////////////////////////////////


	//Load CMU base address
	ldr r1, =CMU_BASE

	// load current value of HFPERCLK ENABLE
	ldr r2, [r1, #CMU_HFPERCLKEN0]

	//set bit for GPIO clk (load #1 to r3 and leftshift 13 bits)
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3 // activates the clock

	//Sets the new value
	str r2, [r1, #CMU_HFPERCLKEN0]
	
	///////////////////////////////////////////////////////////////////////
	//
  	// Configure LEDs on gamepad
	//
	///////////////////////////////////////////////////////////////////////

	//Load GPIO_CTRL til r1
	ldr r1, =GPIO_PA_BASE

	//Writes 0x1 to r2 to set low drive strength (0x2 for high)
	mov r2, #0x1

	str r2, [r1, #GPIO_CTRL]

	//Set pins 8-15 to output
	ldr r2, =0x55555555
	str r2, [r1, #GPIO_MODEH]

	///////////////////////////////////////////////////////////////////////
	//
  	// Configure button addresses
	//
	///////////////////////////////////////////////////////////////////////

	ldr r1, =GPIO_PC_BASE

	//Set pins 0-7 to input
	ldr r2, =0x33333333
	str r2, [r1, #GPIO_MODEL]

	//internal pull-up
	mov r2, #0xff
	str r2, [r1, #GPIO_DOUT]

	///////////////////////////////////////////////////////////////////////
	//
  	// Setup Energy Management Unit. Power savings mode
	//
	///////////////////////////////////////////////////////////////////////

	//Writing 6 to System Control Register to activate deep sleep
 	//as well as automatic return from interrupt handler

	ldr r1, =SCR
	mov r2, #6
	str r2, [r1]
	
	///////////////////////////////////////////////////////////////////////
	//
  	// Setup interrupts
	//
	///////////////////////////////////////////////////////////////////////

	ldr r1, =GPIO_BASE
	mov r2, #0x22222222
	str r2, [r1, #GPIO_EXTIPSELL]
	
	//Setting high to low transition (falling edge)
	mov r2, #0xff
	str r2, [r1, #GPIO_EXTIFALL]

	//Setting low to high transition (rising edge)
	mov r2, #0xff
	str r2, [r1, #GPIO_EXTIRISE]

	//Enable interrupt generation
	mov r2, #0xff
	str r2, [r1, #GPIO_IEN]

	//Enable interrupt-handling
	ldr r1, =ISER0
	ldr r2, =0x802 
	str r2, [r1]

	// Disable SRAM Blocks 1-3
	//ldr r1, =EMU_BASE
	//mov r2, #0x7
	//str r2, [r1, #EMU_MEMCTRL]
	
	//turn of LFACLK/LFBCLK (Energy Mode 3)
	//ldr r1, =CMU_BASE
	//mov r2, #0x1
	//lsl r2, #0x10
	//str r2, [r1, #CMU_LFCLKSEL]

	BX LR

	///////////////////////////////////////////////////////////////////////
	//
  	// GPIO handler. The CPU will jump here when there is a GPIO interrupt
  	// Since energy mode 2 is enabled, the CPU will automatically sleep on
	// return from this function
	// 
	///////////////////////////////////////////////////////////////////////
	
	.thumb_func
gpio_handler:

	//Clear interrupt handler
	ldr r1, =GPIO_BASE
	ldr r2, [r1, #GPIO_IF]
	str r2, [r1, #GPIO_IFC]

	///////////////////////////////////////////////////////////////////////
	//
  	// LED-handling
	//
	///////////////////////////////////////////////////////////////////////

	ldr r1, =GPIO_PC_BASE	
	ldr r2, [r1, #GPIO_DIN]  
	
	//Leftshifting 8 bits because pins 0-7 on port C and pins 8-15 on port A
        //use the same 16-bit register. We then get the led corresponding the 
	//the same button
	lsl r1, r2, #8
		
	ldr r2, =GPIO_PA_BASE

	//Turn on/off light
	str r1, [r2, #GPIO_DOUT]	

	BX LR

        .thumb_func
dummy_handler:  
        BX LR  // branch back to link register when called

