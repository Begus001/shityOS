#include <io/serial.h>

#include "intr/pic.h"

#define ICW1_READ_ICW4      0x01  // Expect ICW4
#define ICW1_SINGLE         0x02  // Only one PIC in system
#define ICW1_INTERVAL4      0x04  // idt has 4 byte entries (compact jump tables)
#define ICW1_NO_EDGE_DETECT 0x08  // Use level detection instead of edge detection
#define ICW1_INIT           0x10  // Begin PIC initializition

#define IRQ0_7_POS  0x20
#define IRQ8_15_POS 0x28

#define ICW3_MASTER_IRQ 0x02  // IRQ, where specific slave is connected to master (send to slaves)
#define ICW3_SLAVE_MASK 0x04  // IRQ, where slaves are connected to master (send to master)

#define ICW4_NO_MICRO_MODE 0x01  // Sets PIC to µProcessor mode if zero (!)
#define ICW4_AUTO_EOI      0x02  // Automatic end of interrupt
#define ICW4_SET_MASTER    0x04  // Is master or slave?
#define ICW4_BUF_MODE      0x08  // Sends signal to enable buffers when outputting to bus
#define ICW4_NESTED_MODE   0x10  /*    Service requests from slaves whose interrupt requests are being
										serviced at the moment */

void pic_init(void)
{
	// Master initialization
	out8(PIC_CMD_ADDR, ICW1_READ_ICW4 | ICW1_INIT);
	out8(PIC_DAT_ADDR, IRQ0_7_POS);
	out8(PIC_DAT_ADDR, ICW3_SLAVE_MASK);
	out8(PIC_DAT_ADDR, ICW4_NO_MICRO_MODE | ICW4_SET_MASTER | ICW4_NESTED_MODE | ICW4_AUTO_EOI);
	
	// Slave initialization
	out8(PIC_SL_CMD_ADDR, ICW1_READ_ICW4 | ICW1_INIT);
	out8(PIC_SL_DAT_ADDR, IRQ8_15_POS);
	out8(PIC_SL_DAT_ADDR, ICW3_MASTER_IRQ);
	out8(PIC_SL_DAT_ADDR, ICW4_NO_MICRO_MODE | ICW4_NESTED_MODE | ICW4_AUTO_EOI);
	
	// Demask IRQ0
	out8(PIC_DAT_ADDR, 0xFE);
	out8(PIC_SL_DAT_ADDR, 0x0);
}
