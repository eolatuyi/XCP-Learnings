#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "XcpBasic.h"
#include "xcp_driver.h"
#include "xcp_application.h"
#include "tester_utilities.h"

#define RATE_10MS               10

#define EXEC_10MS_TASK_PRIORITY        (1)
#define TESTER_UTILITIES_TASK_PRIORITY (0)
#define XCP_APP_SERVER_TASK_PRIORITY   (1) //TODO: need some yield

#define EXEC_10MS_STACK_DEPTH            128 /* 4 bytes X EXEC_STACK_DEPTH = Stack Size */
#define XCP_APP_SERVER_STACK_DEPTH       128 /* 4 bytes X EXEC_STACK_DEPTH = Stack Size */
#define TESTER_UTILITIES_STACK_DEPTH     128 /* 4 bytes X EXEC_STACK_DEPTH = Stack Size */

DECLARE_XCP_DRIVER;

uint32_t SystemCoreClock = 16000000;

void exec_10ms(void *pvParameters)
{
    uint8_t run_count;

    // Establish the task's period.
    const TickType_t   period = pdMS_TO_TICKS(RATE_10MS);
    TickType_t last_wake_time = xTaskGetTickCount();

    for (;;)
    {
            /* 10ms cycle */
        
        if (!(run_count % 10))
        { 
            XcpEvent(0); /* 100ms cycle */
        }
        
        if (!(run_count % 20))
        { 
            XcpEvent(1); /* 200ms cycle */
        }
        
        if (!(run_count % 100))
        { 
            XcpEvent(2); /* 1000ms cycle */
        }
        
        run_count++;
        // Block until the next release time.
        vTaskDelayUntil(&last_wake_time, period);
    }

}

void init(void)
{
    tester_utilities_init();
    xcp_app_init();
    return;
}

int main()
{
    init();

    xTaskCreate(exec_10ms, "10ms Executive Task", EXEC_10MS_STACK_DEPTH, NULL, 
                EXEC_10MS_TASK_PRIORITY, NULL);
    
    xTaskCreate(xcp_app_server_thread, "XCP Server Task", XCP_APP_SERVER_STACK_DEPTH, 
               &XCP_DRIVER_CBLK, XCP_APP_SERVER_TASK_PRIORITY, NULL);

    xTaskCreate(tester_utilities_simulator_thread, "Tester Utility Task", TESTER_UTILITIES_STACK_DEPTH, 
               &TESTER_UTILITIES_SIMULATOR_CBLK, TESTER_UTILITIES_TASK_PRIORITY, NULL);
               
    // Startup of the FreeRTOS scheduler.  The program should block here.
    vTaskStartScheduler();

    // The following line should never be reached.  Failure to allocate enough
    //  memory from the heap would be one reason.
    for (;;);

}


