#include "Crsf.h"

uint8_t RxBuf[CRSF_MAX_PACKET_SIZE];		//Crsf数据包缓冲区
uint8_t RxBuf_Index;										

//uint16_t CRSF_CH[17];
//uint16_t CRSF_CH_LAST[17];

int CrsfChannels[CRSF_NUM_CHANNELS];		//Crsf通道值
CrsfLinkStatistics_t LinkStatistics;		

void HandleByteReceived(void)
{
    bool reprocess;
    do
    {
        reprocess = false;
        if (RxBuf_Index > 1)
        {
            uint8_t len = RxBuf[1];
            // Sanity check the declared length isn't outside Type + X{1,CRSF_MAX_PAYLOAD_LEN} + CRC
            // assumes there never will be a CRSF message that just has a type and no data (X)
            if (len < 3 || len > (CRSF_MAX_PAYLOAD_LEN + 2))
            {
                ShiftRxBuffer(1);
                reprocess = true;
            }
            else if (RxBuf_Index >= (len + 2))
            {
                uint8_t inCrc = RxBuf[2 + len - 1];
                uint8_t crc = Calc(&RxBuf[2], len - 1);
                if (crc == inCrc)
                {
										ProcessPacketIn();
                    ShiftRxBuffer(len + 2);
                    reprocess = true;
                }
                else
                {
                    ShiftRxBuffer(1);
                    reprocess = true;
                }
            }  // if complete packet
        } // if pos > 1
    } while (reprocess);
}

void ShiftRxBuffer(uint8_t cnt)
{
    // If removing the whole thing, just set pos to 0
    if (cnt >= RxBuf_Index)
    {
        RxBuf_Index = 0;
        return;
    }
    // Otherwise do the slow shift down
    uint8_t *src = &RxBuf[cnt];
    uint8_t *dst = &RxBuf[0];
    RxBuf_Index -= cnt;
    uint8_t left = RxBuf_Index;
    while (left--)
        *dst++ = *src++;
}


void ProcessPacketIn(void)													//数据包分类处理
{
    const Crsf_Header_t *hdr = (Crsf_Header_t *)RxBuf;
    if (hdr->device_addr == CRSF_ADDRESS_FLIGHT_CONTROLLER)
    {
        switch (hdr->type)
        {
//        case CRSF_FRAMETYPE_GPS:									//GPS数据
//            packetGps(hdr);
//            break;
        case CRSF_FRAMETYPE_RC_CHANNELS_PACKED:			//遥控器通道值数据
            PacketChannelsPacked(hdr);
            break;
        case CRSF_FRAMETYPE_LINK_STATISTICS:				//连接信息
            PacketLinkStatistics(hdr);
            break;
        }
    } // CRSF_ADDRESS_FLIGHT_CONTROLLER
}

void PacketChannelsPacked(const Crsf_Header_t *p)
{
    crsf_channels_t *ch = (crsf_channels_t *)&p->data;
    CrsfChannels[0] = ch->ch0;
    CrsfChannels[1] = ch->ch1;
    CrsfChannels[2] = ch->ch2;
    CrsfChannels[3] = ch->ch3;
    CrsfChannels[4] = ch->ch4;
    CrsfChannels[5] = ch->ch5;
    CrsfChannels[6] = ch->ch6;
    CrsfChannels[7] = ch->ch7;
    CrsfChannels[8] = ch->ch8;
    CrsfChannels[9] = ch->ch9;
    CrsfChannels[10] = ch->ch10;
    CrsfChannels[11] = ch->ch11;
    CrsfChannels[12] = ch->ch12;
    CrsfChannels[13] = ch->ch13;
    CrsfChannels[14] = ch->ch14;
    CrsfChannels[15] = ch->ch15;
}

void PacketLinkStatistics(const Crsf_Header_t *p)
{
    const CrsfLinkStatistics_t *link = (CrsfLinkStatistics_t *)p->data;
    memcpy(&LinkStatistics, &link, sizeof(LinkStatistics));
		onPacketLinkStatistics(&LinkStatistics);
}


//void Get_Crsf_CH(void)
//{
//		CRSF_CH[1]  = ((int16_t)RxBuf[ 3] >> 0 | ((int16_t)RxBuf[ 4] << 8 )) & 0x07FF;
//		CRSF_CH[2] = ((int16_t)RxBuf[ 4] >> 3 | ((int16_t)RxBuf[ 5] << 5 )) & 0x07FF;
//		CRSF_CH[3]  = ((int16_t)RxBuf[ 5] >> 6 | ((int16_t)RxBuf[ 6] << 2 ) | (int16_t)RxBuf[ 7] << 10 ) & 0x07FF;
//		CRSF_CH[4]  = ((int16_t)RxBuf[ 7] >> 1 | ((int16_t)RxBuf[ 8] << 7 )) & 0x07FF;
//		CRSF_CH[5] = ((int16_t)RxBuf[ 8] >> 4 | ((int16_t)RxBuf[ 9] << 4 )) & 0x07FF;
//		CRSF_CH[6]  = ((int16_t)RxBuf[ 9] >> 7 | ((int16_t)RxBuf[10] << 1 ) | (int16_t)RxBuf[11] << 9 ) & 0x07FF;
//		CRSF_CH[7]  = ((int16_t)RxBuf[11] >> 2 | ((int16_t)RxBuf[12] << 6 )) & 0x07FF;
//		CRSF_CH[8]  = ((int16_t)RxBuf[12] >> 5 | ((int16_t)RxBuf[13] << 3 )) & 0x07FF;
//		CRSF_CH[9]	 = ((int16_t)RxBuf[14] << 0 | ((int16_t)RxBuf[15] << 8 )) & 0x07FF;
//		CRSF_CH[10] = ((int16_t)RxBuf[15] >> 3 | ((int16_t)RxBuf[16] << 5 )) & 0x07FF;
//		CRSF_CH[11] = ((int16_t)RxBuf[16] >> 6 | ((int16_t)RxBuf[17] << 2 ) | (int16_t)RxBuf[18] << 10 ) & 0x07FF;
//		CRSF_CH[12] = ((int16_t)RxBuf[18] >> 1 | ((int16_t)RxBuf[19] << 7 )) & 0x07FF;
//		CRSF_CH[13] = ((int16_t)RxBuf[19] >> 4 | ((int16_t)RxBuf[20] << 4 )) & 0x07FF;
//		CRSF_CH[14] = ((int16_t)RxBuf[20] >> 7 | ((int16_t)RxBuf[21] << 1 ) | (int16_t)RxBuf[22] << 9 ) & 0x07FF;
//		CRSF_CH[15] = ((int16_t)RxBuf[22] >> 2 | ((int16_t)RxBuf[23] << 6 )) & 0x07FF;
//		CRSF_CH[16] = ((int16_t)RxBuf[23] >> 5 | ((int16_t)RxBuf[24] << 3 )) & 0x07FF;				
//	
//		for(int i=1;i<17;i++)
//		{
//				if((CRSF_CH[i]<CRSF_CHANNEL_VALUE_MIN)||((CRSF_CH[i]>CRSF_CHANNEL_VALUE_MAX))&&(CRSF_CH[i]!=0))
//				CRSF_CH[i] = CRSF_CH_LAST[i];				
//				CRSF_CH_LAST[i] = CRSF_CH[i];
//		}
//}

static void onPacketLinkStatistics(CrsfLinkStatistics_t *link)	//回传信号质量，电压...
{
//		printf("Link_quality:%d\n",link->uplink_Link_quality);
}


