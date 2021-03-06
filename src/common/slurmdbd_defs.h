/****************************************************************************\
 *  slurmdbd_defs.h - definitions used for Slurm DBD RPCs
 *****************************************************************************
 *  Copyright (C) 2008-2010 Lawrence Livermore National Security.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>
 *  CODE-OCEC-09-009. All rights reserved.
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.schedmd.com/slurmdocs/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _SLURMDBD_DEFS_H
#define _SLURMDBD_DEFS_H

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif			/* HAVE_INTTYPES_H */
#else				/* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif				/*  HAVE_CONFIG_H */

#include "slurm/slurm.h"

#include "src/common/pack.h"
#include "src/common/list.h"
#include "src/common/slurm_accounting_storage.h"

/*
 * SLURMDBD_VERSION is the version of the slurmdbd protocol currently
 *	being used (i.e. this code). Increment this value whenever an
 *	RPC is modified. When modifing an existing RPC copy the old
 *	version and the create a new pack/unpack for the new rpc_version.
 *
 * SLURMDBD_VERSION_MIN is the minimum protocol version which slurmdbd
 *	will accept. Messages being sent to the slurmdbd from commands
 *	or daemons using older versions of the protocol will be
 *	rejected. Increment this value and discard the code processing
 *	that msg_type only after all systems have been upgraded. Don't
 *	remove entries from slurmdbd_msg_type_t or the numbering scheme
 *	will break (the enum value of a msg_type would change).
 *
 * The slurmdbd should be at least as current as any Slurm cluster
 *	communicating with it (e.g. it will not accept messages with a
 *	version higher than SLURMDBD_VERSION).
 */
#define SLURMDBD_VERSION	10 /* already changed for 2.4 */
#define SLURMDBD_VERSION_MIN	7

