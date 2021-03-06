#include "mcp_can.h"

const struct MCP_CAN::TXBn_REGS MCP_CAN::TXB[MCP_CAN::N_TXBUFFERS] = {
    {MCP_TXB0CTRL, MCP_TXB0SIDH, MCP_TXB0DATA},
    {MCP_TXB1CTRL, MCP_TXB1SIDH, MCP_TXB1DATA},
    {MCP_TXB2CTRL, MCP_TXB2SIDH, MCP_TXB2DATA}
};

const struct MCP_CAN::RXBn_REGS MCP_CAN::RXB[N_RXBUFFERS] = {
    {MCP_RXB0CTRL, MCP_RXB0SIDH, MCP_RXB0DATA, CANINTF_RX0IF},
    {MCP_RXB1CTRL, MCP_RXB1SIDH, MCP_RXB1DATA, CANINTF_RX1IF}
};

void MCP_CAN::startSPI() {
    SPI.beginTransaction(SPISettings(SPI_CLOCK, MSBFIRST, SPI_MODE0));
    digitalWrite(SPICS, LOW);
}

void MCP_CAN::endSPI() {
    digitalWrite(SPICS, HIGH);
    SPI.endTransaction();
}

/*****************************************************************************
** Function name:           reset
** Descriptions:            reset the device
******************************************************************************/
void MCP_CAN::reset(void)
{
    startSPI();
    SPI.transfer(INSTRUCTION_RESET);
    endSPI();
    delay(10);
}

/*****************************************************************************
** Function name:           readRegister
** Descriptions:            read register
******************************************************************************/
uint8_t MCP_CAN::readRegister(const REGISTER reg)
{
    startSPI();
    SPI.transfer(INSTRUCTION_READ);
    SPI.transfer(reg);
    uint8_t ret = SPI.transfer(0x00);
    endSPI();

    return ret;
}

/*****************************************************************************
** Function name:           readRegisterS
** Descriptions:            read registerS
******************************************************************************/
void MCP_CAN::readRegisterS(const REGISTER reg, uint8_t values[], const uint8_t n)
{
    startSPI();
    SPI.transfer(INSTRUCTION_READ);
    SPI.transfer(reg);
    // mcp2515 has auto-increment of address-pointer
    for (uint8_t i=0; i<n; i++) {
        values[i] = SPI.transfer(0x00);
    }
    endSPI();
}

/*****************************************************************************
** Function name:           setRegister
** Descriptions:            set register
******************************************************************************/
void MCP_CAN::setRegister(const REGISTER reg, const uint8_t value)
{
    startSPI();
    SPI.transfer(INSTRUCTION_WRITE);
    SPI.transfer(reg);
    SPI.transfer(value);
    endSPI();
}

/*****************************************************************************
** Function name:           setRegisterS
** Descriptions:            set registerS
******************************************************************************/
void MCP_CAN::setRegisterS(const REGISTER reg, const uint8_t values[], const uint8_t n)
{
    startSPI();
    SPI.transfer(INSTRUCTION_WRITE);
    SPI.transfer(reg);
    for (uint8_t i=0; i<n; i++) {
        SPI.transfer(values[i]);
    }
    endSPI();
}

/*****************************************************************************
** Function name:           modifyRegister
** Descriptions:            set bit of one register
******************************************************************************/
void MCP_CAN::modifyRegister(const REGISTER reg, const uint8_t mask, const uint8_t data)
{
    startSPI();
    SPI.transfer(INSTRUCTION_BITMOD);
    SPI.transfer(reg);
    SPI.transfer(mask);
    SPI.transfer(data);
    endSPI();
}

/*****************************************************************************
** Function name:           getStatus
** Descriptions:            read mcp2515's Status
******************************************************************************/
uint8_t MCP_CAN::getStatus(void)
{
    startSPI();
    SPI.transfer(INSTRUCTION_READ_STATUS);
    uint8_t i = SPI.transfer(0x00);
    endSPI();

    return i;
}

