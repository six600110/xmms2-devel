/*  XMMS2 - X Music Multiplexer System
 *  Copyright (C) 2003	Peter Alm, Tobias Rundstr�m, Anders Gustafsson
 * 
 *  PLUGINS ARE NOT CONSIDERED TO BE DERIVED WORK !!!
 * 
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *                   
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */




#ifndef __SIGNAL_XMMS_H__
#define __SIGNAL_XMMS_H__

typedef enum {
	XMMS_IPC_OBJECT_MAIN,
	XMMS_IPC_OBJECT_PLAYLIST,
	XMMS_IPC_OBJECT_CONFIG,
	XMMS_IPC_OBJECT_OUTPUT,
	XMMS_IPC_OBJECT_MEDIALIB,
	XMMS_IPC_OBJECT_SIGNAL,
 	XMMS_IPC_OBJECT_VISUALISATION,
	XMMS_IPC_OBJECT_END
} xmms_ipc_objects_t;

typedef enum {
	XMMS_IPC_SIGNAL_OBJECT_DESTROYED = 1,
	XMMS_IPC_SIGNAL_PLAYLIST_CHANGED = 4,
	XMMS_IPC_SIGNAL_CONFIGVALUE_CHANGED = 8,
	XMMS_IPC_SIGNAL_OUTPUT_STATUS = 16,
	XMMS_IPC_SIGNAL_OUTPUT_MIXER_CHANGED = 32,
	XMMS_IPC_SIGNAL_OUTPUT_PLAYTIME = 64,
	XMMS_IPC_SIGNAL_OUTPUT_CURRENTID = 128,
	XMMS_IPC_SIGNAL_OUTPUT_OPEN_FAIL = 256,
	XMMS_IPC_SIGNAL_PLAYLIST_MEDIAINFO_ID = 512,
	XMMS_IPC_SIGNAL_TRANSPORT_MIMETYPE = 1024,
	XMMS_IPC_SIGNAL_DECODER_THREAD_EXIT = 2048, 
	XMMS_IPC_SIGNAL_VISUALISATION_DATA = 4096,
	XMMS_IPC_SIGNAL_END = 8192
} xmms_ipc_signals_t;

typedef enum {
	/* Main */
	XMMS_IPC_CMD_HELLO,
	XMMS_IPC_CMD_QUIT,
	XMMS_IPC_CMD_REPLY,
	XMMS_IPC_CMD_ERROR,

	/* Playlist */
	XMMS_IPC_CMD_SHUFFLE,
	XMMS_IPC_CMD_JUMP,
	XMMS_IPC_CMD_ADD,
	XMMS_IPC_CMD_MEDIALIBADD,
	XMMS_IPC_CMD_GETMEDIAINFO,
	XMMS_IPC_CMD_REMOVE,
	XMMS_IPC_CMD_MOVE,
	XMMS_IPC_CMD_LIST,
	XMMS_IPC_CMD_CLEAR,
	XMMS_IPC_CMD_SORT,
	XMMS_IPC_CMD_SAVE,

	/* Config */
	XMMS_IPC_CMD_GETVALUE,
	XMMS_IPC_CMD_SETVALUE,
	XMMS_IPC_CMD_LISTVALUES,

	/* output */
	XMMS_IPC_CMD_START,
	XMMS_IPC_CMD_STOP,
	XMMS_IPC_CMD_PAUSE,
	XMMS_IPC_CMD_DECODER_KILL,
	XMMS_IPC_CMD_CPLAYTIME,
	XMMS_IPC_CMD_SEEKMS,
	XMMS_IPC_CMD_SEEKSAMPLES,
	XMMS_IPC_CMD_STATUS,
	XMMS_IPC_CMD_CURRENTID,

	/* Medialib */
	XMMS_IPC_CMD_SELECT,
	XMMS_IPC_CMD_PLAYLIST_SAVE_CURRENT,
	XMMS_IPC_CMD_PLAYLIST_LOAD,

	/* Signal subsystem */
	XMMS_IPC_CMD_SIGNAL,
	XMMS_IPC_CMD_BROADCAST,

	/* end */
	XMMS_IPC_CMD_END
} xmms_ipc_cmds_t;

#endif /* __SIGNAL_XMMS_H__ */
