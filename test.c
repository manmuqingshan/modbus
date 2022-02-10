#include <stdio.h>

#include "mb.h"

#include "mb-crc.h"
#include "mb-packet.h"

void send_data_to_mb(uint8_t * Data,uint8_t Size)
{
    int i;
    for(i=0;i<Size;i++)
    {
        mb_rx_new_data(Data[i]);
    }
}

#if(MB_MODE==MB_MODE_MASTER)
void master_process(mb_packet_s Packet)
{
    #ifdef MB_DEBUG
    printf("MASTER: OK F:%02x T:%02x \n",Packet.func,Packet.type);
    #endif
}
#endif

void send_data(uint8_t *Data,uint8_t Len)
{
    #ifdef MB_DEBUG
    int i;
    printf("TX: ");
    for(i=0;i<Len;i++)printf("%02x ",Data[i]);
    printf("\n");
    #endif
}

int main()
{
    //Set Handler for transmit data from MODBUS layer
    mb_set_tx_handler(&send_data);

    #if(MB_MODE==MB_MODE_MASTER)

    //Set handler for process recaived packet in master mode
    mb_set_master_process_handler(master_process);

    //Testing Send Packet as Master
    mb_tx_packet_handler(mb_packet_request_read_holding_registers(0x01,0x0000,0x0002));

    //Simulate Recaiving Data in Master Mode
    uint8_t A[]={0x01,0x03,0x04,0x00,0x00,0xff,0xff, 0,0};
    mb_crc_add(A,sizeof(A)-2);
    send_data_to_mb(A,sizeof(A));

    #elif(MODE==MB_MODE_SLAVEV)

    //Simulate Recaiving Data in Slave Mode
    uint8_t A[]={0x01,0x06,0x00,0x01,0xFF,0xFF ,0,0};
    uint8_t B[]={0x01,0x03,0x00,0x00,0x00,0x02 ,0,0};

    mb_crc_add(A,sizeof(A)-2);
    mb_crc_add(B,sizeof(B)-2);

    send_data_to_mb(A,sizeof(A));
    send_data_to_mb(B,sizeof(B));

    #endif

    return 0;
}