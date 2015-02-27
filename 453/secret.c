#include <minix/drivers.h>
#include <minix/driver.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <minix/ds.h>
#include <fcntl.h>
#include <unistd.h>
#include "secret.h"

/*
 * Function prototypes for the secret driver.
 */
FORWARD _PROTOTYPE( char * secret_name,   (void) );
FORWARD _PROTOTYPE( int secret_ioctl,      (struct driver *d, message *m) );
FORWARD _PROTOTYPE( int secret_open,      (struct driver *d, message *m) );
FORWARD _PROTOTYPE( int secret_close,     (struct driver *d, message *m) );
FORWARD _PROTOTYPE( struct device * secret_prepare, (int device) );
FORWARD _PROTOTYPE( int secret_transfer,  (int procnr, int opcode,
                                          u64_t position, iovec_t *iov,
                                          unsigned nr_req) );
FORWARD _PROTOTYPE( void secret_geometry, (struct partition *entry) );

/* SEF functions and variables. */
FORWARD _PROTOTYPE( void sef_local_startup, (void) );
FORWARD _PROTOTYPE( int sef_cb_init, (int type, sef_init_info_t *info) );
FORWARD _PROTOTYPE( int sef_cb_lu_state_save, (int) );
FORWARD _PROTOTYPE( int lu_state_restore, (void) );

/* Entry points to the secret driver. */
PRIVATE struct driver secret_tab =
{
    secret_name,
    secret_open,
    secret_close,
    secret_ioctl,
    secret_prepare,
    secret_transfer,
    nop_cleanup,
    secret_geometry,
    nop_alarm,
    nop_cancel,
    nop_select,
    nop_ioctl,
    do_nop,
};

/** Represents the /dev/secret device. */
PRIVATE struct device secret_device;

/** State variable to count the number of times the device has been opened. */
PRIVATE int secret_isopen;
PRIVATE int open_fds;
PRIVATE int secret_iswriting;
PRIVATE uid_t secret_owner;
PRIVATE size_t secret_size;
PRIVATE char secret_message[SECRET_SIZE];

PRIVATE char * secret_name(void)
{
    return "secretkeeper";
}

PRIVATE int secret_ioctl(d, m)
    struct driver *d;
    message *m;
{
    int res;
    uid_t grantee; /* the uid of the new secret owner */
    if (m->REQUEST == SSGRANT) {
        res = sys_safecopyfrom(m->IO_ENDPT, (vir_bytes)m->IO_GRANT,
                               0, (vir_bytes)&grantee, sizeof(grantee), D);
        if (res)
            return res;

        secret_owner = grantee;

        return OK;
    }
    else
        return ENOTTY;
}

PRIVATE int secret_open(d, m)
    struct driver *d;
    message *m;
{
    struct ucred ucreds;
    int err;

    err = getnucred(m->IO_ENDPT, &ucreds);
    if (err) {
       fprintf(stderr, "Error getting ucred: %s\n", strerror(errno));
       return errno;
    }

    if ((m->REQUEST & (R_BIT | W_BIT)) == R_BIT) {
        if (secret_isopen || secret_owner == ucreds.uid) {
            open_fds ++;
            return OK;
        }
        else
            return EACCES;
    }
    else if ((m->REQUEST & (W_BIT | R_BIT)) == W_BIT) {
        if (secret_isopen) {
            secret_owner = ucreds.uid;
            secret_isopen = 0;

            secret_iswriting = 1;
            return OK;
        }
        else if (secret_owner == ucreds.uid)
            return ENOSPC;
        else
            return EACCES;
    }
    else
        return EACCES;
}

PRIVATE int secret_close(d, m)
    struct driver *d;
    message *m;
{
    if (secret_iswriting) {
        secret_iswriting = 0;
    }
    else {
       if (--open_fds <= 0) {
           secret_isopen = 1;
           secret_size = 0;
       }
    }
    return OK;
}

