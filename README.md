SOMB - Stratasys Objet Material Bypass
-----------

This is software to allow the use of expired & non-supported PolyJet materials on Objet (any maybe other Stratasys) 3D printers.

You may use this code to emulate supported material types whilst using unsupported cartridges. e.g. SUP706 on an Objet30 V2.

## Vectors
### Arduino
* *Work in progress.*

### VB.NET
* *Work in progress.*

## Mode of operation.

The device I have access to (and all testing is based off) is a Objet30 Orthodesk. Please be aware that key differences do exist in various models.

### Example Packet Structure

| Length | Payload | Checksum |
| :---: | :---: | :---: |
| 06 | 00 01 00 C1 02 | C4 |

**Length:**
Payload + Checksum (00 01 00 C1 02 C4 = 06).

**Payload:**
Generally reversed (00 01 00 C1 02 = 02 C1 00 01 00).

**Checksum:**
XOR (06 00 01 00 C1 02 = C4).

#### 01. RFID Initialisation
| Length | Request | Checksum | | Length | Response | Checksum |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 02 | 56 | 54 | | 1D | 00 56 36 2E 30 31 2E 35 78 30 31 2D 30 31 2D 30 31 41 42 43 44 00 00 00 31 32 33 34 | 30 |
| | | | | | V6.01.5x01-01-01ABCD   1234 | |

#### 02. Cartridge Selection *(All following commands are based on "selected" cartridge)*
| Length | Request | Checksum | | Length | Response | Checksum | Note |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 03 | AF 04 | A8 | | 02 | 00 | 02 | L-Material |
| 03 | AF 08 | A4 | | | | | R-Material |
| 03 | AF 0C | A0 | | | | | L-Support |
| 03 | AF 00 | AC | | | | | R-Support |

#### 03. Request Cartridge Details
| Length | Request | Checksum | | Length | Response | Checksum |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
03 | 79 01 | 7B | | 07 | 00 **B0 C9 BD 90** 00 | 53 |

#### 04. Request Cartridge Presence *(Note response from #03)*
| Length | Request | Checksum | | Length | Response | Checksum | Note |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 07 | 7A **B0 C9 BD 90** 01 | 28 | | 06 | 00 CA 00 00 AA | 66 | Present |
| | | | | 02 | FD | FF | Missing |

#### 05. Request Material Type
| Length | Request | Checksum | | Length | Response | Checksum | Note |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 04 | 50 00 08 | 5C | | 06 | 00 01 00 C1 02 | C4 | 705 (SUP705) |
| | | | | 06 | 00 01 00 C2 02 | C7 | 706 (SUP706) |
| | | | | 06 | 00 01 00 62 02 | 67 | 610 (MED610) |


#### 06. Request Expiry Date
| Length | Request | Checksum | | Length | Response | Checksum | Note |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 04 | 50 00 0D | 59 | | 06 | 00 80 4E 96 5E | 00 | 5E 96 4E 80 = 15/04/2020 |
| | | | | 06 | 00 00 BA E6 5E | 04 | 5E E6 BA 00 = 15/06/2020 |
| | | | | 06 | 00 00 BA E6 5E | 59 | 5D F5 77 80 = 15/12/2019 |
| | | | | 06 | 00 00 BA E6 5E | FC | 5D CD EA 80 = 15/11/2019 |