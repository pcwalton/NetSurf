/*
 * Copyright 2010 Ole Loots <ole@monochrom.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windom.h>
#include <assert.h>
#include <math.h>

#include "utils/log.h"
#include "desktop/gui.h"
#include "desktop/history_core.h"
#include "desktop/netsurf.h"
#include "desktop/browser.h"
#include "desktop/mouse.h"
#include "desktop/plotters.h"

#include "atari/gui.h"
#include "atari/statusbar.h"
#include "atari/browser_win.h"
#include "atari/misc.h"
#include "atari/global_evnt.h"
#include "atari/res/netsurf.rsh"
#include "atari/plot/plotter.h"

extern short vdih;

static
void __CDECL evnt_sb_redraw( COMPONENT *c, long buff[8] )
{
	struct gui_window * gw = (struct gui_window *)mt_CompDataSearch(&app, c, CDT_OWNER);
	CMP_STATUSBAR sb = gw->root->statusbar;
	LGRECT work, lclip;
	short pxy[8], d, pxyclip[4];

	mt_CompGetLGrect(&app, sb->comp, WF_WORKXYWH, &work);
	lclip = work;
	if ( !rc_lintersect( (LGRECT*)&buff[4], &lclip ) ) return;

	vsf_interior( vdih, FIS_SOLID );
	vsl_color( vdih, BLACK );
	vsl_type( vdih, 1);
	vsl_width( vdih, 1 );
	vst_color(vdih, BLACK);
	vst_height( vdih, 10, &pxy[0], &pxy[1], &pxy[2], &pxy[3] );
	vst_arbpt( vdih, 9, &pxy[0], &pxy[1], &pxy[2], &pxy[3] );
	vst_alignment(vdih, 0, 5, &d, &d );
	vst_effects( vdih, 0 );

	pxyclip[0] = lclip.g_x;
	pxyclip[1] = lclip.g_y;
	pxyclip[2] = lclip.g_x + lclip.g_w;
	pxyclip[3] = lclip.g_y + lclip.g_h;
	vs_clip(vdih, 1, (short*)&pxyclip );
	vswr_mode( vdih, MD_REPLACE );

	if( lclip.g_y <= work.g_y ) {
		pxy[0] = work.g_x;
		pxy[1] = work.g_y;
		pxy[2] = MIN( work.g_x + work.g_w, lclip.g_x + lclip.g_w );
		pxy[3] = work.g_y;
		v_pline( vdih, 2, (short*)&pxy );
	}

	vsf_color( vdih, LWHITE);
	pxy[0] = work.g_x;
	pxy[1] = work.g_y+1;
	pxy[2] = work.g_x + work.g_w;
	pxy[3] = work.g_y + work.g_h-1;
	v_bar( vdih, pxy );

	vswr_mode( vdih, MD_TRANS );
	v_gtext( vdih, work.g_x + 2, work.g_y + 5, (char*)&sb->text );

	vswr_mode( vdih, MD_REPLACE );

	pxy[0] = work.g_x + work.g_w - MOVER_WH;
	pxy[1] = work.g_y + 1;
	pxy[2] = work.g_x + work.g_w;
	pxy[3] = work.g_y + work.g_h-1;
	v_bar( vdih, pxy );

	pxy[0] = work.g_x + work.g_w - MOVER_WH;
	pxy[1] = work.g_y + work.g_h;
	pxy[2] = work.g_x + work.g_w - MOVER_WH;
	pxy[3] = work.g_y + work.g_h - MOVER_WH;
	v_pline( vdih, 2, (short*)&pxy );	

	vs_clip(vdih, 0, (short*)&pxyclip );
	
}

static void __CDECL evnt_sb_click( COMPONENT *c, long buff[8] )
{
	short sbuff[8], mx, my;
	LGRECT work;
	mt_CompGetLGrect(&app, c, WF_WORKXYWH, &work);
	if( evnt.mx >= work.g_x + (work.g_w - MOVER_WH) && evnt.mx <= work.g_x + work.g_w &&
		evnt.my >= work.g_y + (work.g_h - MOVER_WH) && evnt.my <= work.g_y + work.g_h ) {
		/* click into the mover region */
		struct gui_window * g;
		for( g = window_list; g; g=g->next ) {
			if( g->root->statusbar->comp == c ) {
				sbuff[0] = WM_SIZED;
				sbuff[1] = (short)buff[0];
				sbuff[2] = 0;
				sbuff[3] = g->root->handle->handle;
				sbuff[4] = g->root->loc.g_x;
				sbuff[5] = g->root->loc.g_y;
				sbuff[6] = g->root->loc.g_w;
				sbuff[7] = g->root->loc.g_h;
				evnt_window_resize( g->root->handle, sbuff );
			}
		}
	}
}

CMP_STATUSBAR sb_create( struct gui_window * gw )
{
	CMP_STATUSBAR s = malloc( sizeof(struct s_statusbar) );
	s->comp = (COMPONENT*)mt_CompCreate(&app, CLT_HORIZONTAL, STATUSBAR_HEIGHT, 0);
	s->comp->rect.g_h = STATUSBAR_HEIGHT;
	s->comp->bounds.max_height = STATUSBAR_HEIGHT;
	mt_CompDataAttach( &app, s->comp, CDT_OWNER, gw );
	mt_CompEvntAttach( &app, s->comp, WM_REDRAW, evnt_sb_redraw );
	mt_CompEvntAttach( &app, s->comp, WM_XBUTTON, evnt_sb_click );
	strncpy( (char*)&s->text, "  ", 254 );
	return( s );
}

void sb_destroy( CMP_STATUSBAR s )
{
	LOG(("%s\n", __FUNCTION__ ));
	if( s ) {
		if( s->comp ){
			mt_CompDelete( &app, s->comp );
		}
		free( s );
	}
}

void sb_set_text( struct gui_window * gw , char * text )
{

	if( gw->root == NULL )
		return;
	CMP_STATUSBAR sb = gw->root->statusbar;
	LGRECT work;

	if( sb == NULL || gw->browser->attached == false )
		return;

	strncpy( (char*)&sb->text, text, 254 );
	mt_CompGetLGrect(&app, sb->comp, WF_WORKXYWH, &work);
	ApplWrite( _AESapid, WM_REDRAW,  gw->root->handle->handle,
		work.g_x, work.g_y, work.g_w, work.g_h );
}