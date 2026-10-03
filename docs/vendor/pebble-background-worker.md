<!--
source: https://developer.repebble.com/guides/events-and-services/background-worker/
fetched: 2026-10-02T07:38:14Z
-->

[pebble](/)

- [Tutorials](/tutorials/)
- [Get the SDK](/sdk/)
- [Guides](/guides/)
- [Documentation](/docs/)
- [Examples](/examples/)
- [Community](/community/)
- [Blog](https://repebble.com/blog)
- [More](/more/)

[Privacy](https://repebble.com/privacy/)  
[Cookies](https://repebble.com/terms)

[Publish](https://developer.rePebble.com/dashboard)

### [Guides](/guides/)

- [Table of Contents](/guides/toc/)
- [Alloy](/guides/alloy/)
- [App Resources](/guides/app-resources/)
- [Best Practices](/guides/best-practices/)
- [Communication](/guides/communication/)
- [Debugging](/guides/debugging/)
- [Design and Interaction](/guides/design-and-interaction/)
- [Events and Services](/guides/events-and-services/)
  - [Accelerometer](/guides/events-and-services/accelerometer/)
  - [Background Worker](/guides/events-and-services/background-worker/)
  - [Backlight](/guides/events-and-services/light/)
  - [Buttons](/guides/events-and-services/buttons/)
  - [Compass](/guides/events-and-services/compass/)
  - [Dictation](/guides/events-and-services/dictation/)
  - [Event Services](/guides/events-and-services/events/)
  - [Heart Rate Monitor](/guides/events-and-services/hrm/)
  - [Pebble Health](/guides/events-and-services/health/)
  - [Persistent Storage](/guides/events-and-services/persistent-storage/)
  - [Speaker](/guides/events-and-services/speaker/)
  - [Touch](/guides/events-and-services/touch/)
  - [Wakeups](/guides/events-and-services/wakeups/)
- [Graphics and Animations](/guides/graphics-and-animations/)
- [Pebble Packages](/guides/pebble-packages/)
- [Pebble Timeline](/guides/pebble-timeline/)
- [Tools and Resources](/guides/tools-and-resources/)
- [User Interfaces](/guides/user-interfaces/)

[](javascript:void(0);)

Adding a Worker Launching the Worker Communicating Between Tasks Managing the Worker Available APIs

# Background Worker

In addition to the main foreground task that every Pebble app implements, a second background worker task can also be created. This worker is capable of running even when the foreground task is closed, and is useful for tasks that must continue for long periods of time. For example, apps that log sensor data.

There are several important points to note about the capabilities of this worker when compared to those of the foreground task:

- The worker is constrained to 10.5 kB of memory.

- Some APIs are not available to the worker. See the [*Available APIs*](#available-apis) section below for more information.

- There can only be one background worker active at a time. In the event that a second one attempts to launch from another watchapp, the user will be asked to choose whether the new worker can replace the existing one.

- The user can determine which app's worker is running by checking the 'Background App' section of the Settings menu. Workers can also be launched from there.

- The worker can launch the foreground app using [`worker_launch_app()`](/docs/c/Worker/#worker_launch_app "worker_launch_app"). This means that the foreground app should be prepared to be launched at any time that the worker is running.

> Note: This API should not be used to build background timers; use the [`Wakeup`](/docs/c/Foundation/Wakeup/ "Wakeup") API instead.

## Adding a Worker

The background worker's behavior is determined by code written in a separate C file to the foreground app, created in the `/worker_src` project directory.

This project structure can also be generated using the [`pebble` tool](/guides/tools-and-resources/pebble-tool/) with the `--worker` flag as shown below:

    $ pebble new-project --worker project_name

The worker C file itself has a basic structure similar to a regular Pebble app, but with a couple of minor changes, as shown below:

    #include <pebble_worker.h>

    static void prv_init() {
      // Initialize the worker here
    }

    static void prv_deinit() {
      // Deinitialize the worker here
    }

    int main(void) {
      prv_init();
      worker_event_loop();
      prv_deinit();
    }

## Launching the Worker

To launch the worker from the foreground app, use [`app_worker_launch()`](/docs/c/Foundation/AppWorker/#app_worker_launch "app_worker_launch"):

    // Launch the background worker
    AppWorkerResult result = app_worker_launch();

The [`AppWorkerResult`](/docs/c/Foundation/AppWorker/#AppWorkerResult "AppWorkerResult") returned will indicate any errors encountered as a result of attempting to launch the worker. Possible result values include:

| Result                                                                                                                                                 | Value | Description                                                                                                                                                                                                                                                           |
|--------------------------------------------------------------------------------------------------------------------------------------------------------|-------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [`APP_WORKER_RESULT_SUCCESS`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_SUCCESS "APP_WORKER_RESULT_SUCCESS")                                     | `0`   | The worker launch was successful, but may not start running immediately. Use [`app_worker_is_running()`](/docs/c/Foundation/AppWorker/#app_worker_is_running "app_worker_is_running") to determine when the worker has started running.                               |
| [`APP_WORKER_RESULT_NO_WORKER`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_NO_WORKER "APP_WORKER_RESULT_NO_WORKER")                               | `1`   | No worker found for the current app.                                                                                                                                                                                                                                  |
| [`APP_WORKER_RESULT_ALREADY_RUNNING`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_ALREADY_RUNNING "APP_WORKER_RESULT_ALREADY_RUNNING")             | `4`   | The worker is already running.                                                                                                                                                                                                                                        |
| [`APP_WORKER_RESULT_ASKING_CONFIRMATION`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_ASKING_CONFIRMATION "APP_WORKER_RESULT_ASKING_CONFIRMATION") | `5`   | The user will be asked for confirmation. To determine whether the worker was given permission to launch, use [`app_worker_is_running()`](/docs/c/Foundation/AppWorker/#app_worker_is_running "app_worker_is_running") for a short period after receiving this result. |

## Communicating Between Tasks

There are three methods of passing data between the foreground and background worker tasks:

- Save the data using the [`Storage`](/docs/c/Foundation/Storage/ "Storage") API, then read it in the other task.

- Send the data to a companion phone app using the [`DataLogging`](/docs/c/Foundation/DataLogging/ "DataLogging") API. Details on how to do this are available in [*Datalogging*](/guides/communication/datalogging/).

- Pass the data directly while the other task is running, using an [`AppWorkerMessage`](/docs/c/Foundation/AppWorker/#AppWorkerMessage "AppWorkerMessage"). These messages can be sent bi-directionally by creating an `AppWorkerMessageHandler` in each task. The handler will fire in both the foreground and the background tasks, so you must identify the source of the message using the `type` parameter.

      // Used to identify the source of a message
      #define SOURCE_FOREGROUND 0
      #define SOURCE_BACKGROUND 1

  **Foreground App**

      static int s_some_value = 1;
      static int s_another_value = 2;

      static void worker_message_handler(uint16_t type, 
                                          AppWorkerMessage *message) {
        if(type == SOURCE_BACKGROUND) {
          // Get the data, only if it was sent from the background
          s_some_value = message->data0;
          s_another_value = message->data1;
        }
      }

      // Subscribe to get AppWorkerMessages
      app_worker_message_subscribe(worker_message_handler);

      // Construct a message to send
      AppWorkerMessage message = {
        .data0 = s_some_value,
        .data1 = s_another_value
      };

      // Send the data to the background app
      app_worker_send_message(SOURCE_FOREGROUND, &message);

  **Worker**

      static int s_some_value = 3;
      static int s_another_value = 4;

      // Construct a message to send
      AppWorkerMessage message = {
        .data0 = s_some_value,
        .data1 = s_another_value
      };

      static void worker_message_handler(uint16_t type, 
                                          AppWorkerMessage *message) {
        if(type == SOURCE_FOREGROUND) {
          // Get the data, if it was sent from the foreground
          s_some_value = message->data0;
          s_another_value = message->data1;
        }
      }

      // Subscribe to get AppWorkerMessages
      app_worker_message_subscribe(worker_message_handler);

      // Send the data to the foreground app
      app_worker_send_message(SOURCE_BACKGROUND, &message);

## Managing the Worker

The current running state of the background worker can be determined using the [`app_worker_is_running()`](/docs/c/Foundation/AppWorker/#app_worker_is_running "app_worker_is_running") function:

    // Check to see if the worker is currently active
    bool running = app_worker_is_running();

The user can tell whether the worker is running by checking the system 'Background App' settings. Any installed workers with be listed there.

The worker can be stopped using [`app_worker_kill()`](/docs/c/Foundation/AppWorker/#app_worker_kill "app_worker_kill"):

    // Stop the background worker
    AppWorkerResult result = app_worker_kill();

Possible `result` values when attempting to kill the worker are as follows:

| Result                                                                                                                               | Value | Description                                                                 |
|--------------------------------------------------------------------------------------------------------------------------------------|-------|:----------------------------------------------------------------------------|
| [`APP_WORKER_RESULT_SUCCESS`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_SUCCESS "APP_WORKER_RESULT_SUCCESS")                   | `0`   | The worker launch was killed successfully.                                  |
| [`APP_WORKER_RESULT_DIFFERENT_APP`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_DIFFERENT_APP "APP_WORKER_RESULT_DIFFERENT_APP") | `2`   | A worker from a different app is running, and cannot be killed by this app. |
| [`APP_WORKER_RESULT_NOT_RUNNING`](/docs/c/Foundation/AppWorker/#APP_WORKER_RESULT_NOT_RUNNING "APP_WORKER_RESULT_NOT_RUNNING")       | `3`   | The worker is not currently running.                                        |

## Available APIs

Background workers do not have access to the UI APIs. They also cannot use the [`AppMessage`](/docs/c/Foundation/AppMessage/ "AppMessage") API or load resources. Most other APIs are available including (but not limited to) [`AccelerometerService`](/docs/c/Foundation/Event_Service/AccelerometerService/ "AccelerometerService"), [`CompassService`](/docs/c/Foundation/Event_Service/CompassService/ "CompassService"), [`DataLogging`](/docs/c/Foundation/DataLogging/ "DataLogging"), [`HealthService`](/docs/c/Foundation/Event_Service/HealthService/ "HealthService"), [`ConnectionService`](/docs/c/Foundation/Event_Service/ConnectionService/ "ConnectionService"), [`BatteryStateService`](/docs/c/Foundation/Event_Service/BatteryStateService/ "BatteryStateService"), [`TickTimerService`](/docs/c/Foundation/Event_Service/TickTimerService/ "TickTimerService") and [`Storage`](/docs/c/Foundation/Storage/ "Storage").

The compiler will throw an error if the developer attempts to use an API unsupported by the worker. For a definitive list of available APIs, check `pebble_worker.h` in the SDK bundle for the presence of the desired API.

You need JavaScript enabled to read and post comments.

### Overview

- [Adding a Worker](#adding-a-worker)
- [Launching the Worker](#launching-the-worker)
- [Communicating Between Tasks](#communicating-between-tasks)
- [Managing the Worker](#managing-the-worker)
- [Available APIs](#available-apis)

### Related SDK Docs

- [Worker](/docs/c/Worker/)

### Examples

- [Background Counter](https://github.com/pebble-examples/feature-background-counter)
- [Background Worker Communication](https://github.com/pebble-examples/feature-worker-message)