/* SLURM DBD message types */
/* ANY TIME YOU ADD TO THIS LIST UPDATE THE CONVERSION FUNCTIONS! */
typedef enum {
	DBD_INIT = 1400,	/* Connection initialization		*/
	DBD_FINI,       	/* Connection finalization		*/
	DBD_ADD_ACCOUNTS,       /* Add new account to the mix           */
	DBD_ADD_ACCOUNT_COORDS, /* Add new coordinatior to an account   */
	DBD_ADD_ASSOCS,         /* Add new association to the mix       */
	DBD_ADD_CLUSTERS,       /* Add new cluster to the mix           */
	DBD_ADD_USERS,          /* Add new user to the mix              */
	DBD_CLUSTER_CPUS,	/* Record total processors on cluster	*/
	DBD_FLUSH_JOBS, 	/* End jobs that are still running
				 * when a controller is restarted.	*/
	DBD_GET_ACCOUNTS,	/* Get account information		*/
	DBD_GET_ASSOCS,         /* Get assocation information   	*/
	DBD_GET_ASSOC_USAGE,  	/* Get assoc usage information  	*/
	DBD_GET_CLUSTERS,	/* Get account information		*/
	DBD_GET_CLUSTER_USAGE, 	/* Get cluster usage information	*/
	DBD_GET_JOBS,		/* VESTIGIAL / DEFUNCT RPC		*/
	DBD_GET_USERS,  	/* Get account information		*/
	DBD_GOT_ACCOUNTS,	/* Response to DBD_GET_ACCOUNTS		*/
	DBD_GOT_ASSOCS, 	/* Response to DBD_GET_ASSOCS   	*/
	DBD_GOT_ASSOC_USAGE,  	/* Response to DBD_GET_ASSOC_USAGE    	*/
	DBD_GOT_CLUSTERS,	/* Response to DBD_GET_CLUSTERS		*/
	DBD_GOT_CLUSTER_USAGE, 	/* Response to DBD_GET_CLUSTER_USAGE   	*/
	DBD_GOT_JOBS,		/* Response to DBD_GET_JOBS		*/
	DBD_GOT_LIST,           /* Response to DBD_MODIFY/REMOVE MOVE_* */
	DBD_GOT_USERS,  	/* Response to DBD_GET_USERS		*/
	DBD_JOB_COMPLETE,	/* Record job completion 		*/
	DBD_JOB_START,		/* Record job starting			*/
	DBD_ID_RC,	        /* return db_index from job
				 * insertion, or any other id from
				 * other commands.              	*/
	DBD_JOB_SUSPEND,	/* Record job suspension		*/
	DBD_MODIFY_ACCOUNTS,    /* Modify existing account              */
	DBD_MODIFY_ASSOCS,      /* Modify existing association          */
	DBD_MODIFY_CLUSTERS,    /* Modify existing cluster              */
	DBD_MODIFY_USERS,       /* Modify existing user                 */
	DBD_NODE_STATE,		/* Record node state transition		*/
	DBD_RC,			/* Return code from operation		*/
	DBD_REGISTER_CTLD,	/* Register a slurmctld's comm port	*/
	DBD_REMOVE_ACCOUNTS,    /* Remove existing account              */
	DBD_REMOVE_ACCOUNT_COORDS,/* Remove existing coordinator from
				   * an account */
	DBD_REMOVE_ASSOCS,      /* Remove existing association          */
	DBD_REMOVE_CLUSTERS,    /* Remove existing cluster              */
	DBD_REMOVE_USERS,       /* Remove existing user                 */
	DBD_ROLL_USAGE,         /* Roll up usage                        */
	DBD_STEP_COMPLETE,	/* Record step completion		*/
	DBD_STEP_START,		/* Record step starting			*/
	DBD_UPDATE_SHARES_USED,	/* Doesn't do anything but
				 * needs to be here for
				 * history sake		*/
	DBD_GET_JOBS_COND, 	/* Get job information with a condition */
	DBD_GET_TXN,		/* Get transaction information		*/
	DBD_GOT_TXN,		/* Got transaction information		*/
	DBD_ADD_QOS,		/* Add QOS information   	        */
	DBD_GET_QOS,		/* Get QOS information   	        */
	DBD_GOT_QOS,		/* Got QOS information   	        */
	DBD_REMOVE_QOS,		/* Remove QOS information   	        */
	DBD_MODIFY_QOS,         /* Modify existing QOS                  */
	DBD_ADD_WCKEYS,		/* Add WCKEY information   	        */
	DBD_GET_WCKEYS,		/* Get WCKEY information   	        */
	DBD_GOT_WCKEYS,		/* Got WCKEY information   	        */
	DBD_REMOVE_WCKEYS,	/* Remove WCKEY information   	        */
	DBD_MODIFY_WCKEYS,      /* Modify existing WCKEY                */
	DBD_GET_WCKEY_USAGE,  	/* Get wckey usage information  	*/
	DBD_GOT_WCKEY_USAGE,  	/* Get wckey usage information  	*/
	DBD_ARCHIVE_DUMP,    	/* issue a request to dump jobs to
				 * archive */
	DBD_ARCHIVE_LOAD,    	/* load an archive file    	        */
	DBD_ADD_RESV,    	/* add a reservation                    */
	DBD_REMOVE_RESV,    	/* remove a reservation                 */
	DBD_MODIFY_RESV,    	/* modify a reservation                 */
	DBD_GET_RESVS,    	/* Get reservation information  	*/
	DBD_GOT_RESVS,		/* Response to DBD_GET_RESV		*/
	DBD_GET_CONFIG,  	/* Get configuration information	*/
	DBD_GOT_CONFIG,		/* Response to DBD_GET_CONFIG		*/
	DBD_GET_PROBS,  	/* Get problems existing in accounting	*/
	DBD_GOT_PROBS,		/* Response to DBD_GET_PROBS		*/
	DBD_GET_EVENTS, 	/* Get event information		*/
	DBD_GOT_EVENTS, 	/* Response to DBD_GET_EVENTS		*/
	DBD_SEND_MULT_JOB_START,/* Send multiple job starts		*/
	DBD_GOT_MULT_JOB_START,	/* Get response to DBD_SEND_MULT_JOB_START */
	DBD_SEND_MULT_MSG,      /* Send multiple message		*/
	DBD_GOT_MULT_MSG,	/* Get response to DBD_SEND_MULT_MSG    */
	DBD_MODIFY_JOB		/* Modify existing Job(s)               */
} slurmdbd_msg_type_t;

