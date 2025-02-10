/*
 * wifi.h
 *
 * Created: 2025/2/10 14:30:41
 *  Author: 17713
 */ 


#ifndef WIFI_H_
#define WIFI_H_

void wifi usart handler(void);
void process incoming byte wifi(uint8 t in byte);
void wifi command response handler(uint32 t ul id, uint32 t ul mask);
void process data wifi (void);
void wifi provision handler(uint32 t ul id, uint32 t ul mask);
void wifi spi handler(void);
void configure usart wifi(void);
void configure wifi comm pin(void);
void configure wifi provision pin(void);
void configure spi(void);
void spi peripheral initialize(void);
void prepare spi transfer(void);
void write wifi command(char* comm, uint8 t cnt);
void write image to web(void);



#endif /* WIFI_H_ */