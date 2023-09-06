#ifndef _STD_MACROS_H_
#define _STD_MACROS_H_

#define REGISTER_SIZE 8

#define SET_BIT(reg,bit)    reg |= (1<<bit)
#define CLR_BIT(reg,bit)    reg &= (~(1<<bit))
#define TOG_BIT(reg,bit)    reg ^= (1<<bit)
#define READ_BIT(reg,bit)    ( reg & (1<<bit)) >> bit
#define IS_BIT_SET(reg,bit)  (reg&(1<<bit))>>bit
#define IS_BIT_CLR(reg,bit)  !((reg&(1<<bit))>>bit)
#define ROR(reg,num)         reg=(reg<<(REGISTER_SIZE-num))|(reg>>(num))
#define ROL(reg,num)        reg= (reg>>(REGISTER_SIZE-num))|(reg<<(num))

//Rising edge
#define RISING_EDGE_SETUP() MCUCR|= (1<<1) | (1<<0)

//Setup use INT0
#define SETUP_INT0() GICR|=(1<<6)

#endif // !_STD_MACROS_H_