/*****************************************************************************\
 * Slurm DBD protocol data structures
\*****************************************************************************/

typedef struct slurmdbd_msg {
	uint16_t msg_type;	/* see slurmdbd_msg_type_t above */
	void * data;		/* pointer to a message type below */
} slurmdbd_msg_t;

typedef struct {
	List acct_list; /* list of account names (char *'s) */
	slurmdb_user_cond_t *cond;
} dbd_acct_coord_msg_t;

typedef struct dbd_cluster_cpus_msg {
	char *cluster_nodes;	/* nodes in cluster */
	uint32_t cpu_count;	/* total processor count */
	time_t event_time;	/* time of transition */
} dbd_cluster_cpus_msg_t;

typedef struct {
	void *rec; /* this could be anything based on the type types
		    * are defined in slurm_accounting_storage.h
		    * *_rec_t */
} dbd_rec_msg_t;

typedef struct {
	void *cond; /* this could be anything based on the type types
		     * are defined in slurm_accounting_storage.h
		     * *_cond_t */
} dbd_cond_msg_t;

typedef struct {
	uint16_t archive_data;
	time_t end;
	time_t start;
} dbd_roll_usage_msg_t;

typedef struct {
	time_t end;
	void *rec;
	time_t start;
} dbd_usage_msg_t;

typedef struct dbd_get_jobs_msg {
	char *cluster_name;	/* name of cluster to query */
	uint16_t completion;	/* get job completion records instead
				 * of accounting record */
	uint32_t gid;		/* group id */
	time_t last_update;	/* time of latest info */
	List selected_steps;	/* List of jobacct_selected_step_t *'s */
	List selected_parts;	/* List of char *'s */
	char *user;		/* user name */
} dbd_get_jobs_msg_t;

typedef struct dbd_init_msg {
	char *cluster_name;     /* cluster this message is coming from */
	uint16_t rollback;      /* to allow rollbacks or not */
	uint16_t version;	/* protocol version */
	uint32_t uid;		/* UID originating connection,
				 * filled by authtentication plugin*/
} dbd_init_msg_t;

typedef struct dbd_fini_msg {
	uint16_t close_conn;  /* to close connection 1, 0 will keep
				 connection open */
	uint16_t commit;      /* to rollback(0) or commit(1) changes */
} dbd_fini_msg_t;

typedef struct dbd_job_comp_msg {
	uint32_t assoc_id;	/* accounting association id needed to
				 * find job record in db */
	char *	 comment;	/* job comment field */
	uint32_t db_index;	/* index into the db for this job */
	uint32_t derived_ec;	/* derived job exit code or signal */
	time_t   end_time;	/* job termintation time */
	uint32_t exit_code;	/* job exit code or signal */
	uint32_t job_id;	/* job ID */
	uint16_t job_state;	/* job state */
	char *   nodes;		/* hosts allocated to the job */
	uint32_t req_uid;	/* requester user ID */
	time_t   start_time;	/* job start time */
	time_t   submit_time;	/* job submit time needed to find job
				 * record in db */
} dbd_job_comp_msg_t;

