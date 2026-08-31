# minipad

a 3x3 macropad for shortcuts, built for [hackpad](https://hackpad.hackclub.com).
9 keys, a rotary encoder, per key rgb, and a 0.91" oled that shows a clock and a
spinning globe.

## the pad

![minipad](assets/minipad.png)

rendered from the actual case and board files in this repo, not a mockup. photo
of the real thing goes here once it's printed and soldered.

## schematic

![schematic](assets/schematic.png)

## pcb

![pcb](assets/pcb-3d.png)

96.38 x 90.90mm, 2 layers. chamfered corners, and 4x m3 mounting holes on an
88.226 x 82.748mm rectangle for a sandwich case.

the 9 sk6812mini-e leds are reverse mounted. they sit in milled slots in the
board and shine up through the switches, so the lighting is per key rather than
underglow.

## case

![case](assets/case.png)

[open the assembly in onshape](https://cad.onshape.com/documents/58a01ca66c97d1b4010a7a00/w/fc5b38729478e50a03cca7d8/e/0de70a1a8ca9505f551d6dfe?renderMode=0&uiState=6a9506f6d62aea1f59718b50)

a sandwich: 1.5mm switch plate on top, the pcb in the middle, tray bottom
underneath. 102.38 x 96.90mm overall, 16.6mm tall closed.

the pcb rests on four bosses at 10mm, which leaves a 7.5mm cavity under the
board for the reverse mounted leds and the xiao. m3 heatset inserts drop into
the bosses from above, and four m3 screws through the plate into those inserts
hold the stack together. the only overhang is the usb-c slot in the side wall,
which is a 7mm bridge.

`production/Top.STEP` and `production/Bottom.STEP` are the two printable parts.
`cad/Assembly.STEP` is the whole thing put together, with the pcb in it.

## oled

![oled](assets/oled.gif)

the globe is generated from real natural earth coastline data. 24 frames, made
by projecting the land polygons onto a tilted sphere, so those are actual
continents at 32x32 pixels.

## bom

| qty | part | refs | in the hackpad kit? |
|---|---|---|---|
| 1 | seeed xiao rp2040 | U1 | yes (1 supplied) |
| 9 | mx style switch | SW1-SW9 | yes (16 supplied) |
| 9 | 1n4148 diode, do-35 | D10-D18 | yes (20 supplied) |
| 9 | sk6812mini-e rgb led | D1-D9 | yes (20 supplied) |
| 1 | ec11 rotary encoder, 20mm d-shaft | SW10 | yes (2 supplied) |
| 1 | 0.91" 128x32 i2c oled | U2 | yes (1 supplied) |
| 9 | dsa blank keycap | - | yes (16 supplied) |
| 4 | m3x16 screw | H1-H4 | yes (6 supplied) |
| 4 | m3x5x4 heatset insert | H1-H4 | yes (6 supplied) |
| 1 | knob for the encoder | - | **no, print it or source it yourself** |

everything except the encoder knob comes from the standard kit.

## keymap

| | | |
|---|---|---|
| open spotify | open chrome | open vs code |
| f11 | screenshot | calculator |
| copy | paste | undo |

the encoder does volume.

the three app launchers use windows taskbar shortcuts (`win`+`1`/`2`/`3`), so
**pin spotify, chrome and vs code to the first three taskbar slots** and the top
row works with no extra software.

## clock

there's no rtc on the board and usb hid can't send the time, so the clock
free runs from a timer and you set it by hand:

- hold the two **top corner keys** together to toggle set mode
- **encoder** adjusts minutes
- **top left / top right** keys adjust hours
- **top middle** exits

it resets to 00:00:00 whenever you unplug it, and drifts a few seconds a day.

## pins

| xiao pin | rp2040 | function |
|---|---|---|
| d0, d1, d2 | gp26, gp27, gp28 | matrix columns |
| d3, d7, d10 | gp29, gp1, gp3 | matrix rows |
| d4, d5 | gp6, gp7 | oled i2c (sda, scl) |
| d6 | gp0 | sk6812 data |
| d8, d9 | gp2, gp4 | encoder b, a |

3x3 matrix, `row2col`. all 11 gpio are used, which is why the encoder's push
switch isn't wired.

## firmware

qmk. `production/firmware.uf2` is prebuilt, so to flash it just double tap reset
on the xiao and drag the file onto the drive that appears.

to build it yourself, drop `firmware/` into
`qmk_firmware/keyboards/minipad/` and run:

```
qmk compile -kb minipad -km default
```

## layout

```
pcb/         kicad source and the board step
firmware/    qmk source
cad/         assembled case model
production/  everything you need to build one: gerbers, the two
             printable case parts, and the prebuilt uf2
assets/      images for this readme
```
