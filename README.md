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

#### RFID Initialisation
| Length | Request | Checksum | | Length | Response | Checksum |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 02 | 56 | 54 | | 1D | 00 56 36 2E 30 31 2E 35 78 30 31 2D 30 31 2D 30 31 41 42 43 44 00 00 00 31 32 33 34 | 30 |