typedef struct dbd_job_start_msg {
	char *   account;       /* Account name for those not running
				 * with associations */
	uint32_t alloc_cpus;	/* count of allocated processors */
	uint32_t alloc_nodes;   /* how many nodes used in job */
	uint32_t assoc_id;	/* accounting association id */
	char *   block_id;      /* Bluegene block id */
	uint32_t db_index;	/* index into the db for this job */
	time_t   eligible_time;	/* time job becomes eligible to run */
	uint32_t gid;	        /* group ID */
	uint32_t job_id;	/* job ID */
	uint16_t job_state;	/* job state */
	char *   name;		/* job name */
	char *   nodes;		/* hosts allocated to the job */
	char *   node_inx;      /* ranged bitmap string of hosts
				 * allocated to the job */
	char *   partition;	/* partition job is running on */
	uint32_t priority;	/* job priority */
	uint32_t qos_id;        /* qos job is running with */
	uint32_t req_cpus;	/* count of req processors */
	uint32_t resv_id;	/* reservation id */
	time_t   start_time;	/* job start time */
	time_t   submit_time;	/* job submit time */
	uint32_t timelimit;	/* job timelimit */
	uint32_t uid;	        /* user ID if associations are being used */
	char *   wckey;		/* wckey name */
} dbd_job_start_msg_t;

/* returns a uint32_t along with a return code */
typedef struct dbd_id_rc_msg {
	uint32_t job_id;
	uint32_t id;
	uint32_t return_code;
} dbd_id_rc_msg_t;

typedef struct dbd_job_suspend_msg {
	uint32_t assoc_id;	/* accounting association id needed
				 * to find job record in db */
	uint32_t db_index;	/* index into the db for this job */
	uint32_t job_id;	/* job ID needed to find job record
				 * in db */
	uint16_t job_state;	/* job state */
	time_t   submit_time;	/* job submit time needed to find job record
				 * in db */
	time_t   suspend_time;	/* job suspend or resume time */
} dbd_job_suspend_msg_t;

typedef struct {
	List my_list;		/* this list could be of any type as long as it
				 * is handled correctly on both ends */
	uint32_t return_code;   /* If there was an error and a list of
				 * them this is the type of error it
				 * was */
} dbd_list_msg_t;

typedef struct {
	void *cond;
	void *rec;
} dbd_modify_msg_t;

#define DBD_NODE_STATE_DOWN  1
#define DBD_NODE_STATE_UP    2
typedef struct dbd_node_state_msg {
	uint32_t cpu_count;     /* number of cpus on node */
	time_t event_time;	/* time of transition */
	char *hostlist;		/* name of hosts */
	uint16_t new_state;	/* new state of host, see DBD_NODE_STATE_* */
	char *reason;		/* explanation for the node's state */
	uint32_t reason_uid;   	/* User that set the reason, ignore if
				 * no reason is set. */
	uint16_t state;         /* current state of node.  Used to get
				   flags on the state (i.e. maintenance) */
} dbd_node_state_msg_t;

typedef struct dbd_rc_msg {
	char *   comment;	/* reason for failure */
	uint32_t return_code;
	uint16_t sent_type;	/* type of message this is in response to */
} dbd_rc_msg_t;

typedef struct dbd_register_ctld_msg {
	uint16_t dimensions;    /* dimensions of system */
	uint32_t flags;         /* flags for cluster */
	uint32_t plugin_id_select; /* the select plugin_id */
	uint16_t port;		/* slurmctld's comm port */
} dbd_register_ctld_msg_t;

typedef struct dbd_step_comp_msg {
	uint32_t assoc_id;	/* accounting association id */
	uint32_t db_index;	/* index into the db for this job */
	time_t   end_time;	/* job termintation time */
	uint32_t exit_code;	/* job exit code or signal */
	jobacctinfo_t *jobacct; /* status info */
	uint32_t job_id;	/* job ID */
	uint32_t req_uid;	/* requester user ID */
	time_t   start_time;	/* step start time */
	time_t   job_submit_time;/* job submit time needed to find job record
				  * in db */
	uint32_t step_id;	/* step ID */
	uint32_t total_cpus;	/* count of allocated processors */
	uint32_t total_tasks;	/* count of tasks for step */
} dbd_step_comp_msg_t;

