#include "app_threadx.h"

static void TxSpeedConf(FDCAN_TxHeaderTypeDef *TxHeader);
static void TxBatteryConf(FDCAN_TxHeaderTypeDef *TxHeader);
static void TxHeartbeatConf(FDCAN_TxHeaderTypeDef *TxHeader);

// Configuration of CAN frame for speed data
void initCanFrames(t_canFrames *canFrames) 
{
    TxSpeedConf(&canFrames->tx_header_speed);
    TxBatteryConf(&canFrames->tx_header_battery);
    TxHeartbeatConf(&canFrames->tx_header_heartbeat);
}

static void    TxSpeedConf(FDCAN_TxHeaderTypeDef *TxHeader) 
{
    TxHeader->Identifier = 0x200;
    TxHeader->IdType = FDCAN_STANDARD_ID;
    TxHeader->TxFrameType = FDCAN_DATA_FRAME;
    TxHeader->DataLength = FDCAN_DLC_BYTES_8;
    TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader->FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader->MessageMarker = 0x0;
}

static void    TxBatteryConf(FDCAN_TxHeaderTypeDef *TxHeader) 
{
    TxHeader->Identifier = 0x201;
    TxHeader->IdType = FDCAN_STANDARD_ID;
    TxHeader->TxFrameType = FDCAN_DATA_FRAME;
    TxHeader->DataLength = FDCAN_DLC_BYTES_8;
    TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader->FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader->MessageMarker = 0x0;
}

static void    TxHeartbeatConf(FDCAN_TxHeaderTypeDef *TxHeader) 
{
    TxHeader->Identifier = 0x202;
    TxHeader->IdType = FDCAN_STANDARD_ID;
    TxHeader->TxFrameType = FDCAN_DATA_FRAME;
    TxHeader->DataLength = FDCAN_DLC_BYTES_8;
    TxHeader->ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader->BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader->FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader->TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader->MessageMarker = 0x0;
}