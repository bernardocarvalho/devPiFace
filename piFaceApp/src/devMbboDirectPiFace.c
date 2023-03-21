/* devMbboDirectSoft.c */
/*
 * Original Author: Bob Dalesio
 * Date: 10-08-93
 */

#include <stdio.h>

#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "mbboDirectRecord.h"
#include "epicsExport.h"

#include "drvPiFace.h"

static long init_record();
static long write_mbbo();
int fd;

static long init_record(dbCommon *common)
{
    mbboDirectRecord *prec = (mbboDirectRecord *) common;
    /* mbboDirectRecord.inp must be a CONSTANT */
    if (prec->out.type != CONSTANT)
    {
        recGblRecordError(S_db_badField, prec,
                          "devMbboDirectPiFace (init_record) Illegal OUT field");
        return S_db_badField;
    }
    if (prec->nobt == 0)
        prec->mask = 0xffffffff;

    prec->mask <<= prec->shft;

//    devRndData *data;
    fd = piface_open();
    prec->dpvt = &fd;

    return 2; /* Don't convert */
}


static long write_mbbo(mbboDirectRecord *prec)
{
    epicsUInt32 data;

    int *pfd;

    pfd = prec->dpvt;
    data = prec->rval & prec->mask;

    piface_write_register(*pfd, PIFACE_GPIOA, data);
    //dbPutLink(&prec->out, DBR_ULONG, &prec->val, 1);
    return 0;
}

/* Create the dset for devMbboDirectPiFace */
mbbodirectdset devMbboDirectPiFace = {
    {5, NULL, NULL, init_record, NULL},
    write_mbbo
};
epicsExportAddress(dset, devMbboDirectPiFace);

