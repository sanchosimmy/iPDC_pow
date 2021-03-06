/* ----------------------------------------------------------------------------- 
 * recreate.h
 *
 * iPDC - Phasor Data Concentrator
 *
 * Copyright (C) 2011-2012 Nitesh Pandit
 * Copyright (C) 2011-2012 Kedar V. Khandeparkar
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Authors: 
 *		Nitesh Pandit <panditnitesh@gmail.com>
 *		Kedar V. Khandeparkar <kedar.khandeparkar@gmail.com>			
 *		Sancho Simmy Louis <sanchoatiitb@gmail.com>
 * ----------------------------------------------------------------------------- */


/* ------------------------------------------------------------------ */
/*                       Function prototypes                          */
/* ------------------------------------------------------------------ */

void recreate_cfg_objects();

void init_cfgparser(unsigned char []); 

void recreate_Connection_Table();

void recreate_udp_connections(struct Lower_Layer_Details *t11);

void recreate_tcp_connections(struct Lower_Layer_Details *t12);

void recreate_mul_connections(struct Lower_Layer_Details *t11);

/**************************************** End of File *******************************************************/
