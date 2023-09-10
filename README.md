# STM32_ToCrsf-Elrs
STM32HAL库解析CRSF(ELRS)协议

## 串口配置

波特率|数据位|校验位|停止位
------|------|------|------
420000|8|无|1

### CRSF协议帧结构
1. **设备地址**（Device Address）:**1字节**
2. **帧长度**（Length）：**1字节**
3. **帧类型**（Type）:**1字节（GPS、RC_channels、LINK_STATISTICS）**
4. **负载（数据）**（Payload）：**根据类型决定（例如RC_channels为22字节）**
5. **CRC校验**（CRC）：**1字节**
   
- **CRSF协议帧结构**
```
 typedef enum
 {
    CRSF_ADDRESS_BROADCAST = 0x00,
    CRSF_ADDRESS_USB = 0x10,
    CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
    CRSF_ADDRESS_RESERVED1 = 0x8A,
    CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
    CRSF_ADDRESS_GPS = 0xC2,
    CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
    CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
    CRSF_ADDRESS_RESERVED2 = 0xCA,
    CRSF_ADDRESS_RACE_TAG = 0xCC,
    CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
    CRSF_ADDRESS_CRSF_RECEIVER = 0xEC,
    CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
} crsf_addr_e;
```