/*****************************************************************************
** Function name:           setCANCTRL_Mode
** Descriptions:            set control mode
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::setCANCTRL_Mode(const MODE newmode)
{
    modifyRegister(MCP_CANCTRL, MODE_MASK, newmode);

    uint8_t i = readRegister(MCP_CANCTRL) & MODE_MASK;
    return i == newmode ? ERROR_OK : ERROR_FAIL;

}

/*****************************************************************************
** Function name:           configRate
** Descriptions:            set boadrate
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::configRate(const CAN_SPEED canSpeed)
{
    uint8_t set, cfg1, cfg2, cfg3;
    set = 1;
    switch (canSpeed) 
    {
        case (CAN_5KBPS):
        cfg1 = MCP_16MHz_5kBPS_CFG1;
        cfg2 = MCP_16MHz_5kBPS_CFG2;
        cfg3 = MCP_16MHz_5kBPS_CFG3;
        break;

        case (CAN_10KBPS):
        cfg1 = MCP_16MHz_10kBPS_CFG1;
        cfg2 = MCP_16MHz_10kBPS_CFG2;
        cfg3 = MCP_16MHz_10kBPS_CFG3;
        break;

        case (CAN_20KBPS):
        cfg1 = MCP_16MHz_20kBPS_CFG1;
        cfg2 = MCP_16MHz_20kBPS_CFG2;
        cfg3 = MCP_16MHz_20kBPS_CFG3;
        break;
        
        case (CAN_31K25BPS):
        cfg1 = MCP_16MHz_31k25BPS_CFG1;
        cfg2 = MCP_16MHz_31k25BPS_CFG2;
        cfg3 = MCP_16MHz_31k25BPS_CFG3;
        break;

        case (CAN_33KBPS):
        cfg1 = MCP_16MHz_33kBPS_CFG1;
        cfg2 = MCP_16MHz_33kBPS_CFG2;
        cfg3 = MCP_16MHz_33kBPS_CFG3;
        break;

        case (CAN_40KBPS):
        cfg1 = MCP_16MHz_40kBPS_CFG1;
        cfg2 = MCP_16MHz_40kBPS_CFG2;
        cfg3 = MCP_16MHz_40kBPS_CFG3;
        break;

        case (CAN_50KBPS):
        cfg1 = MCP_16MHz_50kBPS_CFG1;
        cfg2 = MCP_16MHz_50kBPS_CFG2;
        cfg3 = MCP_16MHz_50kBPS_CFG3;
        break;

        case (CAN_80KBPS):
        cfg1 = MCP_16MHz_80kBPS_CFG1;
        cfg2 = MCP_16MHz_80kBPS_CFG2;
        cfg3 = MCP_16MHz_80kBPS_CFG3;
        break;

        case (CAN_83K3BPS):
        cfg1 = MCP_16MHz_83k3BPS_CFG1;
        cfg2 = MCP_16MHz_83k3BPS_CFG2;
        cfg3 = MCP_16MHz_83k3BPS_CFG3;
        break;  

        case (CAN_95KBPS):
        cfg1 = MCP_16MHz_95kBPS_CFG1;
        cfg2 = MCP_16MHz_95kBPS_CFG2;
        cfg3 = MCP_16MHz_95kBPS_CFG3;
        break;

        case (CAN_100KBPS):
        cfg1 = MCP_16MHz_100kBPS_CFG1;
        cfg2 = MCP_16MHz_100kBPS_CFG2;
        cfg3 = MCP_16MHz_100kBPS_CFG3;
        break;

        case (CAN_125KBPS):
        cfg1 = MCP_16MHz_125kBPS_CFG1;
        cfg2 = MCP_16MHz_125kBPS_CFG2;
        cfg3 = MCP_16MHz_125kBPS_CFG3;
        break;

        case (CAN_200KBPS):
        cfg1 = MCP_16MHz_200kBPS_CFG1;
        cfg2 = MCP_16MHz_200kBPS_CFG2;
        cfg3 = MCP_16MHz_200kBPS_CFG3;
        break;

        case (CAN_250KBPS):
        cfg1 = MCP_16MHz_250kBPS_CFG1;
        cfg2 = MCP_16MHz_250kBPS_CFG2;
        cfg3 = MCP_16MHz_250kBPS_CFG3;
        break;

        case (CAN_500KBPS):
        cfg1 = MCP_16MHz_500kBPS_CFG1;
        cfg2 = MCP_16MHz_500kBPS_CFG2;
        cfg3 = MCP_16MHz_500kBPS_CFG3;
        break;
        
        case (CAN_1000KBPS):
        cfg1 = MCP_16MHz_1000kBPS_CFG1;
        cfg2 = MCP_16MHz_1000kBPS_CFG2;
        cfg3 = MCP_16MHz_1000kBPS_CFG3;
        break;  

        default:
        set = 0;
        break;
    }

    if (set) {
        setRegister(MCP_CNF1, cfg1);
        setRegister(MCP_CNF2, cfg2);
        setRegister(MCP_CNF3, cfg3);
        return ERROR_OK;
    }
    else {
        return ERROR_FAIL;
    }
}

/*****************************************************************************
** Function name:           init
** Descriptions:            init the device
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::init(const CAN_SPEED canSpeed)
{
    reset();

    ERROR res = setCANCTRL_Mode(MODE_CONFIG);
    delay(10);
    if (res != ERROR_OK) {
        return res;
    }

    res = configRate(canSpeed);
    delay(10);
    if (res != ERROR_OK) {
        return res;
    }

    uint8_t zeros[14];
    memset(zeros, 0, sizeof(zeros));
    setRegisterS(MCP_TXB0CTRL, zeros, 14);
    setRegisterS(MCP_TXB1CTRL, zeros, 14);
    setRegisterS(MCP_TXB2CTRL, zeros, 14);

    setRegister(MCP_RXB0CTRL, 0);
    setRegister(MCP_RXB1CTRL, 0);

    setRegister(MCP_CANINTE, CANINTF_RX0IF | CANINTF_RX1IF);

    modifyRegister(MCP_RXB0CTRL,
                   RXBnCTRL_RXM_MASK | RXB0CTRL_BUKT,
                   RXBnCTRL_RXM_STDEXT | RXB0CTRL_BUKT);
    modifyRegister(MCP_RXB1CTRL, RXBnCTRL_RXM_MASK, RXBnCTRL_RXM_STDEXT);

    res = setCANCTRL_Mode(m_mode);
    delay(10);

    return res;

}

/*****************************************************************************
** Function name:           write_id
** Descriptions:            write can id
******************************************************************************/
void MCP_CAN::write_id(const REGISTER reg, const bool ext, const uint32_t id)
{
    uint8_t tbufdata[4];

    prepareId(tbufdata, ext, id);

    setRegisterS(reg, tbufdata, 4);
}

