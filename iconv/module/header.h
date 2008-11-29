/* Generated by CMunge 0.76 (10 May 2006)
 * CMunge Copyright (c) 1999-2006 Robin Watts/Justin Fletcher */

#ifndef _CMUNGE_Iconv_H_
#define _CMUNGE_Iconv_H_

#include "kernel.h"

#define CMUNGE_VERSION (76)
#define CMHG_VERSION   (531) /* Nearest equivalent version */

#define Module_Title		"Iconv"
#define Module_Help		"Iconv"
#define Module_VersionString	"0.10"
#define Module_VersionNumber	10
#ifndef Module_Date
#define Module_Date		"29 Nov 2008"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/***************************************************************************
 * Function:     mod_init
 * Description:  Initialise the module, setting up vectors, callbacks and
 *               any other parts of the system necessary for the module to
 *               function.
 * Parameters:   tail        = pointer to command line (control terminated)
 *               podule_base = address of podule module was started from, or
 *                             NULL if none
 *               pw          = private word for module
 * On exit:      Return NULL for successful initialisation, or a pointer to
 *               an error block if the module could not start properly.
 **************************************************************************/
_kernel_oserror *mod_init(const char *tail, int podule_base, void *pw);


/***************************************************************************
 * Function:     mod_fini
 * Description:  Finalise the module, shutting down any systems necessary,
 *               freeing vectors and releasing workspace
 * Parameters:   fatal       = fatality indicator; 1 if fatal, 0 if
 *                             reinitialising
 *               podule_base = address of podule module was started from, or
 *                             NULL if none
 *               pw          = private word for module
 * On exit:      Return 0 for successful finalisation, or a pointer to an
 *               error block if module was not shutdown properly.
 **************************************************************************/
_kernel_oserror *mod_fini(int fatal, int podule_base, void *pw);


/***************************************************************************
 * Description:  Star command and help request handler routines.
 * Parameters:   arg_string = pointer to argument string (control
 *                            terminated), or output buffer
 *               argc       = number of arguments passed
 *               number     = command number (see CMD_* definitions below)
 *               pw         = private word for module
 * On exit:      If number indicates a help entry:
 *                 To output, assemble zero terminated output into
 *                 arg_string, and return help_PRINT_BUFFER to print it.
 *                 To stay silent, return NULL.
 *                 To given an error, return an error pointer.
 *                 [In this case, you need to cast the 'const' away]
 *               If number indicates a configure option:
 *                 If arg_string is arg_STATUS, then print status, otherwise
 *                 use argc and arg_string to set option.
 *                 Return NULL for no error.
 *                 Return one of the four error codes below (configure_*)
 *                 for a generic error message.
 *                 Return an error pointer for a custom error.
 *               If number indicates a command entry:
 *                 Execute the command given by number, and arg_string.
 *                 Return NULL on success,
 *                 Return a pointer to an error block on failure.
 **************************************************************************/
_kernel_oserror *command_handler(const char *arg_string, int argc,
                                 int number, void *pw);
#define help_PRINT_BUFFER		((_kernel_oserror *) arg_string)
#define arg_CONFIGURE_SYNTAX		((char *) 0)
#define arg_STATUS			((char *) 1)
#define configure_BAD_OPTION		((_kernel_oserror *) -1)
#define configure_NUMBER_NEEDED		((_kernel_oserror *) 1)
#define configure_TOO_LARGE		((_kernel_oserror *) 2)
#define configure_TOO_MANY_PARAMS	((_kernel_oserror *) 3)

/* Command numbers, as passed to the command handler functions (see above) */
#undef CMD_Iconv
#define CMD_Iconv (0)
#undef CMD_ReadAliases
#define CMD_ReadAliases (1)


/***************************************************************************
 * Description:  SWI handler routine. All SWIs for this module will be
 *               passed to these routines.
 * Parameters:   number = SWI number within SWI chunk (i.e. 0 to 63)
 *               r      = pointer to register block on entry
 *               pw     = private word for module
 * On exit:      Return NULL if SWI handled sucessfully, setting return
 *               register values (r0-r9) in r.
 *               Return error_BAD_SWI for out of range SWIs.
 *               Return an error block for a custom error.
 **************************************************************************/
/* Function called to handle SWI calls */
_kernel_oserror *swi_handler(int number, _kernel_swi_regs *r, void *pw);
/* SWI number definitions */
#define Iconv_00 (0x00057540)
#undef Iconv_Open
#undef XIconv_Open
#define Iconv_Open                (0x00057540)
#define XIconv_Open               (0x00077540)
#undef Iconv_Iconv
#undef XIconv_Iconv
#define Iconv_Iconv               (0x00057541)
#define XIconv_Iconv              (0x00077541)
#undef Iconv_Close
#undef XIconv_Close
#define Iconv_Close               (0x00057542)
#define XIconv_Close              (0x00077542)
#undef Iconv_Convert
#undef XIconv_Convert
#define Iconv_Convert             (0x00057543)
#define XIconv_Convert            (0x00077543)
#undef Iconv_CreateMenu
#undef XIconv_CreateMenu
#define Iconv_CreateMenu          (0x00057544)
#define XIconv_CreateMenu         (0x00077544)
#undef Iconv_DecodeMenu
#undef XIconv_DecodeMenu
#define Iconv_DecodeMenu          (0x00057545)
#define XIconv_DecodeMenu         (0x00077545)

/* Special error for 'SWI values out of range for this module' */
#define error_BAD_SWI ((_kernel_oserror *) -1)

#ifdef __cplusplus
}
#endif

#endif
