/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _IPC_LOGGING_H
#define _IPC_LOGGING_H

#include <mach/msm_ipc_logging.h>

struct ipc_log_page_header {
	uint32_t magic;
	uint32_t nmagic; /* inverse of magic number */
	uint32_t log_id; /* owner of log */
	uint32_t page_num;
	uint16_t read_offset;
	uint16_t write_offset;
	struct list_head list;
};

struct ipc_log_page {
	struct ipc_log_page_header hdr;
	char data[PAGE_SIZE - sizeof(struct ipc_log_page_header)];
};

/**
 * struct ipc_log_context - main logging context
 *
 * @user_version:  Version number for user-defined messages
 * @list:  List of log contexts (struct ipc_log_context)
 * @page_list:  List of log pages (struct ipc_log_page)
 * @first_page:  First page in list of logging pages
 * @last_page:  Last page in list of logging pages
 * @write_page:  Current write page
 * @read_page:  Current read page (for internal reads)
 * @write_avail:  Number of bytes available to write in all pages
 * @dent:  Debugfs node for run-time log extraction
 * @dfunc_info_list:  List of deserialization functions
 * @ipc_log_context_lock:  Lock for entire structure
 * @read_avail:  Completed when new data is added to the log
 */
struct ipc_log_context {
	uint16_t user_version;

	/* add local data structures after this point */
	struct list_head list;
	struct list_head page_list;
	struct ipc_log_page *first_page;
	struct ipc_log_page *last_page;
	struct ipc_log_page *write_page;
	struct ipc_log_page *read_page;
	uint32_t write_avail;
	struct dentry *dent;
	struct list_head dfunc_info_list;
	spinlock_t ipc_log_context_lock;
	struct completion read_avail;
};

struct dfunc_info {
	struct list_head list;
	int type;
	void (*dfunc) (struct encode_context *, struct decode_context *);
};

enum {
	TSV_TYPE_INVALID,
	TSV_TYPE_TIMESTAMP,
	TSV_TYPE_POINTER,
	TSV_TYPE_INT32,
	TSV_TYPE_BYTE_ARRAY,
};

enum {
	OUTPUT_DEBUGFS,
};

#define IPC_LOGGING_MAGIC_NUM 0x52784425
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define IS_MSG_TYPE(x) (((x) > TSV_TYPE_MSG_START) && \
			((x) < TSV_TYPE_MSG_END))
#define MAX_MSG_DECODED_SIZE (MAX_MSG_SIZE*4)

extern rwlock_t ipc_log_context_list_lock;

extern int msg_read(struct ipc_log_context *ilctxt,
		    struct encode_context *ectxt);

static inline int is_ilctxt_empty(struct ipc_log_context *ilctxt)
{
	if (!ilctxt)
		return -EINVAL;

	return ((ilctxt->read_page == ilctxt->write_page) &&
		(ilctxt->read_page->hdr.read_offset ==
		 ilctxt->write_page->hdr.write_offset));
}

#if (defined(CONFIG_DEBUG_FS))
void check_and_create_debugfs(void);

void create_ctx_debugfs(struct ipc_log_context *ctxt,
			const char *mod_name);
#else
void check_and_create_debugfs(void)
{
}

void create_ctx_debugfs(struct ipc_log_context *ctxt, const char *mod_name)
{
}
#endif

#endif