typedef struct dbd_step_start_msg {
	uint32_t assoc_id;	/* accounting association id */
	uint32_t db_index;	/* index into the db for this job */
	uint32_t job_id;	/* job ID */
	char *   name;		/* step name */
	char *   nodes;		/* hosts allocated to the step */
	char *   node_inx;	/* bitmap index of hosts allocated to
				 * the step */
	uint32_t node_cnt;      /* how many nodes used in step */
	time_t   start_time;	/* step start time */
	time_t   job_submit_time;/* job submit time needed to find job record
				  * in db */
	uint32_t step_id;	/* step ID */
	uint16_t task_dist;     /* layout method of step */
	uint32_t total_cpus;	/* count of allocated processors */
	uint32_t total_tasks;	/* count of tasks for step */
} dbd_step_start_msg_t;

/* flag to let us know if we are running on cache or from the actual
 * database */
extern uint16_t running_cache;
/* mutex and signal to let us know if associations have been reset so we need to
 * redo all the pointers to the associations */
extern pthread_mutex_t assoc_cache_mutex; /* assoc cache mutex */
extern pthread_cond_t assoc_cache_cond; /* assoc cache condition */

/*****************************************************************************\
 * Slurm DBD message processing functions
\*****************************************************************************/

/* Open a socket connection to SlurmDbd
 * auth_info IN - alternate authentication key
 * make_agent IN - make agent to process RPCs if set
 * rollback IN - keep journal and permit rollback if set
 * Returns SLURM_SUCCESS or an error code */
extern int slurm_open_slurmdbd_conn(char *auth_info,
                                    const slurm_trigger_callbacks_t *callbacks,
                                    bool rollback);

/* Close the SlurmDBD socket connection */
extern int slurm_close_slurmdbd_conn();

/* Send an RPC to the SlurmDBD. Do not wait for the reply. The RPC
 * will be queued and processed later if the SlurmDBD is not responding.
 * NOTE: slurm_open_slurmdbd_conn() must have been called with make_agent set
 *
 * Returns SLURM_SUCCESS or an error code */
extern int slurm_send_slurmdbd_msg(uint16_t rpc_version,
				   slurmdbd_msg_t *req);

/* Send an RPC to the SlurmDBD and wait for an arbitrary reply message.
 * The RPC will not be queued if an error occurs.
 * The "resp" message must be freed by the caller.
 * Returns SLURM_SUCCESS or an error code */
extern int slurm_send_recv_slurmdbd_msg(uint16_t rpc_version,
					slurmdbd_msg_t *req,
					slurmdbd_msg_t *resp);

/* Send an RPC to the SlurmDBD and wait for the return code reply.
 * The RPC will not be queued if an error occurs.
 * Returns SLURM_SUCCESS or an error code */
extern int slurm_send_slurmdbd_recv_rc_msg(uint16_t rpc_version,
					   slurmdbd_msg_t *req,
					   int *rc);

extern Buf pack_slurmdbd_msg(slurmdbd_msg_t *req, uint16_t rpc_version);
extern int unpack_slurmdbd_msg(slurmdbd_msg_t *resp,
			       uint16_t rpc_version, Buf buffer);

extern slurmdbd_msg_type_t str_2_slurmdbd_msg_type(char *msg_type);
extern char *slurmdbd_msg_type_2_str(slurmdbd_msg_type_t msg_type,
				     int get_enum);

extern void slurmdbd_free_buffer(void *x);

/*****************************************************************************\
 * Free various SlurmDBD message structures
\*****************************************************************************/
extern void slurmdbd_free_acct_coord_msg(dbd_acct_coord_msg_t *msg);
extern void slurmdbd_free_cluster_cpus_msg(dbd_cluster_cpus_msg_t *msg);
extern void slurmdbd_free_rec_msg(dbd_rec_msg_t *msg, slurmdbd_msg_type_t type);
extern void slurmdbd_free_cond_msg(dbd_cond_msg_t *msg,
				   slurmdbd_msg_type_t type);