PRIVATE struct device * secret_prepare(dev)
    int dev;
{
    secret_device.dv_base.lo = 0;
    secret_device.dv_base.hi = 0;
    secret_device.dv_size.lo = secret_size;
    secret_device.dv_size.hi = 0;
    return &secret_device;
}

PRIVATE int secret_transfer(proc_nr, opcode, position, iov, nr_req)
    int proc_nr;
    int opcode;
    u64_t position;
    iovec_t *iov;
    unsigned nr_req;
{
    int bytes, ret, err;
    struct ucred ucreds;

    err = getnucred(proc_nr, &ucreds);
    if (err) {
       fprintf(stderr, "Error getting ucred: %s\n", strerror(errno));
    }
    
    switch (opcode)
    {
        case DEV_GATHER_S:
            bytes = secret_size - position.lo < iov->iov_size ?
                    secret_size - position.lo : iov->iov_size;

            if (bytes <= 0)
                return OK;


            ret = sys_safecopyto(proc_nr, iov->iov_addr, 0,
                                (vir_bytes) (secret_message + position.lo),
                                 bytes, D);

            iov->iov_size -= bytes;
            break;

        case DEV_SCATTER_S:

            if (secret_size + iov->iov_size > SECRET_SIZE)
                return ENOSPC;
            else
                bytes = iov->iov_size;

            ret = sys_safecopyfrom(proc_nr, iov->iov_addr, 0,
                                  (vir_bytes) (secret_message + position.lo),
                                   bytes, D);
            iov->iov_size -= bytes;
            secret_size += bytes;
  
            break;
        
        default:
            return EINVAL;
    }
    return ret;
}

PRIVATE void secret_geometry(entry)
    struct partition *entry;
{
    printf("secret_geometry()\n");
    entry->cylinders = 0;
    entry->heads     = 0;
    entry->sectors   = 0;
}

PRIVATE int sef_cb_lu_state_save(int state) {
/* Save the state. */
    ds_publish_u32("secret_owner", secret_owner, DSF_OVERWRITE);
    ds_publish_u32("secret_isopen", secret_isopen, DSF_OVERWRITE);
    ds_publish_u32("secret_iswriting", secret_iswriting, DSF_OVERWRITE);
    ds_publish_u32("secret_size", secret_size, DSF_OVERWRITE);
    ds_publish_u32("open_fds", open_fds, DSF_OVERWRITE);
    ds_publish_mem("secret_message", secret_message, secret_size, 0);

    return OK;
}

PRIVATE int lu_state_restore() {
/* Restore the state. */
    u32_t value;

    ds_retrieve_u32("secret_owner", &value);
    ds_delete_u32("secret_owner");
    secret_owner = (uid_t) value;

    ds_retrieve_u32("secret_isopen", &value);
    ds_delete_u32("secret_isopen");
    secret_isopen = (int) value;

    ds_retrieve_u32("open_fds", &value);
    ds_delete_u32("open_fds");
    open_fds = (int) value;

    ds_retrieve_u32("secret_iswriting", &value);
    ds_delete_u32("secret_iswriting");
    secret_iswriting = (int) value;

    ds_retrieve_u32("secret_size", &value);
    ds_delete_u32("secret_size");
    secret_size = (size_t) value;

    ds_retrieve_mem("secret_message", (char *)secret_message, &secret_size);
    ds_delete_u32("secret_message");
    
    return OK;
}

PRIVATE void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

PRIVATE int sef_cb_init(int type, sef_init_info_t *info)
{
/* Initialize the secret driver. */
    int do_announce_driver = TRUE;

    secret_isopen = 1;
    secret_size = open_fds = 0;
    if(type == SEF_INIT_LU) {
        /* Restore the state. */
        lu_state_restore();
        do_announce_driver = FALSE;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        driver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

PUBLIC int main(int argc, char **argv)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    driver_task(&secret_tab, DRIVER_STD);
    return OK;
}

