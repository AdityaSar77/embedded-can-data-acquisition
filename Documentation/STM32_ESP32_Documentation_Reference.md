# STM32 CAN Data Acquisition — Documentation Reference

_Last verified: October 7, 2026_

This file collects the main documentation used for the STM32 + ESP32 CAN data-acquisition project.

## Project Hardware

- **STM32 board:** NUCLEO-F091RC
- **STM32 MCU:** STM32F091RC
- **ESP32 board:** Adafruit ESP32 Feather V2
- **CAN transceivers:** SN65HVD230
- **ESP32 framework:** ESP-IDF
- **STM32 framework:** STM32Cube HAL

---

# ESP32 / ESP-IDF

## 1. ESP-IDF TWAI Programming Guide

**Main CAN/TWAI reference for the ESP32.**

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html

Use this first when working on:

- Creating and enabling a TWAI node
- TX/RX GPIO configuration
- CAN bit timing and bitrate
- Acceptance filters
- RX callbacks
- Transmitting and receiving CAN frames
- Error handling
- Bus-off recovery
- TWAI node state

> ESP32 documentation calls the CAN controller **TWAI** (Two-Wire Automotive Interface).

---

## 2. `esp_twai.h`

**Primary API header for the new ESP-IDF TWAI driver.**

https://github.com/espressif/esp-idf/blob/master/components/esp_driver_twai/include/esp_twai.h

Useful when you need the exact function declaration, arguments, return codes, and calling restrictions for APIs such as:

- `twai_node_enable()`
- `twai_node_disable()`
- `twai_node_register_event_callbacks()`
- `twai_node_config_mask_filter()`
- `twai_node_receive_from_isr()`
- `twai_node_transmit()`

---

## 3. `esp_twai_types.h`

**TWAI driver type definitions.**

https://github.com/espressif/esp-idf/blob/master/components/esp_driver_twai/include/esp_twai_types.h

Useful for understanding structures and types such as:

- `twai_node_handle_t`
- `twai_frame_t`
- `twai_frame_header_t`
- `twai_event_callbacks_t`
- `twai_mask_filter_config_t`
- Bit-timing configuration structures

This is one of the places to trace fields such as:

```c
rx_frame.header.id
rx_frame.header.dlc
```

---

## 4. `esp_twai_onchip.h`

**Configuration for the ESP32's built-in TWAI controller.**

https://github.com/espressif/esp-idf/blob/master/components/esp_driver_twai/include/esp_twai_onchip.h

Useful for:

- `twai_onchip_node_config_t`
- `twai_new_node_onchip()`
- TX/RX GPIO selection
- Bitrate configuration
- TX queue depth
- Listen-only, loopback, and self-test options

---

## 5. Official TWAI Listen-Only Example

**Useful reference showing the TWAI pieces assembled into a real ESP-IDF application.**

https://github.com/espressif/esp-idf/blob/master/examples/peripherals/twai/twai_network/twai_listen_only/main/twai_listen_only.c

Useful for seeing the expected setup order:

1. Configure node
2. Create node
3. Configure acceptance filter
4. Register callbacks
5. Enable node
6. Receive/process frames

---

## 6. Adafruit ESP32 Feather V2 Pinout

**Board-level GPIO reference.**

https://learn.adafruit.com/adafruit-esp32-feather-v2/pinouts

Relevant pins in the current CAN setup:

- **GPIO4 = A5**
- **GPIO5 = SCK**

Current TWAI mapping:

```text
GPIO4 / A5  -> CAN transceiver TXD
GPIO5 / SCK <- CAN transceiver RXD
```

Do not assume `A4` means GPIO4. On this board, `A4` is GPIO36.

---

## ESP32 API Note

For this project, use the **new ESP-IDF TWAI API**:

```c
#include "esp_twai.h"
#include "esp_twai_onchip.h"
```

Avoid building new project code around the older legacy API:

```c
#include "driver/twai.h"
```

unless you are intentionally reading legacy examples.

---

# STM32 / STM32Cube

## 1. STM32F091RC Product Page

**Central ST page for the exact MCU used by the NUCLEO-F091RC.**

https://www.st.com/en/microcontrollers-microprocessors/stm32f091rc.html

Use this to find:

- Datasheet
- Reference manual
- Errata
- CAD/resources
- MCU features
- Peripheral availability

---

## 2. RM0091 — STM32F0x1 / STM32F0x2 / STM32F0x8 Reference Manual

**Main hardware/peripheral reference for the STM32F091RC.**

https://www.st.com/resource/en/reference_manual/dm00031936.pdf

Important areas for this project include:

