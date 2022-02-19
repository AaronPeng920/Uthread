/*
 *   FILE: uthread_mtx.c
 * AUTHOR: Peter Demoreuille
 *  DESCR: userland mutexes
 *   DATE: Sat Sep  8 12:40:00 2001
 *
 *
 * Modified to handle time slicing by Tom Doeppner
 *   DATE: Sun Jan 10, 2016
 * Modified for SCUT students in July 2021 By Wu Yimin
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "uthread.h"
#include "uthread_mtx.h"
#include "uthread_sched.h"

/*
 * uthread_mtx_init
 *
 * 初始化指定的 mutex.
 */
void uthread_mtx_init(uthread_mtx_t *mtx) {
    // Function_you_need_to_implement("UTHREADS: uthread_mtx_init");
    
    utqueue_init(&mtx->m_waiters);
}

/*
 * uthread_mtx_lock
 *
 * 如果没有线程占有该锁，则将锁的拥有着改为当前线程，否则，当前阻塞.  
 */
void uthread_mtx_lock(uthread_mtx_t *mtx) {
    // Function_you_need_to_implement("UTHREADS: uthread_mtx_lock");
    
    assert(mtx != NULL);
    if(mtx->m_owner == NULL)
        mtx->m_owner = ut_curthr;
    else{
        utqueue_enqueue(&mtx->m_waiters,ut_curthr);
        uthread_block();
        mtx->m_owner = ut_curthr;
    }
}

/*
 * uthread_mtx_trylock
 *
 * 试图上锁 mutex, 得到锁时返回 1 , 否则返回 0 .
 */
int uthread_mtx_trylock(uthread_mtx_t *mtx) {
    // Function_you_need_to_implement("UTHREADS: uthread_mtx_trylock");
    
    assert(mtx != NULL);
    if(mtx->m_owner == NULL){
        mtx->m_owner = ut_curthr;
        return 1;
    }

    return 0;
}

/*
 * uthread_mtx_unlock
 *
 * 释放锁.  如果有其他线程在等待该锁，则唤醒它.
 */
void uthread_mtx_unlock(uthread_mtx_t *mtx) {
    // Function_you_need_to_implement("UTHREADS: uthread_mtx_unlock");
    
    assert(mtx != NULL && mtx->m_owner == ut_curthr);
    mtx->m_owner = NULL;
    if(!utqueue_empty(&mtx->m_waiters)){
        uthread_t* ready = utqueue_dequeue(&mtx->m_waiters);
        uthread_wake(ready);
    }
}
