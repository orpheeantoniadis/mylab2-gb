#ifndef _LCDC_H
#define _LCDC_H

#define GB_LCDC_WIDTH     160
#define GB_LCDC_HEIGHT    144
#define SCANLINES_NB      153
#define VBLANK_PERIOD     456

#define CONTROL_REG       0xff40
#define STATUS_REG        0xff41
#define SCANLINE_REG      0xff44
#define SCANLINE_CMP_REG  0xff45

void lcdc_cycle(void);

#endif