void MCP_CAN::prepareId(uint8_t *buffer, const bool ext, const uint32_t id)
{
    uint16_t canid = (uint16_t)(id & 0x0FFFF);

    if (ext) {
        buffer[MCP_EID0] = (uint8_t) (canid & 0xFF);
        buffer[MCP_EID8] = (uint8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        buffer[MCP_SIDL] = (uint8_t) (canid & 0x03);
        buffer[MCP_SIDL] += (uint8_t) ((canid & 0x1C) << 3);
        buffer[MCP_SIDL] |= TXB_EXIDE_MASK;
        buffer[MCP_SIDH] = (uint8_t) (canid >> 5);
    } else {
        buffer[MCP_SIDH] = (uint8_t) (canid >> 3);
        buffer[MCP_SIDL] = (uint8_t) ((canid & 0x07 ) << 5);
        buffer[MCP_EID0] = 0;
        buffer[MCP_EID8] = 0;
    }
}

/*****************************************************************************
** Function name:           set CS
** Descriptions:            init CS pin and set UNSELECTED
******************************************************************************/
MCP_CAN::MCP_CAN(const uint8_t _CS, const MODE mode)
{
    m_mode = mode;
    SPICS = _CS;
    pinMode(SPICS, OUTPUT);
    endSPI();
}

/*****************************************************************************
** Function name:           init
** Descriptions:            init can and set speed
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::begin(const CAN_SPEED speedset)
{
    SPI.begin();
    return init(speedset);
}

/*****************************************************************************
** Function name:           initMask
** Descriptions:            init canid Masks
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::initMask(const uint8_t num, const bool ext, const uint32_t ulData)
{
    delay(10);

    ERROR res = setCANCTRL_Mode(MODE_CONFIG);
    if (res != ERROR_OK){
        delay(10);
        return res;
    }
    
    if (num == 0) {
        write_id(MCP_RXM0SIDH, ext, ulData);
    } else if (num == 1){
        write_id(MCP_RXM1SIDH, ext, ulData);
    } else {
        //res = ERROR_FAIL;
    }
    
    res = setCANCTRL_Mode(m_mode);
    delay(10);
    return res;
}

/******************************************************************************
** Function name:           initFilt
** Descriptions:            init canid filters
******************************************************************************/
MCP_CAN::ERROR MCP_CAN::initFilt(const RXF num, const bool ext, const uint32_t ulData)
{
    delay(10);
    ERROR res = setCANCTRL_Mode(MODE_CONFIG);
    if (res != ERROR_OK) {
        delay(10);
        return res;
    }
    
    switch (num) {
        case RXF0:
            write_id(MCP_RXF0SIDH, ext, ulData);
            break;

        case RXF1:
            write_id(MCP_RXF1SIDH, ext, ulData);
            break;

        case RXF2:
            write_id(MCP_RXF2SIDH, ext, ulData);
            break;

        case RXF3:
            write_id(MCP_RXF3SIDH, ext, ulData);
            break;

        case RXF4:
            write_id(MCP_RXF4SIDH, ext, ulData);
            break;

        case RXF5:
            write_id(MCP_RXF5SIDH, ext, ulData);
            break;

        default:
            return ERROR_FAIL;
    }
    
    res = setCANCTRL_Mode(m_mode);
    delay(10);
    return res;
}

MCP_CAN::ERROR MCP_CAN::sendMessage(const TXBn txbn, const struct can_frame *frame)
{
    const struct TXBn_REGS *txbuf = &TXB[txbn];

    uint8_t data[13];

    bool ext = frame->can_id & CAN_EFF_FLAG;
    bool rtr = frame->can_id & CAN_RTR_FLAG;
    uint32_t id = frame->can_id & (ext ? CAN_EFF_MASK : CAN_SFF_MASK);

    prepareId(data, ext, id);

    data[MCP_DLC] = rtr ? frame->can_dlc | RTR_MASK : frame->can_dlc;

    memcpy(&data[MCP_DATA], frame->data, frame->can_dlc);

    setRegisterS(txbuf->SIDH, data, 5 + frame->can_dlc);

    modifyRegister(txbuf->CTRL, TXB_TXREQ, TXB_TXREQ);

    return ERROR_OK;
}

MCP_CAN::ERROR MCP_CAN::sendMessage(const struct can_frame *frame)
{
    if (frame->can_dlc > CAN_MAX_DLEN) {
        return ERROR_FAILTX;
    }

    TXBn txBuffers[N_TXBUFFERS] = {TXB0, TXB1, TXB2};

    for (int i=0; i<N_TXBUFFERS; i++) {
        const struct TXBn_REGS *txbuf = &TXB[txBuffers[i]];
        uint8_t ctrlval = readRegister(txbuf->CTRL);
        if ( (ctrlval & TXB_TXREQ) == 0 ) {
            return sendMessage(txBuffers[i], frame);
        }
    }

    return ERROR_FAILTX;
}

//MCP_CAN::ERROR MCP_CAN::readMessage(const RXBn rxbn, uint32_t *id, uint8_t *dlc, uint8_t buf[], bool *rtr, bool *ext)
MCP_CAN::ERROR MCP_CAN::readMessage(const RXBn rxbn, struct can_frame *frame)
{
    const struct RXBn_REGS *rxb = &RXB[rxbn];

    uint8_t tbufdata[5];

    readRegisterS(rxb->SIDH, tbufdata, 5);

    uint32_t id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & TXB_EXIDE_MASK) ==  TXB_EXIDE_MASK ) {
        id = (id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        id = (id<<8) + tbufdata[MCP_EID8];
        id = (id<<8) + tbufdata[MCP_EID0];
        id |= CAN_EFF_FLAG;
    }

    uint8_t dlc = tbufdata[MCP_DLC] & DLC_MASK;
    if (dlc > CAN_MAX_DLEN) {
        return ERROR_FAIL;
    }

    uint8_t ctrl = readRegister(rxb->CTRL);
    if (ctrl & RXBnCTRL_RTR) {
        id |= CAN_RTR_FLAG;
    }

    frame->can_id = id;
    frame->can_dlc = dlc;

    readRegisterS(rxb->DATA, frame->data, dlc);

    modifyRegister(MCP_CANINTF, rxb->CANINTF_RXnIF, 0);

    return ERROR_OK;
}

