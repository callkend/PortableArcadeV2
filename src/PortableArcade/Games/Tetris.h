

#ifndef TETRIS_H
#define	TETRIS_H

#ifdef	__cplusplus
// extern "C" {
#endif

    MenuResult tetrisSetup(PixiGFX *graphics);

    MenuResult tetrisLoop(PixiGFX *graphics);

#ifdef	__cplusplus
// }
#endif

#endif	/* TETRIS_H */