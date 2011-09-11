/*
 * Copyright (c) 2004 Silicon Graphics, Inc.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * common data structures for pmlogextract
 */

#ifndef _LOGGER_H
#define _LOGGER_H

typedef struct {
    int			flags;		/* GLOBAL_* flags */
    struct timeval	time;		/* timestamp shift */
    char		hostname[PM_LOG_MAXHOSTLEN];
    char		tz[PM_TZ_MAXLEN]; 
} global_t;

/* values for global_t flags */
#define GLOBAL_CHANGE_TIME	1
#define GLOBAL_CHANGE_HOSTNAME	2
#define GLOBAL_CHANGE_TZ	4

extern global_t global;

typedef struct indomspec {
    struct indomspec	*i_next;
    pmInDom		old_indom;
    pmInDom		new_indom;	/* PM_INDOM_NULL if no change */
    int			numinst;
    int			*old_inst;	/* filled from pmGetInDomArchive() */
    char		**old_iname;	/* filled from pmGetInDomArchive() */
    int			*flags;		/* INST_* flags * */
    int			*new_inst;
    char		**new_iname;
} indomspec_t;

/* values for indomspec_t flags[] */
#define INST_CHANGE_INST	16
#define INST_CHANGE_INAME	32
#define INST_DELETE		64

extern indomspec_t	*indom_root;

/*
 *  list of pdu's to write out at start of time window
 */
typedef struct _reclist_t {
    __pmPDU		*pdu;		/* PDU ptr */
    pmDesc		desc;
    int			written;	/* written status */
    struct _reclist_t	*ptr;		/* ptr to record in another reclist */
    struct _reclist_t	*next;		/* ptr to next reclist_t record */
} reclist_t;

/*
 *  Input archive control
 */
typedef struct {
    int		ctx;
    char	*name;
    pmLogLabel	label;
    __pmPDU	*pb[2];
    pmResult	*_result;
    pmResult	*_Nresult;
    int		eof[2];
    int		mark;		/* need EOL marker */
} inarch_t;

extern inarch_t	inarch;	/* input archive control(s) */

/*
 *  instance list
 */
typedef struct {
    int		id;		/* instance id */
    int		ready;		/* if (ready == 1) then write out this inst */
    /*
     * timestamp is used to determine when to write out the next record
     */
    double	lasttstamp;	/* time of last write (0 == b4 first) */
    double	nexttstamp;	/* time of next write (0 == b4 first) */
    /*
     * lasttime and lastval are used to calculate the time and value for
     * this interval
     */
    double	lasttime;	/* time of last record (0 == b4 first) */
    double	lastval;	/* value of last record */
    /*
     * value, deltat and numsamples, are used to calculate the final value
     * to be written out; 
     *		time average       = value / deltat
     *		stochastic average = value / numsamples
     */
    double	value;		/* value so far (0 == b4 first value) */
    double	deltat;		/* elapsed time since last write */
    int		numsamples;	/* number of samples since last write */
    /*
     * the final average value to be written out
     */
    double	average;
} instlist_t;
    
/*
 *  metric [instance] list
 */
typedef struct {
    char	*name;		/* metric name */
    /* normally idesc and odesc will point to the same descriptor ...
     * however, if the "-t" flag is specified, then in the case of
     * counters and instantaneous values, odesc will be different
     */
    pmDesc	*idesc;		/* input  metric descriptor - pmid, pmindom */
    pmDesc	*odesc;		/* output metric descriptor - pmid, pmindom */
    double	scale;		/* scale multiplier for counter metrics */
    int		numinst;	/* number of instances (0 means all) */
    instlist_t	*instlist;	/* instance id [see above] list */
} mlist_t;


/*
 *  pmResult list
 */
typedef struct __rlist_t {
    pmResult		*res;		/* ptr to pmResult */
    struct __rlist_t	*next;		/* ptr to next element in list */
} rlist_t;


extern int	ml_numpmid;		/* num pmid in ml list */
extern int	ml_size;		/* actual size of ml array */
extern mlist_t	*ml;			/* list of pmids with indoms */
extern rlist_t	*rl;			/* list of pmResults */

/* generic error message buffer */
extern char	mess[256];

/* yylex() gets intput from here ... */
extern FILE	*fconfig;

extern void	yyerror(char *);
extern void	yywarn(char *);
extern int	yylex(void);
extern int	yyparse(void);
extern void	dometric(const char *);

#endif /* _LOGGER_H */
