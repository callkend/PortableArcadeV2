/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.

#ifndef PIXI_PUSHER_H
#include "../PixiPusher/PixiGFX.h"
#endif

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_MENU_NAME_LENGTH 12
    
    typedef void (*MenuSetupHandle)(void);
    typedef int (*MenuLoopHandle)(void);

    typedef struct Menu {
        const char Name[MAX_MENU_NAME_LENGTH];

        int ActiveSubMenuIndex;

        struct Menu *SubMenus;

        struct Menu *ParentMenu;

        MenuLoopHandle Loop;
        MenuSetupHandle Setup;
        
    } Menu_t;

    typedef struct MenuState {
        Menu_t *ActiveMenu;
        MenuLoopHandle ActiveLoop;
    } MenuState_t;

#define DEFINE_MENU(name, subMenus) \
{                                   \
    .Name = name,                   \
    .ActiveSubMenuIndex = 0,        \
    .SubMenus = subMenus,           \
    .ParentMenu = NULL,             \
    .Setup = NULL,                  \
    .Loop = NULL,                   \
}
    
#define DEFINE_EMPTY_MENU()         \
{                                   \
    .Name = "",                     \
    .ActiveSubMenuIndex = 0,        \
    .SubMenus = NULL,               \
    .ParentMenu = NULL,             \
    .Setup = NULL,                  \
    .Loop = NULL,                   \
}

#define DEFINE_MENU_FUNCTION(name, startup, loop) \
{                                                 \
    .Name = "",                                   \
    .ActiveSubMenuIndex = 0,                      \
    .SubMenus = NULL,                             \
    .ParentMenu = NULL,                           \
    .Setup = setup,                               \
    .Loop = loop,                                 \
}
    
void ProcessMenus(MenuState_t *state, PixiGFX *graphics);

void RenderMenu(Menu_t *menu, PixiGFX *graphics);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