- **bxCAN peripheral**
- CAN registers and operating modes
- CAN transmit mailboxes
- CAN receive FIFOs
- CAN acceptance filters
- CAN error handling
- GPIO alternate functions
- ADC
- DMA
- TIM2 and other timers
- RCC / clock configuration
- Interrupts

---

## 3. STM32F091xB / STM32F091xC Datasheet — DS10312

**Electrical characteristics, pins, alternate functions, memory and peripheral summary.**

https://www.st.com/resource/en/datasheet/stm32f091cb.pdf

Use this for:

- Pin mappings
- Alternate functions
- Electrical limits
- Clock limits
- Memory sizes
- Peripheral availability
- Package information

---

## 4. STM32CubeF0 Product Page

**Main software-package page for the STM32F0 HAL and LL drivers.**

https://www.st.com/en/embedded-software/stm32cubef0.html

Useful for:

- STM32F0 HAL/LL documentation
- STM32CubeF0 package information
- Examples
- Driver package information

---

## 5. UM1785 — STM32F0 HAL and Low-Layer Driver Manual

**Main STM32F0 HAL API manual.**

https://www.st.com/resource/en/user_manual/um1785-description-of-stm32f0-hal-and-lowlayer-drivers-stmicroelectronics.pdf

For CAN specifically, this is where to look for HAL-level behavior around functions such as:

```c
HAL_CAN_Start()
HAL_CAN_ConfigFilter()
HAL_CAN_AddTxMessage()
HAL_CAN_GetRxMessage()
HAL_CAN_GetTxMailboxesFreeLevel()
HAL_CAN_GetError()
```

---

## 6. STM32F0 HAL CAN Header

**Exact CAN HAL types, macros and public function declarations.**

https://github.com/STMicroelectronics/stm32f0xx-hal-driver/blob/master/Inc/stm32f0xx_hal_can.h

Use this when you need to inspect:

- `CAN_HandleTypeDef`
- `CAN_TxHeaderTypeDef`
- `CAN_RxHeaderTypeDef`
- `CAN_FilterTypeDef`
- HAL CAN status/error definitions
- Exact function prototypes
- CAN constants and macros

---

## 7. STM32F0 HAL CAN Source

**Implementation of the STM32F0 HAL CAN functions.**

https://github.com/STMicroelectronics/stm32f0xx-hal-driver/blob/master/Src/stm32f0xx_hal_can.c

Use this when you need to know what a HAL function actually does internally.

---

## 8. NUCLEO-F091RC Board Page

**Board-level documentation for the development board itself.**

https://www.st.com/en/evaluation-tools/nucleo-f091rc.html

Useful for:

- Board user manual
- Schematics
- ST-LINK information
- Arduino/ST morpho header mapping
- Board jumpers and solder bridges

The relevant board manual listed by ST is:

**UM1724 — STM32 Nucleo-64 boards (MB1136)**

---

# CAN Transceiver

## SN65HVD230 Datasheet

**Texas Instruments datasheet for the CAN transceiver used on both nodes.**

https://www.ti.com/lit/ds/symlink/sn65hvd230.pdf

Use this for:

- TXD / RXD behavior
- CANH / CANL electrical behavior
- Supply requirements
- Dominant/recessive states
- Propagation delays
- Bus loading
- Transceiver operating modes
- Physical-layer debugging

---

# Where to Look First

| Question | Best reference |
|---|---|
| How do I set up ESP32 CAN/TWAI? | ESP-IDF TWAI Programming Guide |
| What arguments does an ESP32 TWAI function take? | `esp_twai.h` |
| What fields are in a TWAI structure? | `esp_twai_types.h` |
| How do I configure ESP32 GPIOs/bitrate? | `esp_twai_onchip.h` |
| How does Espressif assemble a real receiver? | TWAI listen-only example |
| What Feather pin corresponds to GPIO4/GPIO5? | Adafruit Feather V2 pinout |
| How does the STM32 CAN peripheral work internally? | RM0091 |
| What STM32 pin supports a peripheral/alternate function? | STM32F091 datasheet |
| How do I use a STM32 HAL function? | UM1785 |
| What fields/macros/functions exist in HAL CAN? | `stm32f0xx_hal_can.h` |
| What does a HAL CAN function actually do? | `stm32f0xx_hal_can.c` |
| How is the Nucleo board wired? | NUCLEO-F091RC board page / UM1724 |
| How does the CAN physical layer/transceiver work? | SN65HVD230 datasheet |

---

# Recommended Documentation Workflow

For **ESP32**:

```text
TWAI Programming Guide
-> esp_twai*.h
-> official ESP-IDF examples
```

For **STM32**:

```text
RM0091
-> UM1785
-> stm32f0xx_hal_*.h / .c
-> CubeMX-generated configuration
```

This keeps the design understandable instead of treating HAL or ESP-IDF APIs as magic function calls.