MCP_CAN::ERROR MCP_CAN::readMessage(struct can_frame *frame)
{
    ERROR rc;
    uint8_t stat = getStatus();

    if ( stat & STAT_RX0IF ) {
        rc = readMessage(RXB0, frame);
    } else if ( stat & STAT_RX1IF ) {
        rc = readMessage(RXB1, frame);
    } else {
        rc = ERROR_NOMSG;
    }

    return rc;
}

/*****************************************************************************
** Function name:           checkReceive
** Descriptions:            check if got something
******************************************************************************/
bool MCP_CAN::checkReceive(void)
{
    uint8_t res = getStatus();
    if ( res & STAT_RXIF_MASK ) {
        return true;
    } else {
        return false;
    }
}

/*****************************************************************************
** Function name:           checkError
** Descriptions:            if something error
******************************************************************************/
bool MCP_CAN::checkError(void)
{
    uint8_t eflg = readRegister(MCP_EFLG);

    if ( eflg & EFLG_ERRORMASK ) {
        return true;
    } else {
        return false;
    }
}


uint8_t MCP_CAN::getInterrupts(void)
{
    return readRegister(MCP_CANINTF);
}

void MCP_CAN::clearInterrupts(void)
{
    setRegister(MCP_CANINTF, 0);
}

uint8_t MCP_CAN::getInterruptMask(void)
{
    return readRegister(MCP_CANINTE);
}

void MCP_CAN::clearTXInterrupts(void)
{
    modifyRegister(MCP_CANINTF, (CANINTF_TX0IF | CANINTF_TX1IF | CANINTF_TX2IF), 0);
}