extern void slurmdbd_free_init_msg(dbd_init_msg_t *msg);
extern void slurmdbd_free_fini_msg(dbd_fini_msg_t *msg);
extern void slurmdbd_free_job_complete_msg(dbd_job_comp_msg_t *msg);
extern void slurmdbd_free_job_start_msg(void *in);
extern void slurmdbd_free_id_rc_msg(void *in);
extern void slurmdbd_free_job_suspend_msg(dbd_job_suspend_msg_t *msg);
extern void slurmdbd_free_list_msg(dbd_list_msg_t *msg);
extern void slurmdbd_free_modify_msg(dbd_modify_msg_t *msg,
				     slurmdbd_msg_type_t type);
extern void slurmdbd_free_node_state_msg(dbd_node_state_msg_t *msg);
extern void slurmdbd_free_rc_msg(dbd_rc_msg_t *msg);
extern void slurmdbd_free_register_ctld_msg(dbd_register_ctld_msg_t *msg);
extern void slurmdbd_free_roll_usage_msg(dbd_roll_usage_msg_t *msg);
extern void slurmdbd_free_step_complete_msg(dbd_step_comp_msg_t *msg);
extern void slurmdbd_free_step_start_msg(dbd_step_start_msg_t *msg);
extern void slurmdbd_free_usage_msg(dbd_usage_msg_t *msg,
				    slurmdbd_msg_type_t type);

/*****************************************************************************\
 * Pack various SlurmDBD message structures into a buffer
\*****************************************************************************/
extern void slurmdbd_pack_acct_coord_msg(dbd_acct_coord_msg_t *msg,
					 uint16_t rpc_version,
					 Buf buffer);
extern void slurmdbd_pack_cluster_cpus_msg(dbd_cluster_cpus_msg_t *msg,
					   uint16_t rpc_version,
					   Buf buffer);
extern void slurmdbd_pack_rec_msg(dbd_rec_msg_t *msg,
				  uint16_t rpc_version,
				  slurmdbd_msg_type_t type, Buf buffer);
extern void slurmdbd_pack_cond_msg(dbd_cond_msg_t *msg,
				   uint16_t rpc_version,
				   slurmdbd_msg_type_t type, Buf buffer);
extern void slurmdbd_pack_init_msg(dbd_init_msg_t *msg, uint16_t rpc_version,
				   Buf buffer, char *auth_info);
extern void slurmdbd_pack_fini_msg(dbd_fini_msg_t *msg,
				   uint16_t rpc_version, Buf buffer);
extern void slurmdbd_pack_job_complete_msg(dbd_job_comp_msg_t *msg,
					   uint16_t rpc_version,
					   Buf buffer);
extern void slurmdbd_pack_job_start_msg(void *in,
					uint16_t rpc_version,
					Buf buffer);
extern void slurmdbd_pack_id_rc_msg(void *in,
				    uint16_t rpc_version,
				    Buf buffer);
extern void slurmdbd_pack_job_suspend_msg(dbd_job_suspend_msg_t *msg,
					  uint16_t rpc_version,
					  Buf buffer);
extern void slurmdbd_pack_list_msg(dbd_list_msg_t *msg,
				   uint16_t rpc_version,
				   slurmdbd_msg_type_t type,
				   Buf buffer);
extern void slurmdbd_pack_modify_msg(dbd_modify_msg_t *msg,
				     uint16_t rpc_version,
				     slurmdbd_msg_type_t type,
				     Buf buffer);
extern void slurmdbd_pack_node_state_msg(dbd_node_state_msg_t *msg,
					 uint16_t rpc_version,
					 Buf buffer);
extern void slurmdbd_pack_rc_msg(dbd_rc_msg_t *msg,
				 uint16_t rpc_version, Buf buffer);
extern void slurmdbd_pack_register_ctld_msg(dbd_register_ctld_msg_t *msg,
					    uint16_t rpc_version,
					    Buf buffer);
extern void slurmdbd_pack_roll_usage_msg(dbd_roll_usage_msg_t *msg,
					 uint16_t rpc_version, Buf buffer);
