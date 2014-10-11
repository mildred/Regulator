#ifndef __ADOS_INCLUDE
#define __ADOS_INCLUDE
/*
  adOS - Cooperative multitasking scheduler for Arduino.
  Copyright (c) 2010 Alexander P. Kolesnikov. All right reserved.

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

extern "C" {
      #include <inttypes.h>
}

#define ADOS      cAdOS::GetInstance()

class AdOSTaskInterface {
public:
  virtual void main() = 0;
};

typedef enum ados_status_t
{
      ADOS_STATUS_RESERVED,
      ADOS_STATUS_OK,
      ADOS_STATUS_ERROR,
      ADOS_STATUS_BAD_PARAM,
      ADOS_STATUS_NOT_READY,
}ados_status_t;

typedef enum ados_state_t
{
      ADOS_STATE_RESERVED,
      ADOS_STATE_READY_TO_RUN,
      ADOS_STATE_NOT_READY_TO_RUN,
      ADOS_STATE_SLEEP,
}ados_state_t;

typedef unsigned long ados_timestamp_t;
typedef uint8_t ados_priority_t;

//typedef void(*ados_taskptr_t)();
typedef AdOSTaskInterface  ados_task_t;
typedef AdOSTaskInterface *ados_taskptr_t;

typedef struct ados_tcb_t
{
      uint8_t* m_stack;
      ados_taskptr_t m_taskptr;
      ados_priority_t m_priority;
      ados_timestamp_t m_nexttimetorun;
      ados_state_t m_state;
      volatile struct ados_tcb_t* m_next;
      volatile struct ados_event_t*   m_waitingfor;
}ados_tcb_t;

typedef struct ados_event_t
{
      uint8_t             m_state;
      volatile void*  m_param;
}ados_event_t;

typedef struct ados_ctrl_t
{
      uint8_t*                         m_stack;
      volatile ados_tcb_t*      m_firstcb;
      volatile ados_tcb_t*      m_currenttcb;
}ados_ctrl_t;

class cAdOS
{
public:
      void Init();
      void Start();

static cAdOS* GetInstance();

      ados_status_t AddTask(volatile ados_tcb_t* tcb, ados_taskptr_t taskptr,
            uint8_t* stack, unsigned int stacksize, ados_priority_t priority);
      void Sleep(unsigned long msec);

      uint8_t EventTest(volatile ados_event_t* event);
      volatile void* EventWaitFor(volatile ados_event_t* event, unsigned long timeout);
      void EventPulse(volatile ados_event_t* event, volatile void* param, bool fromisr=false);
      void EventSet(volatile ados_event_t* event, volatile void* param, bool fromisr=false);
      void EventReset(volatile ados_event_t* event);

private:
      cAdOS();
};

class AdOSTask : public AdOSTaskInterface {
  volatile ados_tcb_t tcb;

public:

  AdOSTask(unsigned char *stack, unsigned int stacksize, ados_priority_t priority) {
    stack[0] = 0xE7;
    ados()->AddTask(&tcb, this, stack, stacksize, priority);
  }

  virtual ~AdOSTask() {}

  inline cAdOS *ados() { return cAdOS::GetInstance(); }

  virtual void setup() {};
  virtual void loop() {};
  virtual void main() {
    setup();
    while(1) {
      loop();
    }
  }
};

template <int stacksize> class AdOSTask2 : public AdOSTask {
  unsigned char stack[stacksize];
  
public:
  
  AdOSTask2(ados_priority_t priority = 0) : AdOSTask(stack, stacksize, priority) {
  }
};

#endif //__ADOS_INCLUDE