/*
 adOS.c - Cooperative multitasking scheduler for Arduino.
 Copyright (c) 2010 Alexander P. Kolesnikov.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Arduino.h>
#include "ados.h"

extern "C" {
      #include <setjmp.h>
}

#define ADOS_STORE_SREG() \
      asm volatile(   \
            "in r0,__SREG__\n\t"      \
            "push r0\n\t"   \
      );

#define ADOS_RESTORE_SREG()  \
      asm volatile(  \
            "pop r0 \n\t" \
            "out __SREG__,r0 \n\t"  \
      );

#define ADOS_PUSH_REGISTERS() \
      asm volatile( \
            "push r0\n\t push r1\n\t push r2\n\t push r3\n\t" \
            "push r4\n\t push r5\n\t push r6\n\t push r7\n\t" \
            "push r8\n\t push r9\n\t push r10\n\t push r11\n\t" \
            "push r12\n\t push r13\n\t push r14\n\t push r15\n\t" \
            "push r16\n\t push r17\n\t push r18\n\t push r19\n\t" \
            "push r20\n\t push r21\n\t push r22\n\t push r23\n\t" \
            "push r24\n\t push r25\n\t push r26\n\t push r27\n\t" \
            "push r28\n\t push r29\n\t push r30\n\t push r31\n\t");

#define ADOS_POP_REGISTERS() \
      asm volatile( \
            "pop r31\n\t pop r30\n\t pop r29\n\t pop r28\n\t" \
            "pop r27\n\t pop r26\n\t pop r25\n\t pop r24\n\t" \
            "pop r23\n\t pop r22\n\t pop r21\n\t pop r20\n\t" \
            "pop r19\n\t pop r18\n\t pop r17\n\t pop r16\n\t" \
            "pop r15\n\t pop r14\n\t pop r13\n\t pop r12\n\t" \
            "pop r11\n\t pop r10\n\t pop r9\n\t pop r8\n\t" \
            "pop r7\n\t pop r6\n\t pop r5\n\t pop r4\n\t" \
            "pop r3\n\t pop r2\n\t pop r1\n\t pop r0\n\t");

#define ADOS_STORE_STACKPTR() \
      asm volatile( \
            "in r0, __SP_L__ \n\t" \
            "sts g_oldstackptr,r0 \n\t" \
            "in r0, __SP_H__ \n\t" \
            "sts (g_oldstackptr)+1,r0 \n\t");

#define ADOS_RESTORE_STACKPTR() \
      asm volatile( \
            "lds r0, (g_newstackptr)+1 \n\t" \
            "out __SP_H__,r0 \n\t" \
            "lds r0, (g_newstackptr) \n\t" \
            "out __SP_L__,r0 \n\t");

extern "C" {
      uint8_t*      g_newstackptr  = (uint8_t*)NULL;
      uint8_t*      g_oldstackptr  = (uint8_t*)NULL;
}

static uint8_t  g_oldSREG;
static volatile ados_ctrl_t g_ados_ctrl = {0};
static volatile ados_tcb_t* g_newtcb = (volatile ados_tcb_t*)NULL;
static ados_taskptr_t      g_taskptr = (ados_taskptr_t)NULL;
static jmp_buf g_jmpbuf;
static uint8_t g_shedulerstarted = 0;

static uint8_t g_idleTaskStack[64];
static volatile ados_tcb_t g_idleTcb = {0};

static void ados_lock()
{
      g_oldSREG = SREG & 0x80;
      cli();
}

static void ados_unlock()
{
      if (g_oldSREG)
      {
            sei();
      }
}

static void ados_reSchedule()
{
      if (g_shedulerstarted)
      {
            volatile ados_tcb_t* t_tcb;

            ados_timestamp_t t_currenttime = millis();

            for (t_tcb=g_ados_ctrl.m_firstcb; NULL!=t_tcb; t_tcb=t_tcb->m_next)
            {
                  if (ADOS_STATE_SLEEP==t_tcb->m_state && t_tcb->m_nexttimetorun<=t_currenttime)
                  {
                        t_tcb->m_state = ADOS_STATE_READY_TO_RUN;
                  }
                  if (ADOS_STATE_READY_TO_RUN==t_tcb->m_state)
                  {
                        if (t_tcb==g_ados_ctrl.m_currenttcb)
                        {
                              break;
                        }
                        g_newstackptr = t_tcb->m_stack;
                        g_newtcb = t_tcb;
                        //save current contex in the stack
                        //store GP registers
                        ADOS_PUSH_REGISTERS();
                        //store status register
                        ADOS_STORE_SREG();
                        //store current SP
                        ADOS_STORE_STACKPTR();
                           g_ados_ctrl.m_currenttcb->m_stack = g_oldstackptr;
                        g_ados_ctrl.m_currenttcb = g_newtcb;
                        //switch to new task
                        //set SP to new task stack
                        ADOS_RESTORE_STACKPTR();
                        //restore status register
                        ADOS_RESTORE_SREG();
                        //restore GP registers
                        ADOS_POP_REGISTERS();                        
                        break;
                  }
            }
      }
      else
      {
            ados_lock();
            //save context
            //store GP registers
            ADOS_PUSH_REGISTERS();
            //store status register
            ADOS_STORE_SREG();
            //store current SP
            ADOS_STORE_STACKPTR();
            g_ados_ctrl.m_currenttcb->m_stack = g_oldstackptr;
            g_ados_ctrl.m_currenttcb = (volatile ados_tcb_t*)NULL;
            longjmp(g_jmpbuf,1);
            //will return to ados_initTask caller
      }
      ados_unlock();
}

static void ados_initTask(volatile ados_tcb_t* tcb)
{
      g_newstackptr = tcb->m_stack;
      g_taskptr = tcb->m_taskptr;
      g_ados_ctrl.m_currenttcb = tcb;

      //store GP registers
      ADOS_PUSH_REGISTERS();
      //store status register
      ADOS_STORE_SREG();
      if (!setjmp(g_jmpbuf))
      {
            //set SP to task stack
            ADOS_RESTORE_STACKPTR();
            ados_unlock();
            //g_taskptr();
            g_taskptr->main();
      }
      else
      {
            //restore status register
            ADOS_RESTORE_SREG();
            //restore GP registers
            ADOS_POP_REGISTERS();
            ados_unlock();
      }
}

class IdleTask : public AdOSTaskInterface {
  void main() {
      ados_lock();
      ados_reSchedule();
      for(;;)
      {
            ados_lock();
            ados_reSchedule();
            delay(1);
      }
  }
};

IdleTask idleTask;

void cAdOS::Init()
{
      //add Idle task
      AddTask(&g_idleTcb, &idleTask, 
            g_idleTaskStack, sizeof(g_idleTaskStack), 255);
}

cAdOS::cAdOS()
{
}

void cAdOS::Start()
{
      volatile ados_tcb_t* t_tcb;

      for (t_tcb=g_ados_ctrl.m_firstcb; NULL!=t_tcb; t_tcb=t_tcb->m_next)
      {
            ados_initTask(t_tcb);
      }
      ados_lock();
      //start scheduler
      g_shedulerstarted = 1;
      //switch to Idle task context
      g_idleTcb.m_state = ADOS_STATE_READY_TO_RUN;
      ados_reSchedule();
}

cAdOS* cAdOS::GetInstance()
{
      static cAdOS *instance = 0;
      if(!instance) {
          instance = new cAdOS();
          instance->Init();
      }
      return instance;
}

ados_status_t cAdOS::AddTask(volatile ados_tcb_t* tcb, ados_taskptr_t taskptr,
      uint8_t* stack, unsigned int stacksize, ados_priority_t priority)
{
      if (NULL==tcb || NULL==stack)
      {
            return ADOS_STATUS_BAD_PARAM;
      }
      ados_lock();
      tcb->m_stack = stack+stacksize-1;
      tcb->m_taskptr = taskptr;
      tcb->m_priority = priority;
      tcb->m_state = ADOS_STATE_NOT_READY_TO_RUN;
      if (NULL==g_ados_ctrl.m_firstcb)
      {
            tcb->m_next = g_ados_ctrl.m_firstcb;
            g_ados_ctrl.m_firstcb = tcb;
      }
      else
      {
            volatile ados_tcb_t* t_tcb;
            ados_tcb_t** t_prevtcb = (ados_tcb_t**)&g_ados_ctrl.m_firstcb;

            for (t_tcb=g_ados_ctrl.m_firstcb; NULL!=t_tcb; t_tcb=t_tcb->m_next)
            {
                  if (tcb->m_priority<=t_tcb->m_priority)
                  {
                        tcb->m_next = *t_prevtcb;
                        *t_prevtcb = (ados_tcb_t*)tcb;
                        break;
                  }
                  t_prevtcb = (ados_tcb_t**)&t_tcb->m_next;
            }
      }
      ados_unlock();
      return ADOS_STATUS_OK;
}

void cAdOS::Sleep(unsigned long msec)
{
      if (0!=msec)
      {
            ados_lock();
            g_ados_ctrl.m_currenttcb->m_state = ADOS_STATE_SLEEP;
            g_ados_ctrl.m_currenttcb->m_nexttimetorun = millis() + msec;
            ados_reSchedule();
      }
}

uint8_t cAdOS::EventTest(volatile ados_event_t* event)
{
      return event->m_state;
}

volatile void* cAdOS::EventWaitFor(volatile ados_event_t* event, unsigned long timeout)
{
      if (!event->m_state)
      {
            ados_lock();
            if (timeout)
            {
                  g_ados_ctrl.m_currenttcb->m_state = ADOS_STATE_SLEEP;
                  g_ados_ctrl.m_currenttcb->m_nexttimetorun = millis() + timeout;
            }
            else
            {
                  g_ados_ctrl.m_currenttcb->m_state = ADOS_STATE_NOT_READY_TO_RUN;
            }
            g_ados_ctrl.m_currenttcb->m_waitingfor = event;
            ados_reSchedule();
      }
      return event->m_param;
}

void cAdOS::EventPulse(volatile ados_event_t* event,
      volatile void* param, bool fromisr)
{
      volatile ados_tcb_t* t_tcb;

      ados_lock();
      event->m_param = param;
      for (t_tcb=g_ados_ctrl.m_firstcb; NULL!=t_tcb; t_tcb=t_tcb->m_next)
      {
            if (t_tcb->m_waitingfor==event)
            {
                  t_tcb->m_state = ADOS_STATE_READY_TO_RUN;
                  t_tcb->m_waitingfor = NULL;
            }
      }
      if (!fromisr)
      {
            ados_reSchedule();
      }
}

void cAdOS::EventSet(volatile ados_event_t* event,
      volatile void* param, bool fromisr)
{
      ados_lock();
      event->m_state = 1;
      ados_unlock();
      EventPulse(event, param, fromisr);
}

void cAdOS::EventReset(volatile ados_event_t* event)
{
      ados_lock();
      event->m_state = 0;
      event->m_param = NULL;
      ados_unlock();
}
