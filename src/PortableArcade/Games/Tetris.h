

#ifndef TETRIS_H
#define	TETRIS_H

#ifdef	__cplusplus
// extern "C" {
#endif

    MenuReturn tetrisSetup(PixiGFX *graphics);

    MenuReturn tetrisLoop(PixiGFX *graphics);

#ifdef	__cplusplus
// }
#endif

#endif	/* TETRIS_H */