extern void slurmdbd_pack_step_complete_msg(dbd_step_comp_msg_t *msg,
					    uint16_t rpc_version,
					    Buf buffer);
extern void slurmdbd_pack_step_start_msg(dbd_step_start_msg_t *msg,
					 uint16_t rpc_version,
					 Buf buffer);
extern void slurmdbd_pack_usage_msg(dbd_usage_msg_t *msg,
				    uint16_t rpc_version,
				    slurmdbd_msg_type_t type,
				    Buf buffer);
extern void slurmdbd_pack_buffer(void *in,
				 uint16_t rpc_version,
				 Buf buffer);

/*****************************************************************************\
 * Unpack various SlurmDBD message structures from a buffer
\*****************************************************************************/
extern int slurmdbd_unpack_acct_coord_msg(dbd_acct_coord_msg_t **msg,
					  uint16_t rpc_version,
					  Buf buffer);
extern int slurmdbd_unpack_cluster_cpus_msg(dbd_cluster_cpus_msg_t **msg,
					    uint16_t rpc_version,
					    Buf buffer);
extern int slurmdbd_unpack_rec_msg(dbd_rec_msg_t **msg,
				   uint16_t rpc_version,
				   slurmdbd_msg_type_t type,
				   Buf buffer);
extern int slurmdbd_unpack_cond_msg(dbd_cond_msg_t **msg,
				    uint16_t rpc_version,
				    slurmdbd_msg_type_t type, Buf buffer);
extern int slurmdbd_unpack_init_msg(dbd_init_msg_t **msg, Buf buffer,
				    char *auth_info);
extern int slurmdbd_unpack_fini_msg(dbd_fini_msg_t **msg,
				    uint16_t rpc_version, Buf buffer);
extern int slurmdbd_unpack_job_complete_msg(dbd_job_comp_msg_t **msg,
					    uint16_t rpc_version,
					    Buf buffer);
extern int slurmdbd_unpack_job_start_msg(void **msg,
					 uint16_t rpc_version,
					 Buf buffer);
extern int slurmdbd_unpack_id_rc_msg(void **msg,
				     uint16_t rpc_version,
				     Buf buffer);
extern int slurmdbd_unpack_job_suspend_msg(dbd_job_suspend_msg_t **msg,
					   uint16_t rpc_version,
					   Buf buffer);
extern int slurmdbd_unpack_list_msg(dbd_list_msg_t **msg,
				    uint16_t rpc_version,
				    slurmdbd_msg_type_t type,
				    Buf buffer);
extern int slurmdbd_unpack_modify_msg(dbd_modify_msg_t **msg,
				      uint16_t rpc_version,
				      slurmdbd_msg_type_t type,
				      Buf buffer);
extern int slurmdbd_unpack_node_state_msg(dbd_node_state_msg_t **msg,
					  uint16_t rpc_version,
					  Buf buffer);
extern int slurmdbd_unpack_rc_msg(dbd_rc_msg_t **msg,
				  uint16_t rpc_version, Buf buffer);
extern int slurmdbd_unpack_register_ctld_msg(dbd_register_ctld_msg_t **msg,
					     uint16_t rpc_version,
					     Buf buffer);
extern int slurmdbd_unpack_roll_usage_msg(dbd_roll_usage_msg_t **msg,
					  uint16_t rpc_version,
					  Buf buffer);
extern int slurmdbd_unpack_step_complete_msg(dbd_step_comp_msg_t **msg,
					     uint16_t rpc_version,
					     Buf buffer);
extern int slurmdbd_unpack_step_start_msg(dbd_step_start_msg_t **msg,
					  uint16_t rpc_version,
					  Buf buffer);
extern int slurmdbd_unpack_usage_msg(dbd_usage_msg_t **msg,
				     uint16_t rpc_version,
				     slurmdbd_msg_type_t type,
				     Buf buffer);
extern int slurmdbd_unpack_buffer(void **in,
				  uint16_t rpc_version,
				  Buf buffer);
#endif	/* !_SLURMDBD_DEFS_H */
